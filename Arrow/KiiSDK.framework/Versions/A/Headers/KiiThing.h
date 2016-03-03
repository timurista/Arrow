//
//  KiiThing.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 12/16/14.
//  Copyright (c) 2014 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiThingOwner.h"
#import "KiiBaseObject.h"
#import "KiiListResult.h"

@class KiiTopic;
@class KiiBucket;
@class KiiThing;
@class KiiThingFields;
@class KiiPushSubscription;

typedef void (^KiiThingBlock)(KiiThing *thing, NSError *error);
typedef void (^KiiThingOwnerBlock)(id<KiiThingOwner> owner, NSError *error);
typedef void (^KiiThingIsOwnerBlock)(KiiThing *thing, id<KiiThingOwner> thingOwner,BOOL isOwner, NSError *error);

/** Represent Thing on KiiCloud. There are two types of property of KiiThing, reserve and custom.
 
 <h3>Reserve Fields</h3>

 Reserve keys can not be used as the key of <[KiiBaseObject setObject:forKey:]> methods of KiiThing.

 To set/get the value of these reserve fields, you use the property accessor of these field.
 
 - _thingID
 - _vendorThingID
 - _password
 - _thingType
 - _vendor
 - _firmwareVersion
 - _productName
 - _lot
 - _created
 - _stringField1
 - _stringField2
 - _stringField3
 - _stringField4
 - _stringField5
 - _numberField1
 - _numberField2
 - _numberField3
 - _numberField4
 - _numberField5
 - _accessToken
 
 
 <h3>Custom Fields</h3>
 
 
 Custom field includes any field choose by you, other than reserve field.
 
 Custom field can have NString, NSNumber type values and collections (NSArray/NSDictionary) contains NSString/NSNumber.
 
 Setter/getter (<[KiiBaseObject setObject:forKey:]> , <[KiiBaseObject getObjectForKey:]> ) method of these type can be used to set/get the custom field.
 
 @note KiiThing does not support removal of fields from Server.

 */
@interface KiiThing : KiiBaseObject

/**Get the thingID.
 */
@property(nonatomic,readonly) NSString* thingID;

/**Get the vendorThingID.
 */
@property(nonatomic,readonly) NSString* vendorThingID;

/**Set and get thingType.
 */
@property(nonatomic) NSString* thingType;

/**Set and get vendor.
 */
@property(nonatomic) NSString* vendor;

/**Set and get firmwareVersion.
 */
@property(nonatomic) NSString* firmwareVersion;

/**Set and get productName.
 */
@property(nonatomic) NSString* productName;

/**Set and get the lot.
 */
@property(nonatomic) NSString* lot;

/**Get created date.
 */
@property(nonatomic,readonly) NSDate* created;

/**Set and get the stringField1.
 */
@property(nonatomic) NSString* stringField1;

/**Set and get the stringField2.
 */
@property(nonatomic) NSString* stringField2;

/**Set and get the stringField3.
 */
@property(nonatomic) NSString* stringField3;

/**Set and get the stringField4.
 */
@property(nonatomic) NSString* stringField4;

/**Set and get the stringField5.
 */
@property(nonatomic) NSString* stringField5;

/**Set and get the numberField1.
 */
@property(nonatomic) NSNumber* numberField1;

/**Set and get the numberField2.
 */
@property(nonatomic) NSNumber* numberField2;

/**Set and get the numberField3.
 */
@property(nonatomic) NSNumber* numberField3;

/**Set and get the numberField4.
 */
@property(nonatomic) NSNumber* numberField4;

/**Set and get the numberField5.
 */
@property(nonatomic) NSNumber* numberField5;

/**Get the accessToken.
 */
@property(nonatomic,readonly) NSString* accessToken;

/**Get disabled status of the thing.YES if the thing is disabled, NO otherwise.
 */
@property (nonatomic,readonly) BOOL disabled;


#pragma mark - constructor

/** Create a KiiThing that refers to existing thing which has
 specified ID. You have to specify the ID of existing KiiThing. Unlike
 KiiObject, you can not assign ID in the client side.  After
 instantiation, call <[KiiThing refresh:]> to fetch the properties.

 @param id Thing ID. You must not set vendor thing ID instead of thing ID.
 @return instance of KiiThing or nil.
*/
+ (instancetype)thingWithID:(NSString*) thingID;

#pragma mark - enable disable

