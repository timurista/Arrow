//
//  SetupStateViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 2/18/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class SetupStateViewController: GenericPickerViewController {

    // MARK: Overrided Methods
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        getStateNames()
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if let id = segue.identifier {
            switch id {
            case "next":
                let dvc = segue.destinationViewController as! SetupSchoolViewController
                let state = States()
                dvc.stateAbreviation = state.getAbreviation(selectedValue!)
            case "logOut":
                CurrentUser().logOut()
            default: break
            }
        }
    }
    
    // MARK: Properties
    @IBAction func continueButton() {
        if selectedValue != nil {
            performSegueWithIdentifier("next", sender: self)
        }
    }
    
    // MARK: Functions
    private func getStateNames(){
        let state = States()
        let stateNames = state.stateNames
        updateUI(stateNames)
    }
}
