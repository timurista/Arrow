//
//  KiiBaseObject.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 1/6/15.
//  Copyright (c) 2015 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
/** Base class for Kii Object.
 */
@interface KiiBaseObject : NSObject

/** Sets a key/value pair to a KiiObject
 
 If the key already exists, its value will be written over. If the object is of invalid type, it will return false and an NSError will be thrown (quietly). Accepted types are any JSON-encodable objects.
 
 ***NOTE: Before involving floating point value, please consider using integer instead. For example, use percentage, permil, ppm, etc.***
 The reason is:
 
 - Will dramatically improve the performance of bucket query.
 - Bucket query does not support the mixed result of integer and floating point.
 ex.) If you use same key for integer and floating point and inquire object with the
 integer value, objects which has floating point value with the key would not be evaluated
 in the query. (and vice versa)
 
 @param object The value to be set. Object must be of a JSON-encodable type (NString, NSNumber type values and collections (NSArray/NSDictionary) contains NSString/NSNumber)
 @param key The key to set. The key must not be a system key (created, metadata, modified, type, uuid) or begin with an underscore (_)
 @return True if the object was set, false otherwise.
 */
- (BOOL) setObject:(id)object forKey:(NSString*)key;

/** Checks to see if an object exists for a given key
 
 @param key The key to check for existence
 @return True if the object exists, false otherwise.
 */
- (BOOL) hasObject:(NSString*)key;


/** Removes a specific key/value pair from the object
 If the key exists, the key/value will be removed from the object. Please note that the object must be saved before the changes propagate to the server.
 @param key The key of the key/value pair that will be removed
 */
- (void) removeObjectForKey:(NSString*)key;


/** Gets the value associated with the given key
 
 @param key The key to retrieve
 @return An object if the key exists, null otherwise
 */
- (id) getObjectForKey:(NSString*)key;

@end
