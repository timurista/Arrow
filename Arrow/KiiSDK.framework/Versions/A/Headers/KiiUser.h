//
//  KiiUser.h
//  SampleApp
//
//  Created by Chris Beauchamp on 12/14/11.
//  Copyright (c) 2011 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiSocialConnect.h"
#import "KiiThingOwner.h"
#import "KiiListResult.h"
@class KiiUser;
@class KiiIdentityData;
@class KiiUserFields;
@class KiiPushSubscription;
typedef void (^KiiUserBlock)(KiiUser *user, NSError *error);
typedef void (^KiiUserArrayBlock)(KiiUser *user, NSArray *results, NSError *error);
typedef void (^KiiErrorBlock)(NSError *error);

typedef NS_ENUM(NSUInteger, KiiNotificationMethod) {
    KiiEMAIL,
    KiiSMS
};

/** Contains user profile/account information and methods
 
 The user class allows an application to generate a user, register them with the server and log them in during subsequent sessions. Since KiiUser is similar to <KiiObject>, the application can also set key/value pairs to this user.
 */

@class KiiBucket, KiiFileBucket, KiiTopic,KiiEncryptedBucket;
@interface KiiUser: NSObject<KiiThingOwner>

/** The unique ID of the KiiUser object, assigned by the server*/
@property (readonly) NSString *userID;

/** The unique id of the KiiUser object, assigned by the server 
 @deprecated Use <[KiiUser userID]> instead.
 */
@property (readonly) NSString *uuid __attribute__((deprecated("Use [KiiUser userID] instead.")));


/** Username to use for authentication or for display. Must be between 3 and 64 characters, which can include alphanumeric characters as well as underscores '_' and periods '.' */
@property (readonly) NSString *username;

/** True if the user is disabled, false otherwise.
 
 Call [KiiUser refreshWithBlock:]>, the <[KiiUser refresh:withCallback:]>
 or the <[KiiUser refreshSynchronous:]>
 prior calling this method to get correct status.
 */
@property (readonly) BOOL disabled;

/** Display name for this user. Cannot be used for logging a user in; is non-unique. 
 Must be between 1 and 50 characters
 */
@property (nonatomic, strong) NSString *displayName;

/** Email address to use for authentication or for display */
@property (readonly) NSString *email;

/** Phone number to use for authentication or for display */
@property (readonly) NSString *phoneNumber;

/** The country code associated with this user */
@property (nonatomic, strong) NSString *country;

/** Whether or not a user has validated their email address. This field is assigned by the server. */
@property (readonly) BOOL emailVerified;

/** Whether or not a user has validated their phone number. This field is assigned by the server. */
@property (readonly) BOOL phoneVerified;

/** The date the user was created on the server */
@property (strong, readonly) NSDate *created;

/** The date the user was last modified on the server */
@property (strong, readonly) NSDate *modified;

/** Get a specifically formatted string referencing the user. The user must exist in the cloud (have a valid UUID). */
@property (strong, readonly) NSString *objectURI;

/** The access token for the user - only available if the user is currently logged in. */
@property (strong, readonly) NSString *accessToken;

/** YES if this instance is pseudo user. otherwise NO.

 If this method is not called for current login user, calling the
 [KiiUser refreshWithBlock:]>, the <[KiiUser refresh:withCallback:]>
 or the <[KiiUser refreshSynchronous:]> method is necessary to get a
 correct value.
*/
@property (readonly) BOOL isPseudoUser;

/** Gets the linked social providers.
 <KiiUser refreshSynchronous:> should be called before calling this API, otherwise it will always return empty dictionary. The keys of this Dictionary is <KiiConnectorProvider> encapsulated in a NSNumber.
 
 Example:
    
        KiiSocialAccountInfo* facebookSocialInfo=[KiiUser currentUser].linkedSocialAccounts[@(kiiConnectorFacebook)];

 @return A Dictionary of <KiiSocialAccountInfo> that is informations from the providers linked with this user.
 */
@property(nonatomic,readonly) NSDictionary* linkedSocialAccounts;

/** Create a user object to prepare for registration with credentials pre-filled
 Creates an pre-filled user object for manipulation. This user will not be authenticated until one of the authentication methods are called on it. Custom fields can be added to it before it is registered or authenticated.
 @param userUsername The user's desired username. Must be between 3 and 64 characters, which can include alphanumeric characters as well as underscores '_', dashes '-' and periods '.'
 @param userPassword The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @return a working KiiUser object
 */
+ (KiiUser*) userWithUsername:(NSString*)userUsername
                  andPassword:(NSString*)userPassword;


/** Create a user object to prepare for registration with credentials pre-filled
 
 Creates an pre-filled user object for registration. This user will not be authenticated until the registration method is called on it. It can be treated as any other KiiUser before it is registered.
 @param phoneNumber The user's phone number
 @param userPassword The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @return a working KiiUser object
 */
+ (KiiUser*) userWithPhoneNumber:(NSString*)phoneNumber
                     andPassword:(NSString*)userPassword;


/** Create a user object to prepare for registration with credentials pre-filled
 
 Creates an pre-filled user object for registration. This user will not be authenticated until the registration method is called on it. It can be treated as any other KiiUser before it is registered.
 @param emailAddress The user's email address
 @param userPassword The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @return a working KiiUser object
 */
+ (KiiUser*) userWithEmailAddress:(NSString*)emailAddress
                      andPassword:(NSString*)userPassword;



/** Create a user object to prepare for registration with credentials pre-filled
 
 Creates an pre-filled user object for registration. This user will not be authenticated until the registration method is called on it. It can be treated as any other KiiUser before it is registered.
 @param username The user's desired username. Must be between 3 and 64 characters, which can include alphanumeric characters as well as underscores '_', dashes '-' and periods '.'
 @param phoneNumber The user's phone number
 @param userPassword The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @return a working KiiUser object
 */
+ (KiiUser*) userWithUsername:(NSString*)username
               andPhoneNumber:(NSString*)phoneNumber
                  andPassword:(NSString*)userPassword;


/** Create a user object to prepare for registration with credentials pre-filled
 
 Creates an pre-filled user object for registration. This user will not be authenticated until the registration method is called on it. It can be treated as any other KiiUser before it is registered.
 @param username The user's desired username. Must be between 3 and 64 characters, which can include alphanumeric characters as well as underscores '_', dashes '-' and periods '.'
 @param emailAddress The user's email address
 @param userPassword The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @return a working KiiUser object
 */
+ (KiiUser*) userWithUsername:(NSString*)username
              andEmailAddress:(NSString*)emailAddress
                  andPassword:(NSString*)userPassword;


/** Create a user object to prepare for registration with credentials pre-filled
 
 Creates an pre-filled user object for registration. This user will not be authenticated until the registration method is called on it. It can be treated as any other KiiUser before it is registered.
 @param emailAddress The user's email address
 @param phoneNumber The user's phone number
 @param userPassword The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @return a working KiiUser object
 */
