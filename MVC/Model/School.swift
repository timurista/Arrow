//
//  School.swift
//  Arrow
//
//  Created by Trevor Sharp on 2/17/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class School {
    
    // MARK: Properties
    var name: String // database: "name"
    var state: String // database: "state"
    var identifier: String? //database: "_id", created by database
    
    // MARK: Initializers
    init(schoolName: String?, stateAbreviation: String?) {
        name = (schoolName != nil) ? schoolName! : ""
        state = (stateAbreviation != nil) ? stateAbreviation! : ""
    }
    
    init(kiiObject: KiiObject) {
        name = kiiObject.getObjectForKey("name") as! String
        state = kiiObject.getObjectForKey("state") as! String
        identifier = kiiObject.getObjectForKey("_id") as? String
    }
    
    // MARK: Functions
    func addToDatabase(error: NSErrorPointer) {
        let table = Table(type: 1)
        table.createObjectWithStringKeys(["name": name, "state": state], error: error)
    }
}