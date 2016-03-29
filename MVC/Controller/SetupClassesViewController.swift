//
//  SetupClassesViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/8/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class SetupClassesViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {

    // MARK: Overrided Methods
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        refresh()
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if let id = segue.identifier {
            switch id {
            case "addClass":
                let dvc = segue.destinationViewController as! UINavigationController
                let ultimateDVC = dvc.topViewController as! AddClassViewController
                ultimateDVC.school = school
            case "next":
                enroll()
            default: break
            }
        }
    }
    
    // MARK: Properties
    var schoolName: String = "" // Passed from previous view controller
    var classToAdd: Class = Class(classTitle: nil, schoolID: nil, professorID: nil) // Passed from add class view controller
    private var school: School = School(schoolName: nil, stateAbreviation: nil)
    private var classes: [[AnyObject]] = [] // follows the format[classObject: Class, professor: Professor, selected: Bool]
    
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var continueButton: UIButton!

    
    // MARK: Functions
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if classes.count == 0 {
            return 1
        } else {
            return classes.count
        }
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        if classes.count != 0 {
            if classes[indexPath.row][2] as! Bool == true {
                classes[indexPath.row][2] = false
            } else {
                classes[indexPath.row][2] = true
            }
        }
        tableView.reloadData()
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("cell", forIndexPath: indexPath)
        
        // Configure the cell...
        if classes.count == 0 {
            cell.textLabel?.text = "No Classes"
            cell.detailTextLabel?.text = "Add one using the button at the top"
        } else {
            cell.textLabel?.text = (classes[indexPath.row][0] as! Class).title
            let professor = classes[indexPath.row][1] as! Professor
            let professorName: String = professor.getName()
            cell.detailTextLabel?.text = professorName
            
            if classes[indexPath.row][2] as! Bool == true {
                cell.accessoryType = .Checkmark
            } else {
                cell.accessoryType = .None
            }
        }
        return cell
    }
    
    private func suspendUI() {
        tableView?.hidden = true
        continueButton?.hidden = true
        spinner?.hidden = false
        spinner?.startAnimating()
    }
    
    private func updateUI() {
        spinner?.stopAnimating()
        spinner?.hidden = true
        tableView?.hidden = false
        continueButton?.hidden = false
    }
    
    private func setSchoolObject() { // Creates school object from the school ID passed from previous view
        let table = Table(type: 1)
        var error: NSError?
        let schoolSearch = table.getObjectsWithKeyValue(["name": self.schoolName], limit: 1, error: &error)
        if schoolSearch.count == 1 {
            self.school = schoolSearch[0] as! School
        }
    }
    
    private func refresh() { // Query database for classes
        suspendUI()
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            var error: NSError?
            
            // Add new class if necessary
            if self.classToAdd.title != "" {
                self.classToAdd.addToDatabase(&error)
                self.classToAdd = Class(classTitle: nil, schoolID: nil, professorID: nil)
            }
            self.errorHandling(error)
            
            // Fill classes array
            if self.school.identifier == nil {
                self.setSchoolObject()
            }
            if self.school.identifier != nil {
                let table = Table(type: 2)
                let classSearch = table.getObjectsWithKeyValue(["school": self.school.identifier!], limit: 0, error: &error) as [AnyObject]
                for object in classSearch {
                    var shouldAdd = true
                    let classObject = object as! Class
                    let professor = classObject.getProfessor(&error)
                    for existingObject in self.classes {
                        let existingClassObject = existingObject[0] as! Class
                        if classObject.identifier == existingClassObject.identifier { shouldAdd = false }
                    }
                    if shouldAdd { self.classes.append([classObject, professor, false]) }
                }
                self.classes.sortInPlace { ($0[0] as! Class).title.compare(($1[0] as! Class).title) == .OrderedAscending }
            }
            self.errorHandling(error)
            
            // Display tableView
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                self.tableView?.reloadData()
                self.updateUI()
            }
        }
    }
    
    private func enroll() { // Creates an enrollment in the database
        var error: NSError?
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            for array in self.classes {
                let classObject = array[0] as! Class
                if array[2] as! Bool {
                    if let classIdentifier = classObject.identifier {
                        CurrentUser().enrollInClass(classIdentifier, error: &error)
                    }
                }
            }
        }
        errorHandling(error)
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

extension SetupClassesViewController { // Functionality to allow users to add a class
    
    override func shouldPerformSegueWithIdentifier(identifier: String, sender: AnyObject?) -> Bool {
        if identifier == "addClass" && school.identifier == nil { return false }
        return true
    }
    
    @IBAction func addButton(sender: UIBarButtonItem) {
        performSegueWithIdentifier("addClass", sender: self)
    }
    @IBAction func didAddClass(segue: UIStoryboardSegue) {}
    @IBAction func cancelAddClass(segue: UIStoryboardSegue) {}
    
}