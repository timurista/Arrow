//
//  ClassViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 4/4/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class ClassViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {

    // MARK: Overridden Methods
    override func viewDidLoad() {
        super.viewDidLoad()
        navigationBar.title = classToDisplay.title
        tableView.delegate = self
        tableView.dataSource = self
        tableView.estimatedRowHeight = tableView.rowHeight
        tableView.rowHeight = UITableViewAutomaticDimension
        tableView.addSubview(self.refreshControl)
        load()
    }
    
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if let id = segue.identifier {
            switch id {
            case "goToComments":
                let dvc = segue.destinationViewController as! CommentsViewController
                dvc.postToDisplay = passToComments
            default: break
            }
        }
    }
    
    // MARK: Properties
    var classToDisplay: Class = Class(classTitle: nil, schoolID: nil, professorID: nil) // Passed from previous view controller
    private var display: [Post] = []
    private var passToComments: Post = Post(0)
    lazy var refreshControl: UIRefreshControl = {
        let refreshControl = UIRefreshControl()
        refreshControl.addTarget(self, action: #selector(ClassViewController.handleRefresh(_:)), forControlEvents: UIControlEvents.ValueChanged)
        return refreshControl
    }()
    private let defaults = NSUserDefaults.standardUserDefaults()
    private var error: NSError? { didSet{ self.errorHandling(error) } }
    
    @IBOutlet weak var navigationBar: UINavigationItem!
    @IBOutlet weak var tableView: UITableView!
    
    // MARK: Functions
    private func load() {
        suspendUI()
        // Get stored data from NSUserDefaults if applicable
        if let decoded  = defaults.objectForKey(UserDefaults().keyForPosts) as? NSData {
            let decodedPosts = NSKeyedUnarchiver.unarchiveObjectWithData(decoded) as! [Post]
            if decodedPosts.count != 0 {
                for post in decodedPosts {
                    if let classID = classToDisplay.identifier {
                        if post.classID == classID {
                            display.append(post)
                        }
                    }
                }
                
            }
        }
        updateUI()
        refresh()
    }
    
    private func refresh() {
        suspendUI()
        var temp: [Post] = []
        let qos = Int(QOS_CLASS_BACKGROUND.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            let table = Table(type: 3)
            if let classID = self.classToDisplay.identifier {
                let results = table.getObjectsWithKeyValue(["class": classID], limit: 0, error: &self.error) as! [Post]
                temp = results
            }
            dispatch_async(dispatch_get_main_queue()){ () -> Void in
                // Store data from database in NSUserDefaults
                var postsToKeep: [Post] = []
                if let decoded  = self.defaults.objectForKey(UserDefaults().keyForPosts) as? NSData {
                    let decodedPosts = NSKeyedUnarchiver.unarchiveObjectWithData(decoded) as! [Post]
                    for post in decodedPosts {
                        if let classID = self.classToDisplay.identifier {
                            if post.classID != classID {
                                postsToKeep.append(post)
                            }
                        }
                    }
                }
                postsToKeep.appendContentsOf(temp)
                let encodedData = NSKeyedArchiver.archivedDataWithRootObject(postsToKeep)
                self.defaults.setObject(encodedData, forKey: UserDefaults().keyForPosts)
                
                // Reload UI
                self.display = temp
                self.updateUI()
            }
        }
    }
    
    private func suspendUI() {
        refreshControl.beginRefreshing()
    }
    
    private func updateUI() {
        display.sortInPlace { "\($0.date)".compare("\($1.date)") == .OrderedDescending }
        tableView.reloadData()
        refreshControl.endRefreshing()
    }
    
    @IBAction func handleRefresh(refreshControl: UIRefreshControl) {
        refresh()
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

extension ClassViewController { // TableView implementation
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int { return 1 }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int { return display.count }
    
    func tableView(tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat { return CGFloat.min }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("postCell", forIndexPath: indexPath) as! PostTableViewCell
        cell.postToDisplay = display[indexPath.row]
        cell.likeButton.tag = indexPath.row
        cell.likeButton.addTarget(self, action: #selector(ClassViewController.like(_:)), forControlEvents: .TouchUpInside)
        cell.moreButton.tag = indexPath.row
        cell.moreButton.addTarget(self, action: #selector(ClassViewController.more(_:)), forControlEvents: .TouchUpInside)
        cell.commentButton.tag = indexPath.row
        cell.commentButton.addTarget(self, action: #selector(ClassViewController.comment(_:)), forControlEvents: .TouchUpInside)
        return cell
    }
    
    @IBAction func like(sender: UIButton) { // Like button tapped
        if display[sender.tag].liked {
            display[sender.tag].liked = false
            display[sender.tag].numberOfLikes -= 1
            tableView.reloadData()
            let qos = Int(QOS_CLASS_BACKGROUND.rawValue)
            dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
                self.display[sender.tag].unlike(&self.error)
            }
        } else {
            display[sender.tag].liked = true
            display[sender.tag].numberOfLikes += 1
            tableView.reloadData()
            let qos = Int(QOS_CLASS_BACKGROUND.rawValue)
            dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
                self.display[sender.tag].like(&self.error)
            }
        }
    }
    
    @IBAction func comment(sender: UIButton) {
        passToComments = display[sender.tag]
        performSegueWithIdentifier("goToComments", sender: self)
    }
    
    @IBAction func more(sender: UIButton) {
    }
}