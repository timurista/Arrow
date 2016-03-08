//
//  User.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/1/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class User {
    
    // MARK: Properties
    var userID: String?
    var school: School?
    var firstName: String?
    var lastName: String?
    var profilePicture: String?
    
    // MARK: Initializers
    init (userIdentifier: String?, error: NSErrorPointer) {
        // Get user's KiiObject
        userID = userIdentifier
        let table = Table(type: 0)
        if userIdentifier != nil {
            // Search for user object in user table
            let userSearch = table.getObjectsWithKeyValue(["_id": userIdentifier!], limit: 1, error: error)
            if userSearch.count == 1 && error == nil {
                let user = userSearch[0]
                
                // Get user's school
                let schoolID = user.getObjectForKey("school") as? String
                let schoolTable = Table(type: 1)
                if schoolID != nil {
                    school = (schoolTable.getObjectsWithKeyValue(["_id": schoolID!], limit: 1, error: error))[0] as? School
                }
                
                // Get user's firstName, lastName and profilePicture
                firstName = user.getObjectForKey("firstName") as? String
                lastName = user.getObjectForKey("lastName") as? String
                profilePicture = user.getObjectForKey("profilePicture") as? String

            }
        }
    }
    
    // MARK: Function
    func getProfilePicture(){
        
    }
}
