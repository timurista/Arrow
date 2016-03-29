//
//  Professor.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/7/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class Professor {
    
    // MARK: Properties
    var firstName: String // database: "firstName"
    var lastName: String // database: "lastName"
    var school: String // database: "school"
    var identifier: String? //database: "_id", created by database
    
    // MARK: Initializers
    init(firstNameText: String?, lastNameText: String?, schoolID: String?) {
        firstName = (firstNameText != nil) ? firstNameText! : ""
        lastName = (lastNameText != nil) ? lastNameText! : ""
        school = (schoolID != nil) ? schoolID! : ""
    }
    
    init(kiiObject: KiiObject) {
        firstName = kiiObject.getObjectForKey("firstName") as! String
        lastName = kiiObject.getObjectForKey("lastName") as! String
        school = kiiObject.getObjectForKey("school") as! String
        identifier = kiiObject.getObjectForKey("_id") as? String
    }
    
    // MARK: Functions
    func addToDatabase(error: NSErrorPointer) {
        let table = Table(type: 6)
        table.createObjectWithStringKeys(["firstName": firstName, "lastName": lastName, "school": school], error: error)
    }
    
    func getName() -> String {
        return (firstName != "" && lastName != "") ? (lastName + ", " + firstName) : ""
    }
}