+ (KiiUser*) userWithEmailAddress:(NSString*)emailAddress
                   andPhoneNumber:(NSString*)phoneNumber
                      andPassword:(NSString*)userPassword;


/** Create a user object to prepare for registration with credentials pre-filled
 
 Creates an pre-filled user object for registration. This user will not be authenticated until the registration method is called on it. It can be treated as any other KiiUser before it is registered.
 @param username The user's desired username. Must be between 3 and 64 characters, which can include alphanumeric characters as well as underscores '_', dashes '-' and periods '.'
 @param emailAddress The user's email address
 @param phoneNumber The user's phone number
 @param userPassword The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @return a working KiiUser object
 */
+ (KiiUser*) userWithUsername:(NSString*)username
              andEmailAddress:(NSString*)emailAddress
               andPhoneNumber:(NSString*)phoneNumber
                  andPassword:(NSString*)userPassword;


/** Create a KiiUser that references an existing user
 
 @param uri A user-specific URI
 @return a working KiiUser
 */
+ (KiiUser*) userWithURI:(NSString*)uri;

/** Instantiate KiiUser that refers to existing user which has specified ID.
 
 You have to specify the ID of existing KiiUser. Unlike KiiObject,
 you can not assign ID in the client side.
 @note This API does not access to the server. After instantiation, it should be 'refreshed' to fetch the properties from server.
 @param userID ID of the KiiUser to instantiate.
 @return instance of KiiUser.
 */
+ (KiiUser*) userWithID:(NSString*)userID;



/** Asynchronously authenticates a user with the server using local phone number, country code and password
 
 Authenticates a user with the server. This method is non blocking.
 If successful, the user is cached inside SDK as current user, and accessible via <[KiiUser currentUser]>.
 This user token is also cached and used by the SDK when the access token is required.
 Access token won't be expired unless you set it explicitly by <[Kii setAccessTokenExpiration:]>.
 User token can be get by <[KiiUser accessToken]>.
 From next time, it is possible to login with the access token until the token is expired.
 
 Example:
        [KiiUser authenticateWithLocalPhoneNumber:@"9812345"
            andPassword:@"mypassword"
            andCountryCode:@"US"
            andBlock:^(KiiUser *user, NSError *error) {
                if(error == nil) {
                NSLog(@"Authenticated user: %@", user);
                }
 
        }];
 
 @param phoneNumber local phone number, it must be numeric and at least 7 digit
 @param password The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @param countryCode 2 digits phone country code, it must be capital letters
 @param block The block to be called upon method completion. See example
 */
+ (void) authenticateWithLocalPhoneNumber:(NSString*)phoneNumber
                              andPassword:(NSString*)password
                           andCountryCode:(NSString*)countryCode
                                 andBlock:(KiiUserBlock)block;

/** Synchronously authenticates a user with the server using local phone number, country code and password
 
 Authenticates a user with the server. This method is blocking.
 @param phoneNumber local phone number, it must be numeric and at least 7 digit
 @param password The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @param countryCode 2 digits phone country code, it must be capital letters
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return The KiiUser object that was authenticated. nil if failed to authenticate
 */
+ (KiiUser*) authenticateWithLocalPhoneNumberSynchronous:(NSString*)phoneNumber
                                             andPassword:(NSString*)password
                                          andCountryCode:(NSString*)countryCode
                                                andError:(NSError**)error;
/** Asynchronously authenticates a user with the server using local phone number, country code and password
 
 Authenticates a user with the server. This method is non blocking.
 If successful, the user is cached inside SDK as current user, and accessible via <[KiiUser currentUser]>.
 This user token is also cached and used by the SDK when the access token is required.
 Access token won't be expired unless you set it explicitly by <[Kii setAccessTokenExpiration:]>.
 User token can be get by <[KiiUser accessToken]>.
 From next time, it is possible to login with the access token until the token is expired.
 @param phoneNumber local phone number, it must be numeric and at least 7 digit
 @param password The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @param countryCode 2 digits phone country code, it must be capital letters
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
    - (void) authenticationComplete:(KiiUser*)user withError:(NSError*)error {
 
        // the request was successful
        if(error == nil) {
        // do something with the user
        }
 
        else {
        // there was a problem
        }
    }
 */
+ (void) authenticateWithLocalPhoneNumber:(NSString*)phoneNumber
                              andPassword:(NSString*)password
                           andCountryCode:(NSString*)countryCode
                              andDelegate:(id)delegate
                              andCallback:(SEL)callback;


/** Asynchronously authenticates a user with the server
 
 Authenticates a user with the server. This method is non-blocking.
 If successful, the user is cached inside SDK as current user, and accessible via <[KiiUser currentUser]>.
 This user token is also cached and used by the SDK when the access token is required.
 Access token won't be expired unless you set it explicitly by <[Kii setAccessTokenExpiration:]>.
 User token can be get by <[KiiUser accessToken]>.
 From next time, it is possible to login with the access token until the token is expired.
 
     [KiiUser authenticate:@"myusername" withPassword:@"mypassword" andBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"Authenticated user: %@", user);
         }
     }];
 
 @param userIdentifier Can be a username or a verified phone number or a verified email address
 @param password The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @param block The block to be called upon method completion. See example
 
 */
+ (void) authenticate:(NSString*)userIdentifier
         withPassword:(NSString*)password
             andBlock:(KiiUserBlock)block;

/** Asynchronously authenticates a user with the server
 
 Authenticates a user with the server. This method is non-blocking.
 If successful, the user is cached inside SDK as current user, and accessible via <[KiiUser currentUser]>.
 This user token is also cached and used by the SDK when the access token is required.
 Access token won't be expired unless you set it explicitly by <[Kii setAccessTokenExpiration:]>.
 User token can be get by <[KiiUser accessToken]>.
 From next time, it is possible to login with the access token until the token is expired.
 
 @param userIdentifier Can be a username or a verified phone number or a verified email address
 @param password The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) authenticationComplete:(KiiUser*)user withError:(NSError*)error {
     
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 
 */
+ (void) authenticate:(NSString*)userIdentifier
         withPassword:(NSString*)password
          andDelegate:(id)delegate
          andCallback:(SEL)callback;


/** Synchronously authenticates a user with the server
 
 Authenticates a user with the server. This method is blocking.
 
 If successful, the user is cached inside SDK as current user, and accessible via <[KiiUser currentUser]>.
 This user token is also cached and used by the SDK when the access token is required.
 Access token won't be expired unless you set it explicitly by <[Kii setAccessTokenExpiration:]>.
 User token can be get by <[KiiUser accessToken]>.
 From next time, it is possible to login with the access token until the token is expired.
 
 @param userIdentifier Can be a username or a verified phone number or a verified email address
 @param password The user's password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return The KiiUser object that was authenticated. nil if failed to authenticate
 */
+ (KiiUser*) authenticateSynchronous:(NSString*)userIdentifier
                        withPassword:(NSString*)password
                            andError:(NSError**)error;



