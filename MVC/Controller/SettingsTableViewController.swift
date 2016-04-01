//
//  SettingsTableViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/30/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class SettingsTableViewController: UITableViewController {

    // MARK: Overridden Methods
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if let id = segue.identifier {
            switch id {
            case "logOut":
                let qos = Int(QOS_CLASS_BACKGROUND.rawValue)
                dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
                    CurrentUser().logOut()
                }
            default: break
            }
        }
    }
    
    // MARK: Properties
    @IBAction func unwindToSettings(segue: UIStoryboardSegue) {}
}
