//
//  LoginViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 2/16/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class LoginViewController: UIViewController {

    // MARK: Overridden Methods
    override func viewDidLoad() {
        super.viewDidLoad()
        if CurrentUser().userID != nil {
            suspendUI()
            loggedIn()
        } else {
            resetUI()
        }
    }
    
    override func viewDidDisappear(animated: Bool) {
        super.viewDidDisappear(animated)
        resetUI()
    }
    
    // MARK: Properties
    private let defaults = NSUserDefaults.standardUserDefaults()
    private var error: NSError? { didSet{ self.errorHandling(error) } }
    
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
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBOutlet weak var loggingInText: UILabel!
    @IBAction func logOut(segue: UIStoryboardSegue) {}

    // MARK: Functions
    private func suspendUI() {
        googleButton?.hidden = true
        facebookButton?.hidden = true
        twitterButton?.hidden = true
        spinner?.hidden = false
        spinner?.startAnimating()
        loggingInText?.hidden = false
    }
    
    private func resetUI() {
        googleButton?.hidden = false
        facebookButton?.hidden = false
        twitterButton?.hidden = false
        spinner?.stopAnimating()
        spinner?.hidden = true
        loggingInText?.hidden = true
    }
    
    private func logIn(provider: KiiConnectorProvider){
        // Set options to nil to indicate that SDK will handle the UI
        let options : Dictionary<String,AnyObject>? = nil
        var didRun: Bool = false
        
        // Login
        KiiSocialConnect.logIn(provider, options: options) { (retUser, provider, retError) -> Void in
            self.suspendUI()
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                if retError == nil && !didRun {
                    // Successful login
                    self.loggedIn()
                    didRun = true
                } else if retError != nil {
                    print("Login return error code: \(retError.code)")
                    self.errorHandling(retError)
                }
            }
        }
    }
    
    private func loggedIn() { // Transition away from login screen
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            CurrentUser().refresh(&self.error)
            CurrentUser().setUpUserObject(&self.error)
            if let userID = CurrentUser().userID {
                self.defaults.setObject(userID, forKey: UserDefaults().keyForUserID)
            }
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
        if CurrentUser().firstName == nil { return true }
        if CurrentUser().lastName == nil {return true }
        return false
    }
    
    private func errorHandling(error: NSError?) {
        if error != nil {
            print("Error: Code \(error!.code), \(error!.description)")
            switch error!.code {
            case 201, 322: // No internet connection alert
                let alert = UIAlertController(
                    title: "Offline",
                    message: "Please check your internet connection and try again.",
                    preferredStyle:  UIAlertControllerStyle.Alert
                )
                alert.addAction(UIAlertAction(
                    title: "Dismiss",
                    style: .Cancel)
                    { (action: UIAlertAction) -> Void in
                        self.resetUI()
                    }
                )
                self.presentViewController(alert, animated: true, completion: nil)
            case 320: break
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
                    self.suspendUI()
                    }
                )
                self.presentViewController(alert, animated: true, completion: nil)
            }
        }
    }
}
