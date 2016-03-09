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
    override func viewDidLoad() {
        super.viewDidLoad()
        refresh()
    }
    
    // MARK: Properties
    var schoolName: String = ""// Passed from previous view controller
    private var school: School {
        get {
            let table = Table(type: 1)
            var error: NSError?
            let schoolSearch = table.getObjectsWithKeyValue(["name": schoolName], limit: 1, error: &error)
            if schoolSearch.count == 1 {
                return schoolSearch[0] as! School
            } else {
                return School(schoolName: nil, stateAbreviation: nil)
            }
        }
    }
    private var classes: [[AnyObject]] = [] // follows the format[classObject: Class, professor: Professor, selected: Bool]
    
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBOutlet weak var tableView: UITableView!
    
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
            let professorName: String =  (professor.firstName != "" && professor.lastName != "") ? (professor.lastName + ", " + professor.firstName) : ""
            cell.detailTextLabel?.text = professorName
            
            if classes[indexPath.row][2] as! Bool == true {
                cell.accessoryType = .Checkmark
            } else {
                cell.accessoryType = .None
            }
        }
        return cell
    }
    
    private func refresh() { // Query database for classes
        suspendUI()
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            var error: NSError?
            
            // Fill classes array
            let schoolIdentifier = self.school.identifier
            if schoolIdentifier != nil {
                let table = Table(type: 2)
                var classSearch = table.getObjectsWithKeyValue(["school": schoolIdentifier!], limit: 0, error: &error) as [AnyObject]
                classSearch.sortInPlace { ($0 as! Class).title.compare(($1 as! Class).title) == .OrderedAscending }
                self.classes.removeAll()
                for object in classSearch {
                    let classObject = object as! Class
                    let professor = classObject.getProfessor(&error)
                    self.classes.append([classObject, professor, false])
                }
            }
            self.errorHandling(error)
            
            // Display tableView
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                self.tableView?.reloadData()
                self.updateUI()
            }
        }
    }
    
    private func suspendUI(){
        tableView?.hidden = true
        spinner?.hidden = false
        spinner?.startAnimating()
    }
    
    private func updateUI(){
        spinner?.stopAnimating()
        spinner?.hidden = true
        tableView?.hidden = false
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
                        title: "Error",
                        message: "Something went wrong. Error \(error!.code)",
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