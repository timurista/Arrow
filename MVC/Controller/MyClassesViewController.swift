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
        getUserClasses()
    }
    
    // MARK: Properties
    var userClasses: [Class] = []
    var professors: [Professor] = []
    
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    
    // MARK: Functions
    func getUserClasses() {
        suspendUI()
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            var error: NSError?
            let table = Table(type: 8)
            if let userID = CurrentUser().userID {
                let searchResults = table.getObjectsWithKeyValue(["user": userID], limit: 0, error: &error)
                for result in searchResults {
                    self.userClasses.append((result as! Enrollment).getClass(&error))
                }
                self.userClasses.sortInPlace { $0.title.compare($1.title) == .OrderedAscending }
                for userClass in self.userClasses {
                    self.professors.append(userClass.getProfessor(&error))
                }
            }
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                self.tableView?.reloadData()
                self.updateUI()
            }
        }
    }
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return userClasses.count
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("reuseIdentifier", forIndexPath: indexPath) as! MyClassesTableViewCell
        
        // Configure the cell...
        cell.infoToDisplay = [userClasses[indexPath.section],professors[indexPath.section]]
        
        return cell
    }
    
    func suspendUI() {
        spinner?.hidden = false
        spinner?.startAnimating()
    }
    
    func updateUI() {
        spinner?.stopAnimating()
        spinner?.hidden = true
    }
}