/** Asynchronously authenticates a user with the server using a valid access token
 
  Authenticates a user with the server. This method is non-blocking.
 
    [KiiUser authenticateWithToken:@"my-user-token"
                          andBlock:^(KiiUser *user, NSError *error) {
        if(error == nil) {
            NSLog(@"Authenticated user: %@", user);
        }
     }];
 
 @note If successful, the user is cached inside SDK as current user, and accessible via <[KiiUser currentUser]>.
 Specified token is also cached and can be get by
 <[KiiUser accessTokenDictionary]> . Note that, token expiration
 time is not cached and set to [NSDate distantFuture].
 If you want token expiration time also be cached, use <[KiiUser authenticateWithTokenSynchronous:andExpiresAt:andError:]> instead.
 @param accessToken A valid access token associated with the desired user
 @param block The block to be called upon method completion. See example
*/
+ (void) authenticateWithToken:(NSString *)accessToken
                      andBlock:(KiiUserBlock)block;

/** Asynchronously authenticates a user with the server using specified access token. This method is non-blocking.
 
 
    [KiiUser authenticateWithToken:@"my-user-token" 
                      andExpiresAt: expiresAt
                          andBlock:^(KiiUser *user, NSError *error) {
        if(error == nil) {
        NSLog(@"Authenticated user: %@", user);
        }
    }];
 
 Specified expiresAt won't be used by SDK. IF login successful, we set this property so that you
 can get it later along with token by <[KiiUser accessTokenDictionary]>.
 Also, if successful, the user is cached inside SDK as current user and accessible
 via <[KiiUser currentUser]>.
 
 @param accessToken A valid access token associated with the desired user.
 @param expiresAt NSDate representation of accessToken expiration obtained from <[KiiUser accessTokenDictionary]>.
 @param block The block to be called upon method completion. See example.
 @return The KiiUser object that was authenticated. nil if failed to authenticate.
 */
+ (void) authenticateWithToken:(NSString *)accessToken
                  andExpiresAt:(NSDate*) expiresAt
                      andBlock:(KiiUserBlock)block;

/** Asynchronously authenticates a user with the server using a valid access token
 
 Authenticates a user with the server. This method is non-blocking.
 If the specified token is expired, authenticataiton will be failed.
 Authenticate the user again to renew the token.
 @note If successful, the user is cached inside SDK as current user, and accessible via <[KiiUser currentUser]>.
 Specified token is also cached and can be get by
 <[KiiUser accessTokenDictionary]> . Note that, token expiration
 time is not cached and set to [NSDate distantFuture].
 If you want token expiration time also be cached, use <[KiiUser authenticateWithTokenSynchronous:andExpiresAt:andError:]> instead.
 @param accessToken A valid access token associated with the desired user
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) authenticationComplete:(KiiUser*)user withError:(NSError*)error {
     
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
     
         else {
             // there was a problem
         }
     }
 
 */
+ (void) authenticateWithToken:(NSString*)accessToken
                   andDelegate:(id)delegate
                   andCallback:(SEL)callback;



/** Synchronously authenticates a user with the server using a valid access token
 
 Authenticates a user with the server. This method is blocking.
 If the specified token is expired, authenticataiton will be failed. 
 Authenticate the user again to renew the token.
 @note If successful, the user is cached inside SDK as current user, and
 accessible via <[KiiUser currentUser]>. Specified token is also cached and
 can be get by <[KiiUser accessTokenDictionary]> . Note that, token expiration
 time is not cached and set to [NSDate distantFuture].
 If you want token expiration time also be cached, use 
 <[KiiUser authenticateWithTokenSynchronous:andExpiresAt:andError:]> instead.
 @param accessToken A valid access token associated with the desired user
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return The KiiUser object that was authenticated. nil if failed to authenticate
 */
+ (KiiUser*) authenticateWithTokenSynchronous:(NSString*)accessToken
                                     andError:(NSError**)error;

/** Synchronously authenticates a user with the server using specified access token. This method is blocking.
 
 Specified expiresAt won't be used by SDK. IF login successful, we set this property so that you
 can get it later along with token by <[KiiUser accessTokenDictionary]>.
 Also, if successful, the user is cached inside SDK as current user and accessible
 via <[KiiUser currentUser]>.
 
 @param accessToken A valid access token associated with the desired user.
 @param expiresAt NSDate representation of accessToken expiration obtained from <[KiiUser accessTokenDictionary]>.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return The KiiUser object that was authenticated. nil if failed to authenticate.
 */
+ (KiiUser*) authenticateWithTokenSynchronous:(NSString*)accessToken
                                 andExpiresAt:(NSDate*)expiresAt
                                     andError:(NSError**)error;
/** Asynchronously authenticates a user with stored credentials from KeyChain.
 
    [KiiUser authenticateWithStoredCredentials:^(KiiUser *user, NSError *error) {
        if (error == nil) {
            // Succeeded.
        }
    }];
 @note This method just restores the predefined fields locally. If you want to get custom fields, you need to access server by calling <[KiiUser refreshWithBlock:]>
 @note Prior to v2.2.2, KiiSDK stored user credentials with kSecAttrAccessibleWhenUnlock,
 so saving or loading user credentials would fail when device was unlocked. From v2.2.2,
 KiiSDK stores user credentials with kSecAttrAccessibleAfterFirstUnlock, which means 
 as long as your app restarts after user first unlock device, it is possible to 
 authenticate with stored user credentials, even the device is lockd.
 @param block The block to be called upon method completion. See example
 */
+ (void) authenticateWithStoredCredentials:(KiiUserBlock)block;

/** Synchronously authenticates a user with stored credentials from KeyChain.
 
 Please confirm the details <[KiiUser authenticateWithStoredCredentials:]>

 @param error error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return The KiiUser object that was authenticated. nil if failed to authenticate
 */
+ (KiiUser *) authenticateWithStoredCredentialsSynchronous:(NSError **) error;

/** Asynchronously registers a user object with the server
 
 Registers a user with the server. The user object must have an associated email/password combination. This method is non-blocking.
 If the specified token is expired, authenticataiton will be failed.
 Authenticate the user again to renew the token.
 
     [u performRegistrationWithBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"Registered user: %@", user);
         }
     }];
 
 @param block The block to be called upon method completion. See example
*/
- (void) performRegistrationWithBlock:(KiiUserBlock)block;


/** Asynchronously registers a user object with the server
 
 Registers a user with the server. The user object must have an associated email/password combination. This method is non-blocking.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) registrationComplete:(KiiUser*)user withError:(NSError*)error {
     
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
     
         else {
             // there was a problem
         }
     }
 
 */
- (void) performRegistration:(id)delegate withCallback:(SEL)callback;


