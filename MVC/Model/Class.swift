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
    var school: String // database: "school"
    var professor: String // database "professor"
    var numberOfMembers: Int
    var identifier: String? //database: "_id", created by database
    
    // MARK: Initializers
    init(classTitle: String?, schoolID: String?, professorID: String?) {
        title = (classTitle != nil) ? classTitle! : ""
        school = (schoolID != nil) ? schoolID! : ""
        professor = (professorID != nil) ? professorID! : ""
        numberOfMembers = 0
    }
    
    init(kiiObject: KiiObject) {
        title = kiiObject.getObjectForKey("title") as! String
        school = kiiObject.getObjectForKey("school") as! String
        professor = kiiObject.getObjectForKey("professor") as! String
        identifier = kiiObject.getObjectForKey("_id") as? String
        if identifier != nil {
            var error: NSError?
            let table = Table(type: 8)
            let results = table.getObjectsWithKeyValue(["class": identifier!], limit: 0, error: &error)
            numberOfMembers = results.count
        } else {
            numberOfMembers = 0
        }
    }
    
    // MARK: Functions
    func addToDatabase(error: NSErrorPointer) {
        let table = Table(type: 2)
        table.createObjectWithStringKeys(["title": title, "school": school, "professor": professor], error: error)
    }
    
    func getProfessor(error: NSErrorPointer) -> Professor {
        let table = Table(type: 6)
        let kiiObject = KiiObject(URI: table.getURI(professor))
        kiiObject.refreshSynchronous(error)
        if error.memory == nil {
            return Professor(kiiObject: kiiObject)
        } else {
            return Professor(firstNameText: nil, lastNameText: nil, schoolID: nil)
        }
    }
}