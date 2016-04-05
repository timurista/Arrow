//
//  Like.swift
//  Arrow
//
//  Created by Trevor Sharp on 4/5/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class Like {
    
    // MARK: Properties
    var postID: String // database: "post"
    var userID: String // database: "_owner"
    
    // MARK: Initializers
    init(user: String, post: String) {
        userID = user
        postID = post
    }
    
    init(kiiObject: KiiObject) {
        userID = kiiObject.getObjectForKey("_owner") as! String
        postID = kiiObject.getObjectForKey("post") as! String
    }
    
    // MARK: Functions
    func addToDatabase(error: NSErrorPointer) {
        let table = Table(type: 5)
        table.createObjectWithStringKeys(["post": postID], error: error)
    }
}