/** Asynchronously enable the thing in Kii Cloud.
 This method is non-blocking version of <enableSynchronous:>
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 After succeeded, If thing is registered with "persistentToken" option, token should be recovered (Access token which is used before disabling can be available).
 Otherwise, it does not recovered.
 
 It doesn't throw error when the thing is already enabled.
 
    [aThing enable:^(KiiThing *thing, NSError *error) {
            if(error == nil) {
                NSLog(@"Thing is enabled: %@", thing);
            }
    }];
 
 @param block The block to be called upon method completion. See example.
 */
- (void) enable:(KiiThingBlock) block;

/** Synchronously enable the thing in Kii Cloud.
 This is a blocking method.
 <br>This API is authorized by owner of thing.
 <br>Need user login who owns this thing before execute this API.
 <br>To let users to own Thing, please call <registerOwnerSynchronous:error:>

 After succeeded, If thing is registered with "persistentToken" option, token should be recovered (Access token which is used before disabling can be available).
 Otherwise, it does not recovered. 
 
 It doesn't throw error when the thing is already enabled.

 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) enableSynchronous:(NSError**) error;

/** Asynchronously disable the thing in Kii Cloud.
 This method is a non-blocking version of <disableSynchronous:>

 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 After succeeded, access token published for thing is disabled. In a result, only the app administrator and owners of thing can access the thing. Used when user lost the thing and avoid using by unknown users. It doesn't throw error when the thing is already disabled. 
 
    [aThing disable:^(KiiThing *thing, NSError *error) {
            if(error == nil) {
                NSLog(@"Thing is disabled: %@", thing);
            }
    }];
 @param block The block to be called upon method completion. See example.
 */
- (void) disable:(KiiThingBlock) block;

/** Synchronously disable the thing in Kii Cloud.
 This is a blocking method.
 <br>This API is authorized by owner of thing.
 <br>Need user login who owns this thing before execute this API.
 <br>To let users to own Thing, please call <registerOwnerSynchronous:error:>
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 After succeeded, access token published for thing is disabled. In a result, only the app administrator and owners of thing can access the thing. Used when user lost the thing and avoid using by unknown users. It doesn't throw error when the thing is already disabled.
 
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) disableSynchronous:(NSError**) error;

#pragma mark - load
/** Asynchronously load registered thing in Kii Cloud by using vendorThingID.
 This method is non-blocking version of <[KiiThing loadSynchronousWithVendorThingID:error:]>
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.

 Successful operation will return an instance in a block parameter.
 
    [KiiThing loadWithVendorThingID:vendorThingID
                              block:^(KiiThing *thing, NSError *error) {
            if(error == nil) {
                NSLog(@"Thing loaded: %@", thing);
            }
    }];
 
 @param vendorThingID identifier given by thing vendor.
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException Thrown if vendorThingID is nil or empty string.
 */
+ (void) loadWithVendorThingID:(NSString*) vendorThingID
                         block:(KiiThingBlock) block;


/** Synchronously load registered thing in Kii Cloud by using vendorThingID.
 This is a blocking method.
 <br>This API is authorized by owner of thing.
 <br>Need user login who owns this thing before execute this API.
 <br>To let users to own Thing, please call <registerOwnerSynchronous:error:>

 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 @param vendorThingID identifier given by thing vendor.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return an instance of registered thing.
 @exception NSInvalidArgumentException Thrown if vendorThingID is nil or empty string.
 */
+ (instancetype) loadSynchronousWithVendorThingID:(NSString*) vendorThingID
                                            error:(NSError**) error;

/** Asynchronously load registered thing in Kii Cloud by using thingID.
 This method is non-blocking version of <[KiiThing loadSynchronousWithThingID:error:]>
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 Successful operation will return an instance in a block parameter.
 
    [KiiThing loadWithThingID:thingID
                        block:^(KiiThing *thing, NSError *error) {
            if(error == nil) {
                NSLog(@"Thing loaded: %@", thing);
            }
    }];

 
 @param thingID identifier given by Kii Cloud.
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException Thrown if thingID is nil or empty string.
 */
+ (void) loadWithThingID:(NSString*) thingID
                   block:(KiiThingBlock) block;


/** Synchronously load registered thing in Kii Cloud by using thingID.
 This is a blocking method. 
 <br>This API is authorized by owner of thing.
 <br>Need user login who owns this thing before execute this API.
 <br>To let users to own Thing, please call <registerOwnerSynchronous:error:>

 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 @param thingID identifier given by Kii Cloud.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return an instance of registered thing.
 @exception NSInvalidArgumentException Thrown if thingID is nil or empty string.
 */
