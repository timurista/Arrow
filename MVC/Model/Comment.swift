//
//  Comment.swift
//  Arrow
//
//  Created by Trevor Sharp on 4/7/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class Comment {
    
    // MARK: Properties
    var user: User // database: "_owner"
    var text: String // database: "text"
    var date: Double // database: "_created"
    var identifier: String? //database: "_id", created by database
    
    // MARK: Initializers
    init() {
        var error: NSError?
        user = User(userIdentifier: nil, error: &error)
        text = ""
        date = 0
    }
    
    init(kiiObject: KiiObject) {
        text = kiiObject.getObjectForKey("text") as! String
        date = (kiiObject.getObjectForKey("_created") as! Double) / 1000
        identifier = kiiObject.getObjectForKey("_id") as? String
        var error: NSError?
        if let userID = kiiObject.getObjectForKey("_owner") as? String {
            let table = Table(type: 0)
            let results = table.getObjectsWithKeyValue(["_id": userID], limit: 1, error: &error)
            if results.count != 0 {
                let userID = (results[0] as! KiiObject).getObjectForKey("_id") as! String
                user = User(userIdentifier: userID, error: &error)
            } else {
                user = User(userIdentifier: nil, error: &error)
            }
        } else {
            user = User(userIdentifier: nil, error: &error)
        }
    }
    
    // MARK: Functions
    func addToDatabase(currentPost: Post, error: NSErrorPointer) {
        let table = Table(type: 4)
        if let postID = currentPost.identifier {
            table.createObjectWithStringKeys(["text": text, "post": postID], error: error)
        }
    }
    
    func getDate() -> String {
        let dateFormatter = NSDateFormatter()
        dateFormatter.timeStyle = NSDateFormatterStyle.NoStyle
        dateFormatter.dateStyle = NSDateFormatterStyle.MediumStyle
        dateFormatter.timeZone = NSTimeZone()
        let returnDate = dateFormatter.stringFromDate(NSDate(timeIntervalSince1970: date))
        return returnDate.substringToIndex(returnDate.startIndex.advancedBy((returnDate.characters.count)-6))
    }
}