//
//  AddClassViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/10/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class AddClassViewController: GenericPickerViewController, UITextFieldDelegate {

    // MARK: Overridden Methods
    override func viewDidLoad() {
        super.viewDidLoad()
        courseTitleTextField.delegate = self
    }
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        getProfessorNames()
        bigSpinner?.hidden = true
    }
    
    override func updateUI(list: [String]) {
        super.updateUI(list)
        picker?.selectRow(0, inComponent: 0, animated: false)
        selectedValue = nil
        picker?.hidden = false
        spinner?.stopAnimating()
    }
    
    // MARK: Properties
    var professors: [Professor] = []
    private let defaults = NSUserDefaults.standardUserDefaults()
    private let userDefaultsKey = "userSchoolID"
    private var schoolID: String { get { if let id = defaults.stringForKey(userDefaultsKey) { return id } else { return "" } } }
    private var error: NSError? { didSet{ self.errorHandling(error) } }
    
    @IBOutlet weak var courseTitleTextField: UITextField!
    @IBOutlet weak var bigSpinner: UIActivityIndicatorView!
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    @IBOutlet weak var textLabelOne: UILabel!
    @IBOutlet weak var textLabelTwo: UILabel!
    @IBOutlet weak var button: UIButton!
    @IBAction func dismiss(sender: UITapGestureRecognizer) { courseTitleTextField.resignFirstResponder() }
    @IBAction func save(sender: UIBarButtonItem) {
        if let courseTitle = courseTitleTextField.text {
            if selectedValue != nil && courseTitle != "" {
                savingUI()
                let qos = Int(QOS_CLASS_USER_INITIATED.rawValue)
                dispatch_async(dispatch_get_global_queue(qos, 0)){ () -> Void in
                    for professor in self.professors {
                        let professorName: String =  (professor.firstName != "" && professor.lastName != "") ? (professor.lastName + ", " + professor.firstName) : ""
                        if professorName == self.selectedValue! {
                            let classObject = Class(classTitle: courseTitle, schoolID: self.schoolID, professorID: professor.identifier)
                            classObject.addToDatabase(&self.error)
                        }
                    }
                    dispatch_async(dispatch_get_main_queue()) { () -> Void in
                        self.performSegueWithIdentifier("didAddClass", sender: self)
                    }
                }
            }
        }
    }
    
    // MARK: Functions
    private func savingUI() {
        picker?.hidden = true
        button?.hidden = true
        textLabelOne?.hidden = true
        textLabelTwo?.hidden = true
        courseTitleTextField?.resignFirstResponder()
        courseTitleTextField?.hidden = true
        bigSpinner?.startAnimating()
        bigSpinner?.hidden = false
    }
    
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
            if self.schoolID != "" {
                let table = Table(type: 6)
                let professorSearch = table.getObjectsWithKeyValue(["school": self.schoolID], limit: 0, error: &self.error) as! [Professor]
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
        if self.schoolID != "" {
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
                        let newProfessor = Professor(firstNameText: firstName, lastNameText: lastName, schoolID: self.schoolID)
                        newProfessor.addToDatabase(&self.error)
                        dispatch_async(dispatch_get_main_queue()) { () -> Void in
                            self.getProfessorNames()
                        }
                    }
                }
            }
        }
    }
}