+ (instancetype) loadSynchronousWithThingID:(NSString*) thingID
                                      error:(NSError**) error;

#pragma mark - register/unregister owner
/** Asynchronously unregister owner of the thing in Kii Cloud.
 This method is non-blocking version of <unregisterOwnerSynchronous:error:>
 
   - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
   
   - An error (error code: <[KiiError codeThingOwnerNotFound]>) will be returned if the passed thing owner (user/group) is not found or already deleted.

   - An error (error code: <[KiiError codeThingOwnershipNotFound]>) will be returned if the passed thing owner (user/group) is not the owner.
 
 Remove ownership of the thing from specified user/ group. 
 
    [aThing unregisterOwner: owner
                      block:^(KiiThing *thing, NSError *error) {
            if(error == nil) {
                NSLog(@"Thing owner unregistered: %@", thing);
            }
    }];

 @param owner user or group that own the Thing.
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException Thrown if owner is nil or not an instance of KiiUser or KiiGroup.
 */
- (void) unregisterOwner:(id<KiiThingOwner>) owner
              block:(KiiThingBlock) block;


/** Synchronously unregister owner of the thing in Kii Cloud.
  This is a blocking method.
 
 Remove ownership of the thing from specified user/ group.
 <br>This API is authorized by owner of thing.
 <br>Need user login who owns this thing before execute this API.

 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
   
 - An error (error code: <[KiiError codeThingOwnerNotFound]>) will be returned if the passed thing owner (user/group) is not found or already deleted.
   
 - An error (error code: <[KiiError codeThingOwnershipNotFound]>) will be returned if the passed thing owner (user/group) is not the owner.
 
 @param owner user or group that own the Thing.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @exception NSInvalidArgumentException Thrown if owner is nil or not an instance of KiiUser or KiiGroup.
 */
- (void) unregisterOwnerSynchronous:(id<KiiThingOwner>) owner
                         error:(NSError**) error;

/** Asynchronously register owner of the thing in Kii Cloud.
 This method is non-blocking version of <registerOwnerSynchronous:error:>
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
   
 - An error (error code: <[KiiError codeThingOwnerNotFound]>) will be returned if the passed thing owner (user/group) is not found or already deleted.
   
 - An error (error code: <[KiiError codeThingOwnershipExist]>) will be returned if the passed thing owner (user/group) is already the owner.
 
 After the registration, owner can access the thing, buckets, objects in bucket, and topics belongs to the thing.
 
    [aThing registerOwner: owner
                    block:^(KiiThing *thing, NSError *error) {
            if(error == nil) {
                NSLog(@"Thing owner registered: %@", thing);
            }
    }];
 
 @param owner user or group that own the Thing.
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException Thrown if owner is nil or not an instance of KiiUser or KiiGroup.
 */
- (void) registerOwner:(id<KiiThingOwner>) owner
              block:(KiiThingBlock) block;


/** Synchronously register owner of the thing in Kii Cloud.
 This is a blocking method.
 <br>This API is authorized by owner of thing.
 <br>Need user login before execute this API.
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
   
 - An error (error code: <[KiiError codeThingOwnerNotFound]>) will be returned if the passed thing owner (user/group) is not found or already deleted.
   
 - An error (error code: <[KiiError codeThingOwnershipExist]>) will be returned if the passed thing owner (user/group) is already the owner.

  After the registration, owner can access the thing, buckets, objects in bucket, and topics belongs to the thing.
 
 @param owner user or group that own the Thing.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @exception NSInvalidArgumentException Thrown if owner is nil or not an instance of KiiUser or KiiGroup.
 */
- (void) registerOwnerSynchronous:(id<KiiThingOwner>) owner
                         error:(NSError**) error;

