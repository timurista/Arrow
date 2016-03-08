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
    
    // MARK: Functions
    func setUpUserObject(error: NSErrorPointer) {
        if userID != nil {
            let objectID = userID!
            let table = Table(type: 0)
            let userSearch = table.getObjectsWithKeyValue(["_id": objectID], limit: 1, error: error)
            if userSearch.count == 0 && error == nil {
                table.createObjectWithId(objectID, error: error)
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
    
    func refresh(error: NSErrorPointer) {
        KiiUser.currentUser()?.refreshSynchronous(error)
    }
    
    func logOut() {
        KiiUser.logOut()
    }
}