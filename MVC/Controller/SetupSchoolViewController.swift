//
//  SetupSchoolViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 2/29/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class SetupSchoolViewController: GenericPickerViewController {
    
    // MARK: Overrided Methods
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        getSchoolNames()
    }
    
    override func updateUI(list: [String]) {
        super.updateUI(list)
        picker?.selectRow(0, inComponent: 0, animated: false)
        selectedValue = nil
        picker?.hidden = false
        spinner?.stopAnimating()
        button?.hidden = false
    }
    
    // MARK: Properties
    var stateAbreviation: String = "" // Passed from previous view controller
    private var schoolsInState: [School] = []
    private let defaults = NSUserDefaults.standardUserDefaults()
    private let userDefaultsKey = "userSchoolID"
    private var error: NSError? { didSet{ self.errorHandling(error) } }
    
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBOutlet weak var button: UIButton!
    @IBAction func continueButton() {
        if selectedValue != nil {
            setUserSchool()
        }
    }
    
    // MARK: Functions
    private func suspendUI() {
        picker?.hidden = true
        spinner?.startAnimating()
        button?.hidden = true
    }
    
    private func getSchoolNames() {
        suspendUI()
        // Query database off the main thread
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            var schoolNames: [String] = []
            // Query database for schools with matching state
            let table = Table(type: 1)
            self.schoolsInState = table.getObjectsWithKeyValue(["state": self.stateAbreviation], limit: 0, error: &self.error) as! [School]
            for school in self.schoolsInState {
                schoolNames.append(school.name)
            }
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                // Load UI with database results
                if schoolNames.count == 0 {
                    self.blank = "No Schools"
                    self.updateUI([])
                } else {
                    self.blank = "-"
                    self.updateUI(schoolNames)
                }
            }
        }
    }
    
    private func setUserSchool() { // Sets the user's school in the database and in NSUserDefaults
        suspendUI()
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            for school in self.schoolsInState {
                if school.name == self.selectedValue! {
                    if let schoolID = school.identifier {
                        self.defaults.setObject(schoolID, forKey: self.userDefaultsKey)
                    }
                    CurrentUser().setSchool(school, error: &self.error)
                }
            }
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                self.performSegueWithIdentifier("next", sender: self)
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
                            self.blank = "Error"
                            self.updateUI([])
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

extension SetupSchoolViewController { // Functionality to allow users to add a school
    
    @IBAction func addButton(sender: UIBarButtonItem) {
        if pickerList.count > 0 && pickerList[0] != "Error" {
            let addSchool = UIAlertController(
                title: "Add School",
                message: "Enter the name of your school.",
                preferredStyle:  UIAlertControllerStyle.Alert
            )
            addSchool.addAction(UIAlertAction(
                title: "OK",
                style: .Default)
                { (action: UIAlertAction) -> Void in
                    // Get school name
                    if let textFieldPointer = addSchool.textFields?.first {
                        let textField = textFieldPointer as UITextField
                        if let schoolName = textField.text {
                            // Add school to database
                            self.createSchool(schoolName)
                        }
                    }
                }
            )
            addSchool.addAction(UIAlertAction(
                title: "Cancel",
                style: .Cancel)
                { (action: UIAlertAction) -> Void in
                    // Do nothing
                }
            )
            addSchool.addTextFieldWithConfigurationHandler { (textField) in
                textField.placeholder = "School Name"
                textField.text = ""
                textField.keyboardType = .ASCIICapable
                textField.autocapitalizationType = .Words
            }
            presentViewController(addSchool, animated: true, completion: nil)
        }
    }
    
    private func createSchool(name: String) {
        if self.stateAbreviation != "" && name != ""{
            // Remove leading and trailing spaces
            var name = name.stringByTrimmingCharactersInSet(
                NSCharacterSet.whitespaceAndNewlineCharacterSet()
            )

            // Reduce all other spaces to a single space
            let components = name.componentsSeparatedByCharactersInSet(NSCharacterSet.whitespaceAndNewlineCharacterSet())
            name = components.filter { !$0.isEmpty }.joinWithSeparator(" ")

            //Check for duplicate in list
            var duplicate: Bool = false
            for listItem in pickerList {
                if name.lowercaseString == listItem.lowercaseString {
                    duplicate = true
                }
            }

            //Add school to database
            if !duplicate {
                let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
                dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
                    let newSchool = School(schoolName: name, stateAbreviation: self.stateAbreviation)
                    newSchool.addToDatabase(&self.error)
                    dispatch_async(dispatch_get_main_queue()) { () -> Void in
                        self.selectedValue = name
                        self.continueButton()
                    }
                }
            }
        }
    }
}