/** Synchronously registers a user object with the server
 
 Registers a user with the server. The user object must have an associated email/password combination. This method is blocking.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) performRegistrationSynchronous:(NSError**)error;



/** Asynchronously update a user's password on the server
 
 Update a user's password with the server. The fromPassword must be equal to the current password associated with the account in order to succeed. This method is non-blocking.
 
    [user updatePassword:@"current_password" 
                      to:@"new_password"
               withBlock:^(KiiUser *user, NSError *error) {
        if(error == nil) {
            NSLog(@"Updated user password: %@", user);
         }
     }];
 
 @param fromPassword The user's current password
 @param toPassword The user's desired password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @param block The block to be called upon method completion. See example
*/
- (void) updatePassword:(NSString*)fromPassword
                     to:(NSString*)toPassword
              withBlock:(KiiUserBlock)block;

/** Asynchronously update a user's password on the server
 
 Update a user's password with the server. The fromPassword must be equal to the current password associated with the account in order to succeed. This method is non-blocking.
 @param fromPassword The user's current password
 @param toPassword The user's desired password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) passwordUpdateComplete:(KiiUser*)user withError:(NSError*)error {
     
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 
 */
- (void) updatePassword:(NSString*)fromPassword
                     to:(NSString*)toPassword
           withDelegate:(id)delegate
            andCallback:(SEL)callback;


/** Synchronously update a user's password on the server
 
 Update a user's password with the server. The fromPassword must be equal to the current password associated with the account in order to succeed. This method is blocking.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @param fromPassword The user's current password
 @param toPassword The user's desired password. Password must be 4-50 characters and can include these characters: a-z, A-Z, 0-9, @, #, $, %, ^, and &.
 */
- (void) updatePasswordSynchronous:(NSError**)error
                              from:(NSString*)fromPassword
                                to:(NSString*)toPassword;



/** Asynchronously reset a user's password on the server.
 
 Reset a user's password on the server. The user is determined by the specified email address that has already been associated with an account. Reset instructions will be sent to that email address. This method is non-blocking.

 
     [KiiUser resetPassword:@"myemail@address.com"
                  withBlock:^(NSError *error) {
         if(error == nil) {
             NSLog(@"Reset user password!");
         }
     }];

 @param userIdentifier The email address which the account is associated with.
 @param block The block to be called upon method completion. See example.
*/
+ (void) resetPassword:(NSString*)userIdentifier withBlock:(KiiErrorBlock)block;

/** Asynchronously reset a user's password on the server.
 
 Reset a user's password on the server. The user is determined by the specified email address that has already been associated with an account. Reset instructions will be sent to that email address. This method is non-blocking.
 @param userIdentifier The email address which the account is associated with.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) passwordResetComplete:(NSError*)error {
     
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 
 */
+ (void) resetPassword:(NSString*)userIdentifier
          withDelegate:(id)delegate
           andCallback:(SEL)callback;


/** Synchronously reset a user's password on the server.
 
 Reset a user's password on the server.The user is determined by the specified email address that has already been associated with an account. Reset instructions will be sent to that email address. This method is blocking.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @param userIdentifier The email address which the account is associated with.
 */
+ (void) resetPasswordSynchronous:(NSError**)error
               withUserIdentifier:(NSString*)userIdentifier;

/** Synchronously reset the user's password.<br>
 Reset the password of user specified by given identifier.<br>
 This api does not execute login after reset.
 @param userIdentifier should be valid email address, global phone number or
 user identifier obtained by <userID>
 @param notificationMethod specify destination of message includes reset password
 link url.
 different type of identifier and destination can be used as long as user have 
 verified email, phone.
 (ex. User registers both email and phone. Identifier is email and 
 notificationMethod is SMS.)
 @param error On input, a pointer to an error object.
 If an error occurs, this pointer is set to an actual error object containing
 the error information.
 You can not specify nil for this parameter or it will cause runtime error.
 @exception NSInvalidArgumentException notificationMethod arguments is not type of KiiNotificationMethod enum.
 */
+ (void) resetPasswordSynchronous:(NSString*)userIdentifier
                 notificationMethod:(KiiNotificationMethod)notificationMethod
                            error:(NSError**)error;

/** Asynchronous version of <resetPasswordSynchronous:notificationMethod:error:><br>
 Reset the password of user specified by given identifier.<br>
 This api does not execute login after reset.<br>
 @param userIdentifier should be valid email address, global phone number or
 user identifier obtained by <userID>
 @param notificationMethod specify destination of message includes reset password
 link url.
 different type of identifier and destination can be used as long as user have
 verified email, phone.
 (ex. User registers both email and phone. Identifier is email and
 notificationMethod is SMS.)
 @param block The block to be called upon method completion.
 @exception NSInvalidArgumentException notificationMethod arguments is not type of KiiNotificationMethod enum.
 */
+ (void) resetPassword:(NSString*)userIdentifier
      notificationMethod:(KiiNotificationMethod)notificationMethod
                 block:(KiiErrorBlock)block;

/** Asynchronously verify the current user's phone number
 
 This method is used to verify the phone number of the currently logged in user. This is a non-blocking method.

     [u verifyPhoneNumber:@"verification_code"
                withBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"Verification complete!");
         }
     }];

 @param code The code which verifies the currently logged in user
 @param block The block to be called upon method completion. See example
 @see [KiiUser verifyPhoneNumber:withCode:]
*/
- (void) verifyPhoneNumber:(NSString*)code withBlock:(KiiUserBlock)block;

/** Synchronously verify the current user's phone number
 
 This method is used to verify the phone number of the currently logged in user.
 This is a blocking method.
 
 Verification code is sent from Kii Cloud through SMS
 when new user is registered with phone number or user requested to change their
 phone number in the application which requires phone verification.<br>

 (You can enable/disable phone verification
 through the console in developer.kii.com)<br>
 
 After the verification succeeded,
 new phone number becomes users phone number and user is able to login with the
 phone number.<br>
 To get the new phone number,
 please call <[KiiUser refreshSynchronous:]> (or its asynchronuos version)
 before access to <phoneNumber>.<br>
 Before completion of <[KiiUser refreshSynchronous:]>,
 value of <phoneNumber> is cached one.
 It could be old phone number or nil.

 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @param code The code which verifies the currently logged in user
 */
// TODO - change method name to end with 'synchronous'
- (void) verifyPhoneNumber:(NSError**)error
                  withCode:(NSString*)code;


/** Asynchronously verify the current user's phone number
 
 This method is used to verify the phone number of the currently logged in user. This is a non-blocking method.
 @param code The code which verifies the currently logged in user
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 @see [KiiUser verifyPhoneNumber:withCode:]
 
     - (void) verificationComplete:(KiiUser*)user withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 
 */
- (void) verifyPhoneNumber:(NSString*)code
              withDelegate:(id)delegate
               andCallback:(SEL)callback;


/** Asynchronously resend the email verification
 
 This method is used to resend the email verification for the currently logged in user. This is a non-blocking method.
 
     [u resendEmailVerificationWithBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"Email verification resent!");
         }
     }];

 @param block The block to be called upon method completion. See example
*/
- (void) resendEmailVerificationWithBlock:(KiiUserBlock)block;

