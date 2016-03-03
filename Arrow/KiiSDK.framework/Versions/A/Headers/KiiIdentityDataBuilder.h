//
//  KiiIdentityDataBuilder.h
//  KiiSDK-Private
//
//  Copyright (c) 2014 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

@class KiiIdentityData;

/** Builder for KiiIdentityData. */
@interface KiiIdentityDataBuilder : NSObject

/** Phone number of the user.

 The user's phone number. must not be nil. Both of local and
 international phone number are available.
*/
@property (nonatomic, strong) NSString *phoneNumber;

/** Email address of the user.

 The user's email.
*/
@property (nonatomic, strong) NSString *email;

/** The user's user name.

 userName must be between 3 and 64 characters, which can include
 alphanumeric characters as well as underscores '_', dashes '-' and
 periodsx'.'
*/
@property (nonatomic, strong) NSString *userName;

/** Build KiiIdentityData instance.

 Build KiiIdentityData instance with username, email and phoneNumber
 if these were set.

 @return if build succeed KiiIdentityData instance, otherwise nil.
 Build will failed when all phoneNumber, email and userName is nil.
 When one or more of them are given, it should be valid format.
*/
- (KiiIdentityData *)build;

/** Build KiiIdentityData instance.

 Build KiiIdentityData instance with username, email and phoneNumber
 if these were set.

 @param error A NSError object, can be nil but not recommended.
 @return if build succeed KiiIdentityData instance, otherwise nil.
 Build will failed when all phoneNumber, email and userName is nil.
 When one or more of them are given, it should be valid format.
*/
- (KiiIdentityData *)buildWithError:(NSError **)error;

@end
