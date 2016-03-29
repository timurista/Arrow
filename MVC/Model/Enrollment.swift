//
//  Enrollment.swift
//  Arrow
//
//  Created by Trevor Sharp on 3/29/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class Enrollment {
    
    // MARK: Properties
    var classID: String
    var userID: String
    
    // MARK: Initializers
    init(kiiObject: KiiObject) {
        userID = kiiObject.getObjectForKey("user") as! String
        classID = kiiObject.getObjectForKey("class") as! String
    }
    
    // MARK: Functions
    func getClass(error: NSErrorPointer) -> Class {
        let table = Table(type: 2)
        let kiiObject = KiiObject(URI: table.getURI(classID))
        kiiObject.refreshSynchronous(error)
        if error.memory == nil {
            return Class(kiiObject: kiiObject)
        } else {
            return Class(classTitle: nil, schoolID: nil, professorID: nil)
        }
    }
}