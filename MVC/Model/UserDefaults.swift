//
//  UserDefaults.swift
//  Arrow
//
//  Created by Trevor Sharp on 4/7/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class UserDefaults {
    
    let keyForUserID: String = "userID"
    let keyForUserFirstName: String = "userFirstName"
    let keyForUserLastName: String = "userLastName"
    let keyForUserSchool: String = "userSchoolID"
    let keyForMyClasses: String = "myClassesArray"
    let keyForPosts: String = "posts"
    var keys: [String] = []
    
    init() {
        keys = [keyForUserID, keyForUserFirstName, keyForUserLastName, keyForUserSchool, keyForMyClasses, keyForPosts]
    }
}