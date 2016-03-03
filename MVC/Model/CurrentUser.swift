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
        super.init(userIdentifier: KiiUser.currentUser()?.userID)
    }
    
    // MARK: Functions
    func setUpUserObject() {
        if userID != nil {
            let objectID = userID!
            let table = Table(type: 0)
            let userSearch = table.getObjectsWithKeyValue(["_id": objectID], limit: 1)
            if userSearch.count == 0 {
                table.createObjectWithId(objectID)
            }
        }
    }
    
    func setSchool(newSchool: School) { // Must pass a School object that is in the database
        if userID != nil {
            // Get school ID
            let schoolID = newSchool.identifier!
            
            // Append user to add school
            let table = Table(type: 0)
            table.appendObjectWithStringKeys(["school": schoolID], id: userID!)
        }
    }
    
    func refresh() {
        var error: NSError?
        KiiUser.currentUser()?.refreshSynchronous(&error)
        // Error handling
            if error != nil {
                print("\(error)")
                return
            }
    }
    
    func logOut() {
        KiiUser.logOut()
    }
}