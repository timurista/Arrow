//
//  MyClassesViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/29/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class MyClassesViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {

    // MARK: Overrided Methods
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView?.delegate = self
        tableView?.dataSource = self
        load()
    }
    
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        refresh()
    }
    
    // MARK: Propertiess
    var display: [Class] = []
    private let defaults = NSUserDefaults.standardUserDefaults()
    private let userDefaultsKey = "myClassesArray"
    private var error: NSError? { didSet{ self.errorHandling(error) } }
    
    @IBAction func unwindToMyClasses(segue: UIStoryboardSegue) {}
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBAction func editButton(sender: UIBarButtonItem) {
        // Switch between editing and not editing
        if !tableView.editing {
            sender.style = .Done
            sender.title = "Done"
            tableView.setEditing(true, animated: true)
        } else {
            sender.style = .Plain
            sender.title = "Edit"
            tableView.setEditing(false, animated: true)
            refresh()
        }
    }
    
    // MARK: Functions
    private func load() {
        suspendUI()
        // Get stored data from NSUserDefaults if applicable
        if let storedArray = defaults.arrayForKey(userDefaultsKey) {
            if storedArray.count != 0 {
                for storedClassArray in storedArray {
                    let classObject = Class(fromStoredArray: storedClassArray as! [String])
                    display.append(classObject)
                }
            }
            updateUI()
        }
    }
    
    private func refresh() {
        var temp: [Class] = []
        let qos = Int(QOS_CLASS_BACKGROUND.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            // Get user's classes from the database
            let table = Table(type: 8)
            if let userID = CurrentUser().userID {
                let searchResults = table.getObjectsWithKeyValue(["user": userID], limit: 0, error: &self.error)
                for result in searchResults {
                    let classToAdd = (result as! Enrollment).getClass(&self.error)
                    classToAdd.getProfessor(&self.error)
                    temp.append(classToAdd)
                }
            }
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                // Store data from database in NSUserDefaults
                var arrayToStore: [NSArray] = []
                for classObject in temp {
                    let classArrayToStore = classObject.getStorableArray()
                    arrayToStore.append(classArrayToStore)
                }
                self.defaults.removeObjectForKey(self.userDefaultsKey)
                self.defaults.setObject(arrayToStore, forKey: self.userDefaultsKey)
                
                // Reload UI
                self.suspendUI()
                self.display = temp
                self.updateUI()
            }
        }
    }
    
    func suspendUI() {
        display.removeAll()
        tableView.reloadData()
        spinner?.hidden = false
        spinner?.startAnimating()
    }
    
    private func updateUI() {
        display.sortInPlace { $0.title.compare($1.title) == .OrderedAscending }
        tableView.reloadData()
        spinner?.stopAnimating()
        spinner?.hidden = true
    }
    
    private func removeClass(index: Int) {
        if let classID = display[index].identifier {
            display.removeAtIndex(index)
            tableView.reloadData()
            let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
            dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
                if let userID = CurrentUser().userID {
                    let table = Table(type: 8)
                    table.deleteObjectWithStringKeys(["user": userID, "class": classID], error: &self.error)
                }
            }
        }
    }
    
    private func errorHandling(error: NSError?) {
        dispatch_async(dispatch_get_main_queue()) { () -> Void in
            if error != nil {
                print("Error: Code \(error!.code), \(error!.description)")
                switch error!.code {
                case 201: // No internet connection alert
                    let alert = UIAlertController(
                        title: "Offline",
                        message: "Please check your internet connection.",
                        preferredStyle:  UIAlertControllerStyle.Alert
                    )
                    alert.addAction(UIAlertAction(
                        title: "Dismiss",
                        style: .Cancel)
                    { (action: UIAlertAction) -> Void in
                        // Do nothing
                        }
                    )
                    self.presentViewController(alert, animated: true, completion: nil)
                default: // Error alert
                    let alert = UIAlertController(
                        title: "Error \(error!.code)",
                        message: "Something went wrong.",
                        preferredStyle:  UIAlertControllerStyle.Alert
                    )
                    alert.addAction(UIAlertAction(
                        title: "Dismiss",
                        style: .Cancel)
                    { (action: UIAlertAction) -> Void in
                        }
                    )
                    self.presentViewController(alert, animated: true, completion: nil)
                }
            }
        }
    }
}

extension MyClassesViewController { // TableView implementation
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int { return display.count }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int { return 1 }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("reuseIdentifier", forIndexPath: indexPath) as! MyClassesTableViewCell
        cell.classToDisplay = display[indexPath.section]
        return cell
    }
    
    func tableView(tableView: UITableView, canEditRowAtIndexPath indexPath: NSIndexPath) -> Bool { return tableView.editing }
    
    func tableView(tableView: UITableView, editingStyleForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCellEditingStyle { return .Delete }
    
    func tableView(tableView: UITableView, commitEditingStyle editingStyle: UITableViewCellEditingStyle, forRowAtIndexPath indexPath: NSIndexPath) {
        if editingStyle == .Delete {
            // Delete the row from the data source
            removeClass(indexPath.section)
        }
    }
}