/** Synchronously resend the email verification
 
 This method will re-send the email verification to the currently logged in user. This is a blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) resendEmailVerificationSynchronous:(NSError**)error;


/** Asynchronously resend the email verification
 
 This method is used to resend the email verification for the currently logged in user. This is a non-blocking method.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) verificationCodeReSent:(KiiUser*)user withError:(NSError*)error {
     
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 
 */
- (void) resendEmailVerification:(id)delegate
                     andCallback:(SEL)callback;




/** Asynchronously resend the phone number verification
 
 This method is used to re-send the SMS verification for the currently logged in user. This is a non-blocking method.
 
     [u resendPhoneNumberVerificationWithBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"SMS verification resent!");
         }
     }];
 
 @param block The block to be called upon method completion. See example
 */
- (void) resendPhoneNumberVerificationWithBlock:(KiiUserBlock)block;

/** Synchronously resend the phone number verification
 
 This method will re-send the SMS verification to the currently logged in user. This is a blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) resendPhoneNumberVerificationSynchronous:(NSError**)error;


/** Asynchronously resend the phone number verification
 
 This method is used to resend the phone number verification for the currently logged in user. This is a non-blocking method.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) verificationCodeReSent:(KiiUser*)user withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 
 */
- (void) resendPhoneNumberVerification:(id)delegate
                           andCallback:(SEL)callback;



/** Asynchronously gets a list of groups which the user is a member of
 
 This is a non-blocking method.
 
     [u memberOfGroupsWithBlock:^(KiiUser *user, NSArray *results, NSError *error) {
         if(error == nil) {
             NSLog(@"User %@ is member of groups: %@", user, results);
         }
     }];

 @param block The block to be called upon method completion. See example
*/
- (void) memberOfGroupsWithBlock:(KiiUserArrayBlock)block;

/** Synchronously gets a list of groups which the user is a member of
 
 This is a blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return An array of <KiiGroup> objects
 */
- (NSArray*) memberOfGroupsSynchronous:(NSError**)error;


/** Asynchronously gets a list of groups which the user is a member of
 
 This is a non-blocking method.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) userIsMember:(KiiUser*)user ofGroups:(NSArray*)groups withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             
             // do something with the user's groups
             for(KiiGroup *g in groups) {
                 // do something with the group
             }
         }
         
         else {
             // there was a problem
         }
     }
 
 */
- (void) memberOfGroups:(id)delegate andCallback:(SEL)callback;

/** Get or create a bucket at the user level
 
 @param bucketName The name of the bucket you'd like to use
 @return An instance of a working <KiiBucket>
 */
- (KiiBucket*) bucketWithName:(NSString*)bucketName;

/** Get or create an encrypted bucket at the user level.
 
 @param bucketName The name of the encrypted bucket you'd like to use.
 @return An instance of a working <KiiEncryptedBucket>
 @exception NSInvalidArgumentException when bucketName is not acceptable format. For details please refer to <[KiiBucket isValidBucketName:(NSString*) bucketName]>.
 */
- (KiiEncryptedBucket*) encryptedBucketWithName:(NSString*)bucketName;


/** Get or create a file bucket at the user level
 
 @param bucketName The name of the file bucket you'd like to use
 @return An instance of a working <KiiFileBucket>
 @deprecated This method is deprecated. Use <[KiiUser bucketWithName:]> instead.
 */
- (KiiFileBucket*) fileBucketWithName:(NSString*)bucketName __attribute__((deprecated("Use [KiiUser bucketWithName:] instead.")));

/** Get or create a Push notification topic at the user level
 
 @param topicName The name of the topic you'd like to use. It has to match the pattern ^[A-Za-z0-9_-]{1,64}$, that is letters, numbers, '-' and '_' and non-multibyte characters with a length between 1 and 64 characters.
 @return An instance of a working <KiiTopic>
 */
- (KiiTopic*) topicWithName:(NSString*)topicName;


/** Asynchronously updates the local user's data with the user data on the server
 
 The user must exist on the server. Local data will be overwritten.
 
     [u refreshWithBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"User refreshed: %@", user);
         }
     }];
 
 @param block The block to be called upon method completion. See example
*/
- (void) refreshWithBlock:(KiiUserBlock)block;

/** Asynchronously updates the local user's data with the user data on the server
 
 The user must exist on the server. Local data will be overwritten.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) userRefreshed:(KiiUser*)user withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 
 */
- (void) refresh:(id)delegate withCallback:(SEL)callback;


/** Synchronously updates the local user's data with the user data on the server
 
 The user must exist on the server. Local data will be overwritten. This is a blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) refreshSynchronous:(NSError**)error;


/** Asynchronously saves the latest user values to the server
 
 The user must exist in order to make this method call. If the user does exist, the application-specific fields that have changed will be updated accordingly. This is a non-blocking method.
 
     [u saveWithBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"User saved: %@", user);
         }
     }];
 
 @param block The block to be called upon method completion. See example
 @deprecated This method is deprecated. Use <[KiiUser updateWithIdentityData:userFields:block:]> instead.
*/
- (void) saveWithBlock:(KiiUserBlock)block;

/** Asynchronously saves the latest user values to the server
 
 The user must exist in order to make this method call. If the user does exist, the application-specific fields that have changed will be updated accordingly. This is a non-blocking method.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) userSaved:(KiiUser*)user withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 
 @deprecated This method is deprecated. Use <[KiiUser updateWithIdentityData:userFields:block:]> instead.
 */
- (void) save:(id)delegate withCallback:(SEL)callback;


/** Synchronously saves the latest user values to the server
 
 The user must exist in order to make this method call. If the user does exist, the application-specific fields that have changed will be updated accordingly. This is a blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @deprecated This method is deprecated. Use <[KiiUser updateWithIdentityDataSynchronous:userFields:error:]> instead.
 */
- (void) saveSynchronous:(NSError**)error;


/** Asynchronously update user attributes.

 Note: This method is exclusive to logged-in user. If you use this
 method by non logged-in user, then this method fails and notifies
 NSError.

 Note: Local modification done by <KiiUser country> <KiiUser
 displayName> and other setter methods in <KiiUser> will be ignored.
 Please make sure to set new values in KiiUserFields.

     [pseudoUser updateWithUserFields:userFields
                                block:^(KiiUser *user, NSError *error) {
                           if (error != nil) {
                               // fail to update. notify to user.
                               return;
                           }
                           // success to update.
                       }
     ];


 @param userFields Mandatory. Must not be empty.
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException userFields and/or block is nil.
 */
- (void) updateWithUserFields:(KiiUserFields *)userFields
                        block:(KiiUserBlock)block;

