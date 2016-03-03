//
//  GenericPickerViewController.swift
//  Arrow
//
//  Created by Trevor Sharp on 2/29/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import UIKit

class GenericPickerViewController: UIViewController, UIPickerViewDataSource, UIPickerViewDelegate {
        
        // MARK: Overrided Methods
        override func viewDidLoad() {
            super.viewDidLoad()
            picker.delegate = self
            picker.dataSource = self
        }
        
        // MARK: Properties
        var selectedValue: String?
        var pickerList: [String] = []
        var blank: String = "-"
        
        @IBOutlet weak var picker: UIPickerView!
        
        // MARK: Functions
    
        func updateUI(var list: [String]) {
            list.append(blank)
            pickerList = list
            pickerList.sortInPlace()
            picker?.reloadAllComponents()
        }
        
        // MARK: Picker View Methods
        func numberOfComponentsInPickerView(pickerView: UIPickerView) -> Int {
            return 1
        }
        func pickerView(pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
            return pickerList.count
        }
        func pickerView(pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
            return pickerList[row]
        }
        func pickerView(pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
            if pickerList[row] == blank {
                selectedValue = nil
            } else {
                selectedValue = pickerList[row]
            }
    }
}
