//
//  Professor.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/7/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class Professor: NSObject, NSCoding {
    
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
    
    required convenience init(coder aDecoder: NSCoder) {
        self.init(firstNameText: nil, lastNameText: nil, schoolID: nil)
        firstName = aDecoder.decodeObjectForKey("firstName") as! String
        lastName = aDecoder.decodeObjectForKey("lastName") as! String
        school = aDecoder.decodeObjectForKey("school") as! String
        identifier = aDecoder.decodeObjectForKey("id") as? String
    }
    
    // MARK: Functions
    func addToDatabase(error: NSErrorPointer) {
        let table = Table(type: 6)
        table.createObjectWithStringKeys(["firstName": firstName, "lastName": lastName, "school": school], error: error)
    }
    
    func getName() -> String {
        return (firstName != "" && lastName != "") ? (lastName + ", " + firstName) : ""
    }
    
    func encodeWithCoder(aCoder: NSCoder) {
        aCoder.encodeObject(firstName, forKey: "firstName")
        aCoder.encodeObject(lastName, forKey: "lastName")
        aCoder.encodeObject(school, forKey: "school")
        aCoder.encodeObject(identifier, forKey: "id")
    }
}