/** Asynchronously update user attributes.

 Notes:

 - This method is exclusive to logged-in user. If you use this
 method by non logged-in user, then this method fails and notifies
 NSError.
 
 - If this user is pseudo user and valid identityData given,
 execlution is failed and NSError is passed as KiiUserBlock argument.

 - Local modification done by <KiiUser country> <KiiUser
 displayName> and other setter methods in <KiiUser> will be ignored.
 Please make sure to set new values in KiiUserFields.

 - At least one of identityData or userFields must be set.

     [pseudoUser updateWithIdentityData:identityData
                             userFields:userFields
                                  block:^(KiiUser *user, NSError *error) {
                           if (error != nil) {
                               // fail to update. notify to user.
                               return;
                           }
                           // success to update.
                       }
     ];


 @param identityData Optional. If nil is passed, Identity data would not
 be updated and current value would be retained.
 @param userFields Optional. If nil is passed, display name, country
 and other custom field would not be set. To set those fields, create
 UserFields instance and pass to this API. fields which is not included
 in this instance
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException
 If this user is not pseudo user, raised when Both of identityData and
 userFields are nil. block should not be nil for both pseudo user and non pseudo user.
 */
- (void) updateWithIdentityData:(KiiIdentityData *)identityData
                     userFields:(KiiUserFields *)userFields
                          block:(KiiUserBlock)block;

/** Synchronously update user attributes.

 Note: This method is exclusive to logged-in user. If you use this
 method by non logged-in user, then this method fails and notifies
 NSError.

 @param userFields Optional. If nil is passed, display name, country
 and other custom field would not be set. To set those fields, create
 UserFields instance and pass to this API. fields which is not included
 in this instance
 @param error An NSError object, can be nil but not recommended.
 */
- (void) updateWithUserFieldsSynchronous:(KiiUserFields *)userFields
                                   error:(NSError **)error;

/** Synchronously update user attributes.

 Note: This method is exclusive to logged-in user. If you use this
 method by non logged-in user, then this method fails and notifies
 NSError.

 Note: Local modification done by <KiiUser country> <KiiUser
 displayName> and other setter methods in <KiiUser> will be ignored.
 Please make sure to set new values in KiiUserFields.

 Note: At least one of identityData or userFields must be set.

 @param identityData Optional. If nil is passed, Identity data would not
 be updated and current value would be retained.
 @param userFields Optional. If nil is passed, display name, country
 and other custom field would not be set. To set those fields, create
 UserFields instance and pass to this API. fields which is not included
 in this instance
 @param error An NSError object, can be nil but not recommended.
 @exception NSInvalidArgumentException If this user is pseudo user, raised when identityData is not nil.
 If this user is not pseudo user, raised when Both of identityData and
 userFields are nil.
 */
- (void) updateWithIdentityDataSynchronous:(KiiIdentityData *)identityData
                                userFields:(KiiUserFields *)userFields
                                     error:(NSError **)error;

/** Asynchronously deletes the user from the server
 
 The user must exist on the server for this method to execute properly. This is a non-blocking method.
 
     [u deleteWithBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"User deleted!");
         }
     }];
 
 @param block The block to be called upon method completion. See example
*/
- (void) deleteWithBlock:(KiiUserBlock)block;

/** Asynchronously deletes the user from the server
 
 The user must exist on the server for this method to execute properly. This is a non-blocking method.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) userDeleted:(KiiUser*)user withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the object
         }
         
         else {
             // there was a problem
         }
 
     }
 
 */
- (void) delete:(id)delegate withCallback:(SEL)callback;


/** Synchronously deletes the user from the server
 
 The user must exist on the server for this method to execute properly. This is a blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) deleteSynchronous:(NSError**)error;


/** Prints the contents of this user to log
 
 For developer purposes only, this method prints the user in a readable format to the log for testing.
 */
- (void) describe;



/** Sets a key/value pair to a KiiUser
 
 If the key already exists, its value will be written over. If the object is of invalid type, it will return false and an NSError will be thrown (quietly). Accepted types are any JSON-encodable objects.
 @param object The value to be set. Object must be of a JSON-encodable type (Ex: NSDictionary, NSArray, NSString, NSNumber, etc)
 @param key The key to set. The key must not begin with an underscore (_)
 @return True if the object was set, false otherwise.
 */
- (BOOL) setObject:(id)object forKey:(NSString*)key;


/** Checks to see if an object exists for a given key
 
 @param key The key to check for existence
 @return True if the object exists, false otherwise.
 */
- (BOOL) hasObject:(NSString*)key;

/** Checks the if this user is linked with specified social provider.
 <KiiUser refreshSynchronous:> should be called before calling this API, otherwise it will always return NO.
 
 @param <KiiConnectorProvider> The provider to check
 @return YES if this user is linked with specified social provider, NO otherwise.
 */
- (BOOL) isLinkedWithSocialProvider:(KiiConnectorProvider)provider;


/** Removes a specific key/value pair from the object
 If the key exists, the key/value will be removed from the object.
 @param key The key of the key/value pair that will be removed.
 @note Since version 2.1.30, the behavior of this API has been changed. This method just removes the key-value pair from the local cache but no longer sets empty string (@"") to the key and does not send specified key-value pair to the cloud when the update method (<[KiiUser updateWithIdentityDataSynchronous:userFields:error:]> etc.) is called. If you want to have same effect as previous, please execute <setObject:forKey:> with empty string (@"") passed to the object explicitly.
 */
- (void) removeObjectForKey:(NSString*)key;


/** Gets the value associated with the given key
 
 @param key The key to retrieve
 @return An object if the key exists, nil otherwise
 */
- (id) getObjectForKey:(NSString*)key;


/** Updates the user's email address on the server
 
 This is a non-blocking method.
 
     [u changeEmail:@"mynewemail@address.com" withBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"Email changed for user: %@", user);
         }
     }];
 
 @param newEmail The new email address to change to
 @param block The block to be called upon method completion. See example
*/
- (void) changeEmail:(NSString*)newEmail withBlock:(KiiUserBlock)block;

/** Updates the user's email address on the server
 
 This is a non-blocking method.
 @param newEmail The new email address to change to
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) emailChanged:(KiiUser*)user withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
 
     }
 **/
- (void) changeEmail:(NSString*)newEmail withDelegate:(id)delegate andCallback:(SEL)callback;


/** Updates the user's email address on the server
 
 This is a blocking method.
 @param newEmail The new email address to change to
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 **/
- (void) changeEmailSynchronous:(NSString*)newEmail withError:(NSError**)error;

/** Updates the user's phone number on the server
 
 This is a non-blocking method.
 
     [u changePhone:@"mynewphonenumber" withBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
             NSLog(@"Phone changed for user: %@", user);
         }
     }];
 
 @param newPhoneNumber The new phone number to change to
 @param block The block to be called upon method completion. See example
*/
- (void) changePhone:(NSString*)newPhoneNumber withBlock:(KiiUserBlock)block;

/** Updates the user's phone number on the server
 
 This is a non-blocking method.
 @param newPhoneNumber The new phone number to change to
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) phoneChanged:(KiiUser*)user withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
 
     }
 **/