/** Register owner of specified thing. This is a blocking method.

 After the registration, owner can access the thing, buckets, objects
 in bucket, and topics belongs to the thing.

 This api is authorized by owner of the Thing.

 Need user login before execute this API.

 **NOTE: This api access to server. Should not be executed in UI/Main thread.**

 - An error (error code: <[KiiError codeThingNotFound]>) will be
   returned if the thing is not found or already deleted.
 - An error (error code: <[KiiError codeThingOwnershipExist]>) will be
   returned if the passed thing owner (user/group) is already the
   owner.

 @param owner to be registered as owner
 @param thingID ID of the KiiThing to instantiate.
 @param error On input, a pointer to an error object. If an error
 occurs, this pointer is set to an actual error object containing the
 error information. You can not specify nil for this parameter or it
 will cause runtime error.
 @exception NSInvalidArgumentException Thrown if owner is nil or not
 an instance of KiiUser or KiiGroup.
 */
+ (void) registerOwnerSynchronous:(id<KiiThingOwner>) owner
                          thingID:(NSString*) thingID
                            error:(NSError**) error;

/** Asynchronous version of <[KiiThing
 registerOwnerSynchronous:withThingID:error:]>.

 - An error (error code: <[KiiError codeThingNotFound]>) will be
   returned if the thing is not found or already deleted.
 - An error (error code: <[KiiError codeThingOwnershipExist]>) will be
   returned if the passed thing owner (user/group) is already the
   owner.

 Successful operation will return an instance in a block parameter.

    [KiiThing registerOwner:owner
                    thingID:@"YourThingID"
                      block:^(KiiThingOwner *owner, NSError *error) {
                          if(error == nil) {
                              NSLog(@"Thing owner registered: %@", owner);
                          }
    }];

 @param owner to be registered as owner
 @param thingID ID of the KiiThing to instantiate.
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException Thrown if owner is nil or not
 an instance of KiiUser or KiiGroup.
 */
+ (void) registerOwner:(id<KiiThingOwner>) owner
               thingID:(NSString*) thingID
                 block:(KiiThingOwnerBlock) block;

/** Register owner of specified thing. This is a blocking method.

 After the registration, owner can access the thing, buckets, objects
 in bucket, and topics belongs to the thing.

 This api is authorized by owner of the Thing.

 Need user login before execute this API.

 **NOTE: This api access to server. Should not be executed in UI/Main thread.**

 - An error (error code: <[KiiError codeThingNotFound]>) will be
   returned if the thing is not found or already deleted.
 - An error (error code: <[KiiError codeThingOwnershipExist]>) will be
   returned if the passed thing owner (user/group) is already the
   owner.

 @param owner to be registered as owner
 @param vendorThingID identifier given by thing vendor.
 @param error On input, a pointer to an error object. If an error
 occurs, this pointer is set to an actual error object containing the
 error information. You can not specify nil for this parameter or it
 will cause runtime error.
 @exception NSInvalidArgumentException Thrown if owner is nil or not
 an instance of KiiUser or KiiGroup.
 */
+ (void) registerOwnerSynchronous:(id<KiiThingOwner>) owner
                    vendorThingID:(NSString*) vendorThingID
                            error:(NSError**) error;

/** Asynchronous version of <[KiiThing
 registerOwnerSynchronous:withVendorThingID:error:]>.

 - An error (error code: <[KiiError codeThingNotFound]>) will be
   returned if the thing is not found or already deleted.
 - An error (error code: <[KiiError codeThingOwnershipExist]>) will be
   returned if the passed thing owner (user/group) is already the
   owner.

 Successful operation will return an instance in a block parameter.

    [KiiThing registerOwner:owner
              vendorThingID:@"YourVendorhingID"
                      block:^(KiiThingOwner *owner, NSError *error) {
            if(error == nil) {
                NSLog(@"Thing owner registered: %@", owner);
            }
    }];
 
 @param owner to be registered as owner
 @param vendorThingID identifier given by thing vendor.
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException Thrown if owner is nil or not
 an instance of KiiUser or KiiGroup.
 */
+ (void) registerOwner:(id<KiiThingOwner>) owner
         vendorThingID:(NSString*) vendorThingID
                 block:(KiiThingOwnerBlock) block;

#pragma mark - register
/** Asynchronously register thing in Kii Cloud using block.
 This method is non-blocking version of <[KiiThing registerThingSynchronous:password:type:fields:error:]>
 
 
    [KiiThing registerThing: vendorThingID
                   password: password
                       type: thingType
                     fields: thingFields
                      block:^(KiiThing *thing, NSError *error) {
            if(error == nil) {
                NSLog(@"Thing registered: %@", thing);
            }
    }];
 
 @param vendorThingID identifier. This is required.
 @param password for Thing. This is required.
 @param thingType the thing device type. This is optional.
 @param thingFields a <ThingFields> object.
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException Thrown if vendorThingID is nil or empty string.
 @exception NSInvalidArgumentException Thrown if password is nil or empty string.
 */
