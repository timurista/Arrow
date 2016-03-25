//
//  Table.swift
//  Arrow
//
//  Created by Trevor Sharp on 2/17/16.
//  Copyright © 2016 Trevor Sharp. All rights reserved.
//

import Foundation

class Table {
    
    // MARK: Properties
    var table: KiiBucket {
        get {
            return Kii.bucketWithName(tableNames[tableType])
        }
    }
    let tableType: Int // Refers to the name in tableNames
    var numberOfObjects: Int {
        get {
            var error : NSError?
            let count = table.countSynchronous(&error)
            // Error handling
            if (error != nil) { return 0 }
            return Int(count)
        }
    }
    let tableNames = [
        0: "_User",
        1: "_School",
        2: "_Class",
        3: "_Post",
        4: "_Comment",
        5: "_Like",
        6: "_Professor",
        7: "_ProfilePicture",
        8: "_Enrollment"
    ]
    let maxQuerySize = 9999
    
    // MARK: Initializers
    init(type: Int){
        tableType = type
    }
    
    // MARK: Functions
    func getAllObjects(error: NSErrorPointer) -> NSArray { // Retrieves all objects from a table
        // Build "all" query
        let allQuery = KiiQuery(clause: nil)
        
        // Create a placeholder for any paginated queries
        var nextQuery : KiiQuery?
        
        // Create an array to store all the results in
        var allResults = [KiiObject]()
        
        // Get an array of KiiObjects by querying the bucket
        let results = table.executeQuerySynchronous(allQuery, withError: error, andNext: &nextQuery)
        if error.memory != nil { return [] }
        
        // Add results to array
        allResults.appendContentsOf(results as! [KiiObject])
        
        // Convert from KiiObject to specific object type
        switch tableType {
        case 0: break // Leave as KiiObject
        case 1:
            var returnResults: [School] = []
            for object in allResults {
                let school = School(kiiObject: object)
                returnResults.appendContentsOf([school])
            }
            return returnResults
        case 2:
            var returnResults: [Class] = []
            for object in allResults {
                let classObject = Class(kiiObject: object)
                returnResults.appendContentsOf([classObject])
            }
            return returnResults
        case 6:
            var returnResults: [Professor] = []
            for object in allResults {
                let professor = Professor(kiiObject: object)
                returnResults.appendContentsOf([professor])
            }
            return returnResults
        default: break
        }
            
        return allResults
    }
    
    func getObjectsWithKeyValue(keyValuePairs: [String:String], limit: Int, error: NSErrorPointer) -> NSArray { // Retreives objects from a table that match the given key value pairs
        // For "unlimited" query, limit = 0 -> limit of maxQuerySize
        var queryLimit = limit
        if queryLimit == 0 { queryLimit = maxQuerySize }
        
        // Set up the clauses
        var clauses: [KiiClause] = []
        for (key, value) in keyValuePairs {
            let clause = KiiClause.equals(key, value: value)
            clauses.append(clause)
        }
        
        // Combine the clauses
        let totalClause = KiiClause.andClauses(clauses)
        
        // Build the query
        let query = KiiQuery(clause: totalClause)
        query.limit = Int32(queryLimit)
        
        // if all the results can't be returned in one pass
        // using the given criteria. This will be pre-configured
        // for you. A non-nil value means there is more data to retrieve
        var allResults = [AnyObject]()
        
        // Get an array of KiiObjects by querying the bucket
        var nextQuery : KiiQuery?
        var results = table.executeQuerySynchronous(query, withError: error, andNext: &nextQuery)
        if error.memory != nil { return [] }
        
        // Add all the results from this query to the total results
        allResults.appendContentsOf(results)
        
        // If there is more data to retreive
        if nextQuery != nil {
            var nextQuery2 : KiiQuery?
            
            // Make the next query, storing the results
            results = table.executeQuerySynchronous(nextQuery, withError: error, andNext: &nextQuery2)
            if error.memory != nil { return [] }
            
            // add these results to the total array
            allResults.appendContentsOf(results)
        }
        
        // Convert from AnyObject to Kii Object
        var resultsAsKiiObjects: [KiiObject] = []
        for object in allResults {
            resultsAsKiiObjects.append(object as! KiiObject)
        }
        
        // Convert from KiiObject to specific object type
        switch tableType {
        case 0: break // Leave as KiiObject
        case 1:
            var returnResults: [School] = []
            for object in resultsAsKiiObjects {
                let school = School(kiiObject: object)
                returnResults.appendContentsOf([school])
            }
            return returnResults
        case 2:
            var returnResults: [Class] = []
            for object in resultsAsKiiObjects {
                let classObject = Class(kiiObject: object)
                returnResults.appendContentsOf([classObject])
            }
            return returnResults
        case 6:
            var returnResults: [Professor] = []
            for object in resultsAsKiiObjects {
                let professor = Professor(kiiObject: object)
                returnResults.appendContentsOf([professor])
            }
            return returnResults
        default: break
        }
        
        return resultsAsKiiObjects
    }
    
    func createObjectWithStringKeys(keyValuePairs: [String:String], error: NSErrorPointer) -> Bool { // Adds an object to the database using key value pairs of type string
        // Create an object with key/value pairs
        let object = table.createObject()
        for (key, value) in keyValuePairs {
            object.setObject(value, forKey: key)
        }
        
        // Save the object
        object.saveSynchronous(error)
        
        return true // if no error
    }
    
    func createObjectWithId(id: String, error: NSErrorPointer) { // Adds an object to the database using id
        // Create an object with key/value pairs
        let object = table.createObjectWithID(id)
        
        // Save the object
        object.saveAllFieldsSynchronous(true, withError: error)
    }
    
    func appendObjectWithStringKeys(keyValuePairs: [String:String], id: String, error: NSErrorPointer) { // Appends an object in the database with the key value pairs
        let URI = getURI(id)
        let object = KiiObject(URI: URI)
        
        for (key, value) in keyValuePairs {
            object.setObject(value, forKey: key)
        }
        
        // This will append the local key/value pairs with the data that already exists on the server
        object.saveSynchronous(error)
    }
    
    func getURI(id: String) -> String {
        let URI = "kiicloud://buckets/\((tableNames[tableType])!)/objects/\(id)"
        return URI
    }
}