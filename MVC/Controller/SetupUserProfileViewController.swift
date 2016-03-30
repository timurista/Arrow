//
//  SetupUserProfileViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/25/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class SetupUserProfileViewController: UIViewController, UITextFieldDelegate {
    
    // MARK: Overridden Methods
    override func viewDidLoad() {
        super.viewDidLoad()
        firstNameTextField.delegate = self
        lastNameTextField.delegate = self
    }
    
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
    }

    override func shouldPerformSegueWithIdentifier(identifier: String, sender: AnyObject?) -> Bool {
        switch identifier {
        case "exitSetup":
            if firstNameTextField.text == nil || lastNameTextField.text == nil { return false }
        default: break
        }
        return true
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        let firstName = firstNameTextField.text!.stringByTrimmingCharactersInSet(
            NSCharacterSet.whitespaceAndNewlineCharacterSet()
        )
        let lastName = lastNameTextField.text!.stringByTrimmingCharactersInSet(
            NSCharacterSet.whitespaceAndNewlineCharacterSet()
        )
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            CurrentUser().setName(firstName, lastName: lastName, error: &self.error)
        }
    }
    
    // MARK: Properties
    private var error: NSError? { didSet{ self.errorHandling(error) } }
    
    @IBOutlet weak var firstNameTextField: UITextField!
    @IBOutlet weak var lastNameTextField: UITextField!
    @IBAction func dismiss(sender: UITapGestureRecognizer) {
        firstNameTextField.resignFirstResponder()
        lastNameTextField.resignFirstResponder()
    }
    
    // MARK: Functions
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return true
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
                        // Do nothing
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
                    }
                    )
                    self.presentViewController(alert, animated: true, completion: nil)
                }
            }
        }
    }
}

extension SetupUserProfileViewController { // Functionality for user profile picture
    
    @IBAction func editButton(sender: UIButton) {
        let alert = UIAlertController(
            title: "Unsupported Feature",
            message: "This application does not currently support changing user profile pictures.",
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
    }
    
    
}