+ (void) registerThing:(NSString*) vendorThingID
              password: (NSString*) password
                  type:(NSString*) thingType
                fields:(KiiThingFields*) thingFields
                 block:(KiiThingBlock) block;

/** Synchronously register thing in Kii Cloud.
 This is blocking method.
 <br>It doesn't require user login. Anonymous user can register thing.
 
 @param vendorThingID identifier. This is required.
 @param password for Thing. This is required.
 @param thingType the thing device type. This is optional.
 @param thingFields a <ThingFields> object.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return an instance of registered <KiiThing>.
 @exception NSInvalidArgumentException Thrown if vendorThingID is nil or empty string.
 @exception NSInvalidArgumentException Thrown if password is nil or empty string.
 */
+ (instancetype) registerThingSynchronous:(NSString*) vendorThingID
                                 password:(NSString*) password
                                     type:(NSString*) thingType
                                   fields:(KiiThingFields*) thingFields
                                    error:(NSError**) error;
#pragma mark - isOwner
/**Synchronously check if user/ group is owner of the thing.
 This is blocking method.
 <br>This API is authorized by owner of thing.
 <br>Need user login before execute this API.
 <br>To let users to own Thing, please call <registerOwnerSynchronous:error:>
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
   
 - An error (error code: <[KiiError codeThingOwnerNotFound]>) will be returned if the passed thing owner (user/group) is not found or already deleted.
 
 @param thingOwner user or group to be checked.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return YES if passed parameter is the owner of thing, NO otherwise.
 @exception NSInvalidArgumentException Thrown if owner is nil or not an instance of KiiUser or KiiGroup.
 */
- (BOOL) checkIsOwnerSynchronous:(id<KiiThingOwner>) thingOwner error:(NSError**) error;

/** Asynchronously check if user/group is owner of the thing.
 This method is non-blocking version of <checkIsOwnerSynchronous:error:>
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
   
 - An error (error code: <[KiiError codeThingOwnerNotFound]>) will be returned if the passed thing owner (user/group) is not found or already deleted.
 
 Checks ownership of the thing from specified user/ group.
 
    [aThing checkIsOwner:thingOwner block:^(KiiThing *thing, id<<KiiThingOwner>> thingOwner, BOOL isOwner, NSError *error) {
        if(error == nil) {
            NSLog(@"%@ is %@ the owner" , thingOwner.thingOwnerID,isOwner?@"":@"not");
        }
    }];
 
 @param thingOwner user or group to be checked.
 @param block The block to be called upon method completion. See example.
 @exception NSInvalidArgumentException Thrown if owner is nil or not an instance of KiiUser or KiiGroup.
 */
- (void) checkIsOwner:(id<KiiThingOwner>) thingOwner block:(KiiThingIsOwnerBlock) block;

#pragma mark - save, refresh, delete

/** Asynchronously updates the local Thing's data with the Thing data on the server.
 This method is non-blocking version of <refreshSynchronous:>
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 The Thing must exist on the server. Local data will be overwritten.
 
    [aThing refresh:^(KiiThing *thing, NSError *error) {
        if(error == nil) {
            NSLog(@"Thing refreshed: %@", thing);
        }
    }];
 
 @param block The block to be called upon method completion. See example.
 */
- (void) refresh:(KiiThingBlock)block;

/** Synchronously updates the local Thing's data with the Thing data on the server.
 This is a blocking method.
 <br>This API is authorized by owner of thing.
 <br>Need user login who owns this thing before execute this API.
 <br>To let users to own Thing, please call <registerOwnerSynchronous:error:>

 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 The Thing must exist on the server. Local data will be overwritten. This is a blocking method.
 
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) refreshSynchronous:(NSError**)error;

/** Asynchronously saves the latest Thing values to the server
 This method is non-blocking version of <updateSynchronous:>
 
 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 The Thing must exist in order to make this method call. If the Thing does exist, the application-specific fields that have changed will be updated accordingly. This is a non-blocking method.
 
    [aThing update:^(KiiThing *thing, NSError *error) {
        if(error == nil) {
            NSLog(@"Thing saved: %@", thing);
        }
    }];
 
 @param block The block to be called upon method completion. See example
 */
- (void) update:(KiiThingBlock)block;