- (void) changePhone:(NSString*)newPhoneNumber withDelegate:(id)delegate andCallback:(SEL)callback;


/** Updates the user's phone number on the server
 
 This is a blocking method.
 @param newPhoneNumber The new phone number to change to
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 **/
- (void) changePhoneSynchronous:(NSString*)newPhoneNumber withError:(NSError**)error;


/** Logs the currently logged-in user out of the KiiSDK */
+ (void) logOut;


/** Determines whether or not there is a KiiUser currently logged in
 @return TRUE if an authenticated user exists, FALSE otherwise
 */
+ (BOOL) loggedIn;


/** Get the currently logged-in user
 @return A KiiUser object representing the current user, nil if no user is logged-in
 */
+ (KiiUser*) currentUser;


//search functionality


/** Find user by username asynchronously using block
 This is a non-blocking method.
 
     [u findUserByUsername:@"user_to_found" withBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
         NSLog(@"Found user: %@", user);
         }
     }];
 
 @param username The username of user that want to be discovered
 @param block The block to be called upon method completion. See example
 */
+(void)  findUserByUsername:(NSString*) username withBlock:(KiiUserBlock) block;



/** Find user by user e mail address asynchronously using block
 This is a non-blocking method. This method can only get user that has verified email.
 
     [u findUserByEmail:@"email_to_found" withBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
         NSLog(@"Found user: %@", user);
         }
     }];
 
 @param emailAddress The email address of user that want to be discovered. User can only find specific user from email that has been verified.
 @param block The block to be called upon method completion. See example 
 */
+(void) findUserByEmail:(NSString*) emailAddress  withBlock:(KiiUserBlock) block;


/** Find user by user phone asynchronously using block
 This is a non-blocking method.  This method can only get user that has verified phone number.
 
     [u findUserByPhone:@"phoneNumber_to_found" withBlock:^(KiiUser *user, NSError *error) {
         if(error == nil) {
         NSLog(@"Found user: %@", user);
         }
     }];
 

 
 @param phoneNumber the global phone number of user that want to be discovered. Do not pass local phone number, it is not supported. User can only find specific user from phone number that has been verified.
 @param block The block to be called upon method completion. See example
 */
+(void) findUserByPhone:(NSString*) phoneNumber  withBlock:(KiiUserBlock) block;

/** Find user by username asynchronously using delegate and callback
  This is a non-blocking method.
 @param username The username of user that want to be discovered
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) userFound:(KiiUser*)user withError:(NSError*)error {
     
         // the request was successful
         if(error == nil) {
         // do something with the user
         }
         
         else {
         // there was a problem
         }
     
     }
 */
+(void) findUserByUsername:(NSString*) username withDelegate :(id) delegate andCallback:(SEL) callback;



/** Find user by user e mail address asynchronously using delegate and callback
 This is a non-blocking method. This method can only get user that has verified email.
 
 @param emailAddress The email address of user that want to be discovered. User can only find specific user from email that has been verified.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
     
     - (void) userFound:(KiiUser*)user withError:(NSError*)error {
     
         // the request was successful
         if(error == nil) {
         // do something with the user
         }
         
         else {
         // there was a problem
         }
     
     }
 */
+(void)  findUserByEmail:(NSString*) emailAddress  withDelegate :(id) delegate andCallback:(SEL) callback;




/** Find user by user phone asynchronously using delegate and callback
 This is a non-blocking method.  This method can only get user that has verified phone number.
 @param phoneNumber The global phone number of user that want to be discovered. Do not pass local phone number, it is not supported. User can only find specific user from phone number that has been verified.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) userFound:(KiiUser*)user withError:(NSError*)error {
     
         // the request was successful
         if(error == nil) {
         // do something with the user
         }
         
         else {
         // there was a problem
         }
     
     }
 */
+(void)  findUserByPhone:(NSString*) phoneNumber  withDelegate :(id) delegate andCallback:(SEL) callback;



/** Find user by username synchronously
  This is a blocking method. 
 
 @param username The username of user that want to be discovered
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
+(KiiUser*) findUserByUsernameSynchronous:(NSString*) username withError:(NSError**) error;



/** Find user by user e mail address synchronously
 This is a blocking method. This method can only get user that has verified email.
 
 @param emailAddress The email address of user that want to be discovered. User can only find specific user from email that has been verified.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
+(KiiUser*) findUserByEmailSynchronous:(NSString*) emailAddress withError:(NSError**) error;



/** Find user by user phone synchronously
 This is a blocking method. This method can only get user that has verified phone number.
 
 @param phoneNumber The global phone number of user that want to be discovered. Do not pass local phone number, it is not supported. User can only find specific user from phone number that has been verified.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
+(KiiUser*) findUserByPhoneSynchronous:(NSString*) phoneNumber withError:(NSError**) error;



/** Asynchronously gets the groups owned by this user.
 Group in the returned array does not contain all the property of group. To get all the
 property from cloud, KiiGroup refresh is necessary.
 This method is non-blocking.

     [u ownerOfGroupsWithBlock:^(KiiUser *user, NSArray *results, NSError *error) {
         if(error == nil) {
             NSLog(@"User %@ is owner of groups: %@", user, results);
         }
     }];

 @param block The block to be called upon method completion. See example.
 @see [KiiGroup refreshSynchronous:]
 @see [KiiGroup refreshWithBlock:]
 @see [KiiGroup refresh:withCallback:]
 */
- (void) ownerOfGroupsWithBlock:(KiiUserArrayBlock)block;

/**
 Synchronously gets groups owned by this user. Group in
 the returned array does not contain all the property of group. To get all the
 property from cloud, KiiGroup refresh is necessary.This method is blocking.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return An NSArray Array of groups owned by the user.
 @see [KiiGroup refreshSynchronous:]
 @see [KiiGroup refreshWithBlock:]
 @see [KiiGroup refresh:withCallback:]
 */
- (NSArray*) ownerOfGroupsSynchronous:(NSError**)error;

/** Set identity data to pseudo user.

 This user must be current user. password is mandatory and needs to
 provide at least one of login name, email address or phone number.
 Before call this method, calling refreshing method such as
 refreshWithBlock: is necessary to keep custom fields, otherwise
 custom fields will be gone.

 Note: This method is exclusive to logged-in pseudo user. If you use
 this method by non logged-in user, then this method fails and
 notifies NSError.

 @param identityData Identity data must not be nil.
 @param userFields Optional. If nil is passed, display name, country
 and other custom field would not be update. To update those fields,
 create UserFields instance and pass to this API. fields which is not
 included in this instance
 @param password Password must not be nil. password must match a
 following regular expression: ^[a-zA-Z0-9]{4,64}$
 @param error An NSError object, can be nil but not recommended.
 @exception NSInvalidArgumentException One or more arguments are invalid.
 @exception NSInternalInconsistencyException Current user is not
 pseudo user.
 */
