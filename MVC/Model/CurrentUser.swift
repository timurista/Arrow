//
//  CurrentUser.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/3/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class CurrentUser: User {
    
    // MARK: Initializers
    init() {
        var error: NSError?
        super.init(userIdentifier: KiiUser.currentUser()?.userID, error: &error)
    }
    
    required convenience init(coder aDecoder: NSCoder) {
        self.init()
    }
    
    // MARK: Functions
    func setUpUserObject(error: NSErrorPointer) {
        if userID != nil {
            let objectID = userID!
            let table = Table(type: 0)
            let userSearch = table.getObjectsWithKeyValue(["_id": objectID], limit: 1, error: error)
            if userSearch.count == 0 {
                table.createObjectWithId(objectID, error: error)
                refresh(error)
            }
        }
    }
    
    func setSchool(newSchool: School, error: NSErrorPointer) { // Must pass a School object that is in the database
        if userID != nil {
            // Get school ID
            let schoolID = newSchool.identifier!
            
            // Append user to add school
            let table = Table(type: 0)
             table.appendObjectWithStringKeys(["school": schoolID], id: userID!, error: error)
        }
    }
    
    func setName(firstName: String, lastName: String, error: NSErrorPointer) {
        if userID != nil {
            // Append user to add name
            let table = Table(type: 0)
            table.appendObjectWithStringKeys(["firstName": firstName, "lastName": lastName], id: userID!, error: error)
        }
    }
    
    func refresh(error: NSErrorPointer) {
        KiiUser.currentUser()?.refreshSynchronous(error)
    }
    
    func enrollInClass(classID: String, error: NSErrorPointer) {
        if userID != nil {
            let table = Table(type: 8)
            if table.getObjectsWithKeyValue(["user": userID!, "class": classID], limit: 1, error: error).count == 0 {
                table.createObjectWithStringKeys(["user": userID!, "class": classID], error: error)
            }
        }
    }
    
    func logOut() {
        let defaults = NSUserDefaults.standardUserDefaults()
        let userDefaultsKeys = UserDefaults().keys
        for key in userDefaultsKeys {
            defaults.removeObjectForKey(key)
        }
        KiiUser.logOut()
    }
}