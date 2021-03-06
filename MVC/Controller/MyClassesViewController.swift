//
//  MyClassesViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/29/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class MyClassesViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {

    // MARK: Overridden Methods
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
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if let id = segue.identifier {
            switch id {
            case "goToClass":
                if let selectedRow = tableView.indexPathForSelectedRow?.section {
                    let dvc = segue.destinationViewController as! ClassViewController
                    dvc.classToDisplay = display[selectedRow]
                }
            default: break
            }
        }
    }
    
    // MARK: Properties
    var display: [Class] = []
    private let defaults = NSUserDefaults.standardUserDefaults()
    private var error: NSError? { didSet{ self.errorHandling(error) } }
    
    @IBAction func unwindToMyClasses(segue: UIStoryboardSegue) {}
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBOutlet weak var noClassesLabel: UILabel!
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
        if let decoded  = defaults.objectForKey(UserDefaults().keyForMyClasses) as? NSData {
            let decodedClasses = NSKeyedUnarchiver.unarchiveObjectWithData(decoded) as! [Class]
            if decodedClasses.count != 0 {
                display = decodedClasses
                updateUI()
            }
        }
    }
    
    func refresh() {
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
            
            // Store current user information
            let currentUser = CurrentUser()
            if let schoolID = currentUser.school?.identifier {
                self.defaults.setObject(schoolID, forKey: UserDefaults().keyForUserSchool)
            }
            if let firstName = currentUser.firstName {
                self.defaults.setObject(firstName, forKey: UserDefaults().keyForUserFirstName)
            }
            if let lastName = currentUser.lastName {
                self.defaults.setObject(lastName, forKey: UserDefaults().keyForUserLastName)
            }
            
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                // Store data from database in NSUserDefaults
                let encodedData = NSKeyedArchiver.archivedDataWithRootObject(temp)
                self.defaults.removeObjectForKey(UserDefaults().keyForMyClasses)
                self.defaults.setObject(encodedData, forKey: UserDefaults().keyForMyClasses)
                
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
        noClassesLabel?.hidden = true
        spinner?.hidden = false
        spinner?.startAnimating()
    }
    
    private func updateUI() {
        display.sortInPlace { $0.title.compare($1.title) == .OrderedAscending }
        tableView.reloadData()
        if display.count == 0 {
            noClassesLabel?.hidden = false
        }
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
    
    func tableView(tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat { return 20 }
    
    func tableView(tableView: UITableView, heightForFooterInSection section: Int) -> CGFloat { return CGFloat.min }
    
    func tableView(tableView: UITableView, canEditRowAtIndexPath indexPath: NSIndexPath) -> Bool { return tableView.editing }
    
    func tableView(tableView: UITableView, editingStyleForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCellEditingStyle { return .Delete }
    
    func tableView(tableView: UITableView, commitEditingStyle editingStyle: UITableViewCellEditingStyle, forRowAtIndexPath indexPath: NSIndexPath) {
        if editingStyle == .Delete {
            // Delete the row from the data source
            removeClass(indexPath.section)
        }
    }
}
