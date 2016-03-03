//
//  LoginViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 2/16/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class LoginViewController: UIViewController {

    
    // MARK: Overrided Methods
    override func supportedInterfaceOrientations() -> UIInterfaceOrientationMask { // Only allow portrait orientation for login
        return UIInterfaceOrientationMask.Portrait
    }
    
    override func viewDidDisappear(animated: Bool) {
        super.viewDidDisappear(animated)
        resetUI()
    }
    
    // MARK: Properties
    @IBAction func loginButton(sender: UIButton) {
        switch sender.tag{
        case 1: logIn(.Googleplus)
        case 2: logIn(.Facebook)
        case 3: logIn(.Twitter)
        default: break
        }
    }
    @IBOutlet weak var googleButton: UIButton!
    @IBOutlet weak var facebookButton: UIButton!
    @IBOutlet weak var twitterButton: UIButton!
    @IBAction func logOut(segue: UIStoryboardSegue) {}
    @IBOutlet weak var spinner: UIActivityIndicatorView!

    // MARK: Functions
    private func suspendUI() {
        googleButton?.hidden = true
        facebookButton?.hidden = true
        twitterButton?.hidden = true
        spinner?.hidden = false
        spinner?.startAnimating()
    }
    
    private func resetUI() {
        googleButton?.hidden = false
        facebookButton?.hidden = false
        twitterButton?.hidden = false
        spinner?.hidden = true
    }
    
    private func logIn(provider: KiiConnectorProvider){
        // Set options to nil to indicate that SDK will handle the UI
        let options : Dictionary<String,AnyObject>? = nil
        var didRun: Bool = false
        
        // Login
        KiiSocialConnect.logIn(provider, options: options) { (retUser, provider, retError) -> Void in
            self.suspendUI()
            if retError == nil && !didRun {
                // Successful login
                self.loggedIn()
                didRun = true
            } else if retError != nil {
                print("Login return error code: \(retError.code)")
                switch retError.code {
                case 322: // No internet connection alert
                    let alert = UIAlertController(
                        title: "Offline",
                        message: "Please check your internet connection and try again.",
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
                case 320:
                    self.resetUI()
                default: break
                }
            }
        }
    }
    
    private func loggedIn() { // Transition away from login screen
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            CurrentUser().refresh()
            CurrentUser().setUpUserObject()
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                if self.shouldLoadSetup() {
                    self.performSegueWithIdentifier("setup", sender: self)
                } else {
                    self.performSegueWithIdentifier("home", sender: self)
                }
            }
        }
    }
    
    private func shouldLoadSetup() -> Bool { // Determine if user setup should be run
        if CurrentUser().school == nil { return true }
        if CurrentUser().school?.name == "" { return true }
        return true // !!!!For Testing!!!!
    }
}
