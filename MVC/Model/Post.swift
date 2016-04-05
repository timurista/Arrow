//
//  Post.swift
//  Arrow
//
//  Created by Trevor Sharp on 4/5/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class Post {
    
    // MARK: Properties
    var user: User // database: "_owner"
    var text: String // database: "text"
    var date: Double // database: "_created"
    var numberOfLikes: Int
    var numberOfComments: Int
    var liked: Bool
    var identifier: String? //database: "_id", created by database
    
    // MARK: Initializers
    init() {
        var error: NSError?
        user = User(userIdentifier: nil, error: &error)
        text = ""
        date = 0
        numberOfComments = 0
        numberOfLikes = 0
        liked = false
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
        if identifier != nil {
            var table = Table(type: 4)
            numberOfComments = table.getObjectsWithKeyValue(["post": identifier!], limit: 0, error: &error).count
            table = Table(type: 5)
            let results = table.getObjectsWithKeyValue(["post": identifier!], limit: 0, error: &error) as! [Like]
            numberOfLikes = results.count
            liked = false
            if let currentUserID  = CurrentUser().userID {
                for like in results {
                    if like.userID == currentUserID {
                        liked = true
                    }
                }
            }
        } else {
            numberOfLikes = 0
            numberOfComments = 0
            liked = false
        }
    }
    
    // MARK: Functions
    func addToDatabase(currentClass: Class, error: NSErrorPointer) {
        let table = Table(type: 3)
        if let classID = currentClass.identifier {
            table.createObjectWithStringKeys(["text": text, "class": classID], error: error)
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
    
    func like(error: NSErrorPointer) {
        if let postID = identifier {
            if let userID = CurrentUser().userID {
                let like = Like(user: userID, post: postID)
                like.addToDatabase(error)
            }
        }
    }
    
    func unlike(error: NSErrorPointer) {
        let table = Table(type: 5)
        if let postID = identifier {
            if let userID = CurrentUser().userID {
                table.deleteObjectWithStringKeys(["post": postID, "_owner": userID], error: error)
            }
        }
    }
}