/** Synchronously saves the latest Thing values to the server.
 This is a blocking method.
 <br>This API is authorized by owner of thing.
 <br>Need user login who owns this thing before execute this API.
 <br>To let users to own Thing, please call <registerOwnerSynchronous:error:>

 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.

 The Thing must exist in order to make this method call. If the Thing does exist, the application-specific fields that have changed will be updated accordingly. This is a blocking method.

 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) updateSynchronous:(NSError**)error;

/** Asynchronously deletes the Thing from the server.
 This method is non-blocking version of <deleteSynchronous:>

 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 It will delete bucket, topic which belongs to this thing, entity belongs to the bucket/topic and all ownership information of thing. This operation can not be reverted. Please carefully use this. This is a non-blocking method.
 
    [aThing delete:^(KiiThing *thing, NSError *error) {
        if(error == nil) {
            NSLog(@"Thing deleted!");
        }
    }];
 
 @param block The block to be called upon method completion. See example.
 */
- (void) delete:(KiiThingBlock)block;

/** Synchronously deletes the Thing from the server.
 This is a blocking method. 
 <br>This API is authorized by owner of thing.
 <br>Need user login who owns this thing before execute this API.
 <br>To let users to own Thing, please call <registerOwnerSynchronous:error:>

 - An error (error code: <[KiiError codeThingNotFound]>) will be returned if the thing is not found or already deleted.
 
 It will delete bucket, topic which belongs to this thing, entity belongs to the bucket/topic and all ownership information of thing. This operation can not be reverted. Please carefully use this.
 
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) deleteSynchronous:(NSError**)error;

#pragma mark - bucket, topic, push subscription

/** Get or create a bucket at the thing level
 
 @param bucketName The name of the bucket you'd like to use. It has to match the pattern ^[A-Za-z0-9_-]{1,64}$, that is letters, numbers, '-' and '_' and non-multibyte characters with a length between 1 and 64 characters.
 @return An instance of a working <KiiBucket>
 @exception NSInvalidArgumentException Thrown if bucketName is invalid. Please refers to parameter description for valid pattern.
 */
- (KiiBucket*) bucketWithName:(NSString*)bucketName;

/** Get or create an thing scope encrypted bucket.

 @param bucketName The name of the encrypted bucket you'd like to use.
 @return An instance of a working <KiiEncryptedBucket>
 @exception NSInvalidArgumentException when bucketName is not acceptable format. For details please refer to <[KiiBucket isValidBucketName:(NSString*) bucketName]>.
 */
- (KiiEncryptedBucket*) encryptedBucketWithName:(NSString *)bucketName;

/** Get or create a Push notification topic at the thing level
 
 @param topicName The name of the topic you'd like to use. It has to match the pattern ^[A-Za-z0-9_-]{1,64}$, that is letters, numbers, '-' and '_' and non-multibyte characters with a length between 1 and 64 characters.
 @return An instance of a working <KiiTopic>
 @exception NSInvalidArgumentException Thrown if topicName is invalid. Please refers to parameter description for valid pattern.
 */
- (KiiTopic*) topicWithName:(NSString*)topicName;

/** Get or create a push subscription for the thing.

 @return An instance of a working <KiiPushSubscription>
 */
- (KiiPushSubscription*) pushSubscription;

/**Returns the topics in this Thing scope. This is blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return  a <KiiListResult> object representing list of topics in this thing scope.
 */
- (KiiListResult*) listTopicsSynchronous:(NSError**) error;

/**Returns the topics in this thing scope. This is blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @param paginationKey pagination key. If nil or empty value is specified, this API regards no paginationKey specified.
 @return  a <KiiListResult> object representing list of topics in this thing scope.
 */
- (KiiListResult*) listTopicsSynchronous:(NSString*) paginationKey error:(NSError**) error;

/**Returns the topics in this thing scope asynchronously.

 Receives a <KiiListResult> object representing list of topics. This is a non-blocking request.

    [aThing listTopics:^(KiiListResult *topics, id callerObject, NSError *error){
       //at this scope, callerObject should be KiiThing instance
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

/**Returns the topics in this thing scope asynchronously.

 Receives a <KiiListResult> object representing list of topics. This is a non-blocking request.

    [aThing listTopics:paginationKey block:^(KiiListResult *topics, id callerObject, NSError *error){
       //at this scope, callerObject should be KiiThing instance
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
