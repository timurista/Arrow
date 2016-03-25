//
//  AddClassViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/10/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class AddClassViewController: GenericPickerViewController, UITextFieldDelegate {

    // MARK: Overrided Methods
    override func viewDidLoad() {
        super.viewDidLoad()
        courseTitleTextField.delegate = self
    }
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        getProfessorNames()
    }
    
    override func updateUI(list: [String]) {
        super.updateUI(list)
        picker?.selectRow(0, inComponent: 0, animated: false)
        selectedValue = nil
        picker?.hidden = false
        spinner?.stopAnimating()
    }
    
    override func shouldPerformSegueWithIdentifier(identifier: String, sender: AnyObject?) -> Bool {
        switch identifier {
        case "didAddClass":
            if selectedValue == nil || courseTitleTextField.text == nil { return false }
        default: break
        }
        return true
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if let id = segue.identifier {
            switch id {
            case "didAddClass":
                courseTitleTextField.resignFirstResponder()
                let dvc = segue.destinationViewController as! SetupClassesViewController
                if let schoolID = school.identifier {
                    if let courseTitle = courseTitleTextField.text {
                        for professor in self.professors {
                            let professorName: String =  (professor.firstName != "" && professor.lastName != "") ? (professor.lastName + ", " + professor.firstName) : ""
                            if professorName == self.selectedValue! {
                                dvc.classToAdd = Class(classTitle: courseTitle, schoolID: schoolID, professorID: professor.identifier)
                            }
                        }
                    }
                }
            default: break
            }
        }
    }
    
    // MARK: Properties
    var school: School = School(schoolName: nil, stateAbreviation: nil) // Passed from previous view controller
    var professors: [Professor] = []
    
    @IBOutlet weak var courseTitleTextField: UITextField!
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBAction func dismiss(sender: UITapGestureRecognizer) {
        courseTitleTextField.resignFirstResponder()
    }
    
    // MARK: Functions
    private func suspendUI() {
        picker?.hidden = true
        spinner?.startAnimating()
    }
    
    private func getProfessorNames() {
        suspendUI()
        // Query database off the main thread
        let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
        dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
            // Get professor names from database
            var professorNames: [String] = []
            var error: NSError?
            if let schoolID = self.school.identifier {
                let table = Table(type: 6)
                let professorSearch = table.getObjectsWithKeyValue(["school": schoolID], limit: 0, error: &error) as! [Professor]
                for professor in professorSearch {
                    self.professors.append(professor)
                    let professorName: String =  (professor.firstName != "" && professor.lastName != "") ? (professor.lastName + ", " + professor.firstName) : ""
                    professorNames.append(professorName)
                }
            }
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                // Load UI with database results
                if professorNames.count == 0 {
                    self.blank = "No Professors"
                    self.updateUI([])
                } else {
                    self.blank = "-"
                    self.updateUI(professorNames)
                }
            }
        }
    }
    
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
                            self.blank = "Error"
                            self.updateUI([])
                        }
                    )
                    self.presentViewController(alert, animated: true, completion: nil)
                default: break
                }
            }
        }
    }
}

extension AddClassViewController { // Functionality to allow users to add a professor
    
    @IBAction func addProfessor(sender: UIButton) {
        if pickerList.count > 0 && pickerList[0] != "Error" {
            let addProfessor = UIAlertController(
                title: "Add Professor",
                message: "Enter the name of the professor.",
                preferredStyle:  UIAlertControllerStyle.Alert
            )
            addProfessor.addAction(UIAlertAction(
                title: "OK",
                style: .Default)
                { (action: UIAlertAction) -> Void in
                    // Get school name
                    if let textFieldPointer = addProfessor.textFields?.first {
                        let textField = textFieldPointer as UITextField
                        if let professorFirstName = textField.text {
                            if let textFieldPointer = addProfessor.textFields?.last {
                                let textField = textFieldPointer as UITextField
                                if let professorLastName = textField.text {
                                    self.createProfessor(professorFirstName, lastName: professorLastName)
                                }
                            }
                        }
                    }
                }
            )
            addProfessor.addAction(UIAlertAction(
                title: "Cancel",
                style: .Cancel)
                { (action: UIAlertAction) -> Void in
                    // Do nothing
                }
            )
            addProfessor.addTextFieldWithConfigurationHandler { (textField) in
                textField.placeholder = "First Name"
                textField.text = ""
                textField.keyboardType = .ASCIICapable
                textField.autocapitalizationType = .Words
            }
            addProfessor.addTextFieldWithConfigurationHandler { (textField) in
                textField.placeholder = "Last Name"
                textField.text = ""
                textField.keyboardType = .ASCIICapable
                textField.autocapitalizationType = .Words
            }
            presentViewController(addProfessor, animated: true, completion: nil)
        }
    }
    
    private func createProfessor(firstName: String, lastName: String) {
        if self.school.identifier != nil {
            if firstName != "" && lastName != "" {
                // Remove all other characters
                var components = firstName.componentsSeparatedByCharactersInSet(NSCharacterSet.whitespaceAndNewlineCharacterSet())
                let firstName = components.filter { !$0.isEmpty }.joinWithSeparator("")
                components = lastName.componentsSeparatedByCharactersInSet(NSCharacterSet.whitespaceAndNewlineCharacterSet())
                let lastName = components.filter { !$0.isEmpty }.joinWithSeparator("")
                
                //Check for duplicate in list
                var duplicate: Bool = false
                for listItem in pickerList {
                    let name = lastName + ", " + firstName
                    if name.lowercaseString == listItem.lowercaseString {
                        duplicate = true
                    }
                }
                
                //Add professor to database
                if !duplicate {
                    let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
                    dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
                        let newProfessor = Professor(firstNameText: firstName, lastNameText: lastName, schoolID: self.school.identifier)
                        var error: NSError?
                        newProfessor.addToDatabase(&error)
                        self.errorHandling(error)
                        dispatch_async(dispatch_get_main_queue()) { () -> Void in
                            self.getProfessorNames()
                        }
                    }
                }
            }
        }
    }
}
