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
    var school: School // database: "school"
    var identifier: String? //database: "_id", created by database
    
    // MARK: Initializers
    init(firstNameInput: String?, lastNameInput: String?, schoolObject: School?) {
        firstName = (firstNameInput != nil) ? firstNameInput! : ""
        lastName = (lastNameInput != nil) ? lastNameInput! : ""
        school = (schoolObject != nil) ? schoolObject! : School(schoolName: nil, stateAbreviation: nil)
    }
    
    init(kiiObject: KiiObject) {
        firstName = kiiObject.getObjectForKey("firstName") as! String
        lastName = kiiObject.getObjectForKey("lastName") as! String
        let schoolIdentifier = kiiObject.getObjectForKey("school") as! String
        let table = Table(type: 1)
        let schoolArray = table.getObjectsWithKeyValue(["_id": schoolIdentifier], limit: 1)
        if schoolArray.count != 0 {
            school = schoolArray[0] as! School
        } else {
            school = School(schoolName: nil, stateAbreviation: nil)
        }
        identifier = kiiObject.getObjectForKey("_id") as? String
    }
    
    // MARK: Functions
    func addToDatabase() {
        let table = Table(type: 6)
        let schoolIdentifier = school.identifier
        if schoolIdentifier != nil {
            table.createObjectWithStringKeys(["firstName": firstName, "lastName": lastName, "school": schoolIdentifier!])
        }
    }
}