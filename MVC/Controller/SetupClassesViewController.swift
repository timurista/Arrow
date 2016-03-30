//
//  SetupClassesViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/8/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class SetupClassesViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {

    // MARK: Overridden Methods
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.delegate = self
        tableView.dataSource = self
    }
    
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        savingText?.hidden = true
        refresh()
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if let id = segue.identifier {
            switch id {
            case "next": enroll()
            case "save":
                let dvc = segue.destinationViewController as! MyClassesViewController
                dvc.suspendUI()
            default: break
            }
        }
    }
    
    // MARK: Properties
    private var display: [[AnyObject]] = [] // Format: [classObject: Class, selected: Bool]
    private let defaults = NSUserDefaults.standardUserDefaults()
    private let userDefaultsKey = "userSchoolID"
    private var schoolID: String { get { if let id = defaults.stringForKey(userDefaultsKey) { return id } else { return "" } } }
    private var error: NSError? { didSet{ self.errorHandling(error) } }
    
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var bottomButton: UIButton!
    @IBOutlet weak var savingText: UILabel!
    @IBAction func saveButton(sender: UIBarButtonItem) {
        suspendUI()
        savingText?.hidden = false
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            self.enroll()
            dispatch_async(dispatch_get_main_queue()){ () -> Void in
                self.performSegueWithIdentifier("save", sender: self)
            }
        }
    }
    
    // MARK: Functions
    private func refresh() { // Query database for classes
        suspendUI()
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            // Clear and fill display array
            if self.schoolID != "" {
                self.display.removeAll()
                let table = Table(type: 2)
                let classSearch = table.getObjectsWithKeyValue(["school": self.schoolID], limit: 0, error: &self.error) as! [Class]
                for classObject in classSearch {
                    classObject.getProfessor(&self.error)
                    self.display.append([classObject, false])
                }
                self.display.sortInPlace { ($0[0] as! Class).title.compare(($1[0] as! Class).title) == .OrderedAscending }
            }
            
            // Display tableView
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                self.updateUI()
            }
        }
    }
    
    private func suspendUI() {
        tableView.hidden = true
        bottomButton?.hidden = true
        spinner?.hidden = false
        spinner?.startAnimating()
    }
    
    private func updateUI() {
        spinner?.stopAnimating()
        spinner?.hidden = true
        tableView.reloadData()
        tableView.hidden = false
        bottomButton?.hidden = false
    }
    
    private func enroll() { // Creates an enrollment in the database
        for array in self.display {
            if array[1] as! Bool {
                if let classIdentifier = (array[0] as! Class).identifier {
                    CurrentUser().enrollInClass(classIdentifier, error: &self.error)
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
                        message: "Please check your internet connection. Then, go back and try again.",
                        preferredStyle:  UIAlertControllerStyle.Alert
                    )
                    alert.addAction(UIAlertAction(
                        title: "Dismiss",
                        style: .Cancel)
                        { (action: UIAlertAction) -> Void in
                            self.suspendUI()
                        }
                    )
                    self.presentViewController(alert, animated: true, completion: nil)
                default: // Error alert
                    let alert = UIAlertController(
                        title: "Error \(error!.code)",
                        message: "Something went wrong. Please go back and try again.",
                        preferredStyle:  UIAlertControllerStyle.Alert
                    )
                    alert.addAction(UIAlertAction(
                        title: "Dismiss",
                        style: .Cancel)
                        { (action: UIAlertAction) -> Void in
                            self.suspendUI()
                        }
                    )
                    self.presentViewController(alert, animated: true, completion: nil)
                }
            }
        }
    }
}

extension SetupClassesViewController { // TableView implementation
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if display.count == 0 {
            return 1
        } else {
            return display.count
        }
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        if display.count != 0 {
            if display[indexPath.row][1] as! Bool == true {
                display[indexPath.row][1] = false
            } else {
                display[indexPath.row][1] = true
            }
        }
        tableView.reloadData()
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("cell", forIndexPath: indexPath)
        
        // Configure the cell...
        if display.count == 0 {
            cell.textLabel?.text = "No Classes"
            cell.detailTextLabel?.text = "Add one using the button at the top"
        } else {
            cell.textLabel?.text = (display[indexPath.row][0] as! Class).title
            let professor = (display[indexPath.row][0] as! Class).professorObject
            let professorName: String = professor.getName()
            cell.detailTextLabel?.text = professorName
            
            if display[indexPath.row][1] as! Bool == true {
                cell.accessoryType = .Checkmark
            } else {
                cell.accessoryType = .None
            }
        }
        return cell
    }
}

extension SetupClassesViewController { // Functionality to allow users to add a class
    
    @IBAction func addButton(sender: UIBarButtonItem) {
        performSegueWithIdentifier("addClass", sender: self)
    }
    @IBAction func newClassButton(sender: UIButton) {
        performSegueWithIdentifier("addClass", sender: self)
    }
    @IBAction func addClassReturn(segue: UIStoryboardSegue) {}
}