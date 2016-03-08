//
//  Class.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/7/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class Class {
    
    // MARK: Properties
    var title: String // database: "title"
    var school: School // database: "school"
    var professor: Professor // database "professor"
    var identifier: String? //database: "_id", created by database
    
    // MARK: Initializers
    init(classTitle: String?, schoolObject: School?, professorObject: Professor?) {
        title = (classTitle != nil) ? classTitle! : ""
        school = (schoolObject != nil) ? schoolObject! : School(schoolName: nil, stateAbreviation: nil)
        professor = (professorObject != nil) ? professorObject! : Professor(firstNameInput: nil, lastNameInput: nil, schoolObject: nil)
    }
    
    init(kiiObject: KiiObject, error: NSErrorPointer) {
        // Title
        title = kiiObject.getObjectForKey("title") as! String
        
        // School
        let schoolIdentifier = kiiObject.getObjectForKey("school") as! String
        var table = Table(type: 1)
        let schoolArray = table.getObjectsWithKeyValue(["_id": schoolIdentifier], limit: 1, error: error)
        if schoolArray.count != 0 {
            school = schoolArray[0] as! School
        } else {
            school = School(schoolName: nil, stateAbreviation: nil)
        }
        
        // Professor
        let professorIdentifier = kiiObject.getObjectForKey("professor") as! String
        table = Table(type: 6)
        let professorArray = table.getObjectsWithKeyValue(["_id": professorIdentifier], limit: 1, error: error)
        if professorArray.count != 0 {
            professor = professorArray[0] as! Professor
        } else {
            professor = Professor(firstNameInput: nil, lastNameInput: nil, schoolObject: nil)
        }
        identifier = kiiObject.getObjectForKey("_id") as? String
    }
    
    // MARK: Functions
    func addToDatabase(error: NSErrorPointer) {
        let table = Table(type: 2)
        let schoolIdentifier = school.identifier
        let professorIdentifier = professor.identifier
        if schoolIdentifier != nil && professor.identifier != nil {
            table.createObjectWithStringKeys(["title": title, "school": schoolIdentifier!, "professor": professorIdentifier!], error: error)
        }
    }

}