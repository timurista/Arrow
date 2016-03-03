//
//  KiiUserFields.h
//  KiiSDK-Private
//
//  Copyright (c) 2014 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface KiiUserFields : NSObject

/** Display name for this user. Cannot be used for logging a user in; is non-unique. */
@property (nonatomic, strong) NSString *displayName;

/** The country code associated with this user */
@property (nonatomic, strong) NSString *country;

/** Sets a key/value pair to a KiiUser

 If the key already exists, its value will be written over. If the
 object is of invalid type, it will return false. Accepted types are
 any JSON-encodable objects.

 @param object The value to be set. Object must be of a JSON-encodable
 type (Ex: NSDictionary, NSArray, NSString, NSNumber, etc)
 @param key The key to set. The key must not be a reserved key
 (loginName, emailAddress, phoneNumber, displayName, country, userID)
 or begin with an underscore (_).
 @return True if the object was set, false otherwise.
 @exception NSInvalidArgumentException when key is nil/empty; equal to reserved key; and/or object is nil.
 */
- (BOOL)setObject:(id)object forKey:(NSString*)key;

/** Checks to see if an object exists for a given key

 @param key The key to check for existence
 @return True if the object exists, false otherwise.
 @exception NSInvalidArgumentException when key is nil/empty.
 */
- (BOOL)hasObject:(NSString*)key;

/** Removes a specific key/value pair from the object

 If the key exists, the key/value will be removed from the
 object.
 @param key The key of the key/value pair that will be removed
 @exception NSInvalidArgumentException when key is nil/empty.
 */
- (void)removeObjectForKey:(NSString*)key;


/** Gets the value associated with the given key

 @param key The key to retrieve
 @return An object if the key exists, nil otherwise
 @exception NSInvalidArgumentException when key is nil/empty.
 */
- (id)getObjectForKey:(NSString*)key;

/** Remove a pair of key/value from UserFields.

 This pair is also removed from server when <KiiUser
 updateIdentitySynchronous:userFields:error:> or <KiiUser
 updateIdentity:userFields:blocking> succeeded.

 Note: <KiiUserFields> has a similar method which is <KiiUserFields
 removeObjectForKey:>. If you remove fields by <KiiUserFields
 removeFromServerForKey:>, pairs are not removed from server. So you
 can use <KiiUserFields removeObjectForKey:> if you want to cancel to
 update fields which are set in {@link UserFields}.
 @param key to remove.
 @exception NSInvalidArgumentException when key is nil/empty or equal to reserved key.
     */
- (void)removeFromServerForKey:(NSString *)key;

@end