- (void)putIdentityDataSynchronous:(KiiIdentityData *)identityData
                    userFields:(KiiUserFields *)userFields
                      password:(NSString *)password
                         error:(NSError **)error;

/** Register this user as pseudo user on KiiCloud.

 This method registers this user as pseudo user on KiiCloud.
 @param userFields Optional. If nil is passed, display name, country
 and other custom field would not be set. To set those fields, create
 UserFields instance and pass to this API. fields which is not included
 in this instance
 @param error An NSError object, can be nil but not recommended.
 @return Registered new KiiUser.
 */
+ (KiiUser *)registerAsPseudoUserSynchronousWithUserFields:(KiiUserFields *)userFields
                                                     error:(NSError **)error;

/** Set identity data to pseudo user.

 This user must be current user. password is mandatory and needs to
 provide at least one of login name, email address or phone number.
 Before call this method, calling refreshing method such as
 refreshWithBlock: is necessary to keep custom fields, otherwise
 custom fields will be gone. This method is non-blocking.


     [pseudoUser putIdentityData:identity
                        password:password
                           block:^(KiiUser *user, NSError *error) {
                           if (error != nil) {
                               // fail to putIdentity. notify to user.
                               return;
                           }
                           // success to putIdentity.
                       }
     ];

 Note: This method is exclusive to logged-in pseudo user. If you use
 this method by non logged-in user, then this method fails and
 notifies NSError.

 @param identityData Identity data must not be nil.
 @param userFields Optional. If nil is passed, display name, country
 and other custom field would not be update. To update those fields,
 create UserFields instance and pass to this API. fields which is not
 included in this instance
 @param password Password must not be nil. password must match a
 following regular expression: ^[a-zA-Z0-9]{4,64}$
 @param block The block to be called upon method completion. must not
 be nil. See example.

 @exception NSInvalidArgumentException block is nil.
 @exception NSInternalInconsistencyException raised when this user is not a
 pseudo user.
 */
- (void)putIdentityData:(KiiIdentityData *)identityData
             userFields:(KiiUserFields *)userFields
               password:(NSString *)password
                  block:(KiiUserBlock)block;

/** Register this user as pseudo user on KiiCloud.

 This method registers this user as pseudo user on KiiCloud.

 custom fields will be gone. This method is non-blocking.


     [KiiUser registerAsPseudoUserWithUserFields:nil
                                           block:^(KiiUser *user, NSError *error) {
                 if (error != nil) {
                     // fail to register. notify to user.
                     return;
                 }
                 // success to register.
             }
     ];


 @param userFields Optional. If nil is passed, display name, country
 and other custom field would not be set. To set those fields, create
 UserFields instance and pass to this API. fields which is not included
 in this instance
 @param block The block to be called upon method completion. must not
 be nil. See example.
 @exception NSInvalidArgumentException One or more arguments are invalid.
 */
+ (void)registerAsPseudoUserWithUserFields:(KiiUserFields *)userFields
                                     block:(KiiUserBlock)block;

/**
 Return YES when the specified KiiUser is in following conditions.
 - KiiUser ID is equal to this one.
 - (If KiiUser does not have ID) KiiUser instance is equal to this one.
@param object KiiUser object that will be compared to this.
@return YES if the given KiiUser is equal to this, NO otherwise.
*/
- (BOOL)isEqual:(id)object;

- (NSUInteger)hash;
/**
 Return the access tokens in a dictionary.

 Dictionary contains following key/values.
 <table border=4 width=250>
   <tr>
    <th>Key</th>
    <th>Type</th>
    <th>Value</th>
   </tr>
   <tr>
    <td>"access_token"</td>
    <td>NSString</td>
    <td>required for accessing KiiCloud</td>
   </tr>
   <tr>
    <td>"expires_at"</td>
    <td>NSDate</td>
    <td>Access token expiration date time (Since January 1, 1970 00:00:00 UTC),
 or [NSDate distantFuture] if the session doesn't expire.</td>
   </tr>
   <tr>
    <td>"refresh_token"</td>
    <td>NSString</td>
    <td>required for refreshing access token</td>
   </tr>
 
 </table>
 @return Dictionary contains accessToken information (see table above),returns nil if user not logged in.  
 */
- (NSDictionary *) accessTokenDictionary;

/** Get or create a push subscription for the user.
 
 @return An instance of a working <KiiPushSubscription>
 */
- (KiiPushSubscription*) pushSubscription;

/**Returns the topics in this user scope. This is blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return  a <KiiListResult> object representing list of topics in this thing scope.
 */
- (KiiListResult*) listTopicsSynchronous:(NSError**) error;

/**Returns the topics in this user scope. This is blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @param paginationKey pagination key. If nil or empty value is specified, this API regards no paginationKey specified.
 @return  a <KiiListResult> object representing list of topics in this user scope.
 */
- (KiiListResult*) listTopicsSynchronous:(NSString*) paginationKey error:(NSError**) error;

/**Returns the topics in this user scope asynchronously.

 Receives a <KiiListResult> object representing list of topics. This is a non-blocking request.

    [aUser listTopics:^(KiiListResult *topics, id callerObject, NSError *error){
       //at this scope, callerObject should be KiiUser instance
       NSLog(@"%@",callerObject);
       if(error == nil) {
            NSLog(@"Got Results: %@", topics);
            NSLog(@"Total topics: %@", topics.results.count);
            NSLog(@"Has Next: %@ next paginationKey: %@", topics.hasNext?@"Yes":@"No", topics.paginationKey);
            KiiTopic *firstTopic = topics.result.firstObject;
            if (firstTopic){
                NSLog(@"topic name :%@", firstTopic.name);
            }
       }
    }];

 @param completion The block to be called upon method completion, this is mandatory. See example.
 @exception NSInvalidArgumentException if completion is nil.
 */
- (void) listTopics:(KiiListResultBlock) completion;

/**Returns the topics in this user scope asynchronously.

 Receives a <KiiListResult> object representing list of topics. This is a non-blocking request.

    [aUser listTopics:paginationKey block:^(KiiListResult *topics, id callerObject, NSError *error){
       //at this scope, callerObject should be KiiUser instance
       NSLog(@"%@",callerObject);
       if(error == nil) {
            NSLog(@"Got Results: %@", topics);
            NSLog(@"Total topics: %@", topics.results.count);
            NSLog(@"Has Next: %@ next paginationKey: %@", topics.hasNext?@"Yes":@"No", topics.paginationKey);
            KiiTopic *firstTopic = topics.result.firstObject;
            if (firstTopic){
                NSLog(@"topic name :%@", firstTopic.name);
            }
       }
    }];

 @param paginationKey pagination key. If nil or empty value is specified, this API regards no paginationKey specified.
 @param completion The block to be called upon method completion, this is mandatory. See example.
 @exception NSInvalidArgumentException if completion is nil.
 */
- (void) listTopics:(NSString*) paginationKey block:(KiiListResultBlock) completion;

@end
