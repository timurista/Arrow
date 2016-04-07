//
//  CommentsViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 4/7/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class CommentsViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {

    // MARK: Overridden Methods
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.delegate = self
        tableView.dataSource = self
        tableView.estimatedRowHeight = tableView.rowHeight
        tableView.rowHeight = UITableViewAutomaticDimension
        refresh()
    }
    
    // MARK: Properties
    var postToDisplay: Post = Post(0) // Passed from previous view controller
    private var display: [Comment] = []
    private let defaults = NSUserDefaults.standardUserDefaults()
    private var error: NSError? { didSet{ self.errorHandling(error) } }
    
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    
    // MARK: Functions
    private func refresh() {
        suspendUI()
        let qos = Int(QOS_CLASS_BACKGROUND.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            let table = Table(type: 4)
            if let postID = self.postToDisplay.identifier {
                let results = table.getObjectsWithKeyValue(["post": postID], limit: 0, error: &self.error) as! [Comment]
                self.display.removeAll()
                self.display = results
            }
            dispatch_async(dispatch_get_main_queue()){ () -> Void in
                self.updateUI()
            }
        }
    }
    
    private func suspendUI() {
        spinner?.hidden = false
        spinner?.startAnimating()
    }
    
    private func updateUI() {
        display.sortInPlace { "\($0.date)".compare("\($1.date)") == .OrderedAscending }
        tableView.reloadData()
        spinner?.stopAnimating()
        spinner?.hidden = true
    }
    
    private func removeComment(index: Int) {
        if let commentID = display[index].identifier {
            display.removeAtIndex(index)
            tableView.reloadData()
            let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
            dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
                let table = Table(type: 4)
                table.deleteObjectWithStringKeys(["_id": commentID], error: &self.error)
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

extension CommentsViewController { // TableView implementation
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int { return 1 }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int { return display.count }
    
    func tableView(tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat { return CGFloat.min }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("commentCell", forIndexPath: indexPath) as! CommentTableViewCell
        cell.commentToDisplay = display[indexPath.row]
        return cell
    }
    
    func tableView(tableView: UITableView, canEditRowAtIndexPath indexPath: NSIndexPath) -> Bool {
        if display[indexPath.row].user.userID == defaults.stringForKey(UserDefaults().keyForUserID) {
            return true
        } else if postToDisplay.user.userID == defaults.stringForKey(UserDefaults().keyForUserID) {
            return true
        } else {
            return false
        }
    }
    
    func tableView(tableView: UITableView, editingStyleForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCellEditingStyle { return .Delete }
    
    func tableView(tableView: UITableView, commitEditingStyle editingStyle: UITableViewCellEditingStyle, forRowAtIndexPath indexPath: NSIndexPath) {
        if editingStyle == .Delete {
            // Delete the row from the data source
            removeComment(indexPath.row)
        }
    }
}
