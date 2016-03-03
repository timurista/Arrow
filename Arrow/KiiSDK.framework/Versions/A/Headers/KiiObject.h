//
//  KiiObject.h
//  KiiSDK-Private
//
//  Created by Chris Beauchamp on 1/5/12.
//  Copyright (c) 2012 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FileHolder.h"

@class KiiACL, KiiBucket, KiiACL, KiiObject,KiiUploader,KiiDownloader, KiiGeoPoint;
typedef void (^KiiObjectBlock)(KiiObject *object, NSError *error);

typedef void (^KiiObjectPublishBodyBlock)(KiiObject *obj, NSString *url, NSError *error);

typedef void (^KiiObjectBodyProgressBlock)(KiiObject *obj, NSUInteger completedSizeInBytes, NSUInteger totalSizeInBytes, NSError *error);

typedef void (^KiiObjectBodyCompletionBlock)(KiiObject *obj, NSError *error);

/** A server-compatible object for generic storage use cases */
@interface KiiObject : NSObject<FileHolder>

/** The unique id of the object, assigned by the server */
@property (readonly) NSString *uuid;

/** The date the object was created on the server */
@property (strong, readonly) NSDate *created;

/** The date the object was last modified on the server */
@property (strong, readonly) NSDate *modified;

/** Get a specifically formatted string referencing the object. The object must exist in the cloud (have a valid UUID). */
@property (strong, readonly) NSString *objectURI;

/** The application-defined class name of the object */
@property (strong, readonly) NSString *objectType;

/** Get the ACL handle for this file. Any <KiiACLEntry> objects added or revoked from this ACL object will be appended to/removed from the server on ACL save. */
@property (readonly) KiiACL *objectACL;

/** The bucket that owns this object */
@property (readonly) KiiBucket *bucket;

/** The content type of object body. nil by default, the value will be set automatically after successful upload/download object body operation. The value will be nullified after object deletion or object body deletion. */
@property(nonatomic,readonly) NSString* bodyContentType;

/** Create a KiiObject that references an existing object
 
 @param uri An object-specific URI
 @return a working KiiObject. nil is returned when the uri is invalid.
 */
+ (KiiObject*) objectWithURI:(NSString*)uri;


/** Retrieves a readable class name for this object 
 
 If the object is user-defined, it will return the class name it was initialized with. If it is a Kii subclass, it will return a readable string value of that class.
 @return A string value of this object's class name
 */
//- (NSString*) getClassName;


/** Sets a key/value pair to a KiiObject
 
 If the key already exists, its value will be written over. If the object is of invalid type, it will return false and an NSError will be thrown (quietly). Accepted types are any JSON-encodable objects.
 ***NOTE: Before involving floating point value, please consider using integer instead. For example, use percentage, permil, ppm, etc.***
 The reason is:
 - Will dramatically improve the performance of bucket query.
 - Bucket query does not support the mixed result of integer and floating point.
 ex.) If you use same key for integer and floating point and inquire object with the
 integer value, objects which has floating point value with the key would not be evaluated
 in the query. (and vice versa)
 @param object The value to be set. Object must be of a JSON-encodable type (Ex: NSDictionary, NSArray, NSString, NSNumber, etc)
 @param key The key to set. The key must not be a system key (created, metadata, modified, type, uuid) or begin with an underscore (_)
 @return True if the object was set, false otherwise.
 */
- (BOOL) setObject:(id)object forKey:(NSString*)key;

/**
  Set GeoPoint to this object with the specified key.
  @param point GeoPoint to be set to the specified key.
  @param key The key to set. The key must not be a system key (created, metadata, modified, type, uuid) or begin with an underscore (_)
 */
- (BOOL) setGeoPoint:(KiiGeoPoint*)point forKey:(NSString*)key;


/** Checks to see if an object exists for a given key
 
 @param key The key to check for existence
 @return True if the object exists, false otherwise.
 */
- (BOOL) hasObject:(NSString*)key;


/** Removes a specific key/value pair from the object
 If the key exists, the key/value will be removed from the object.  
 @param key The key of the key/value pair that will be removed.
 @note Since version 2.1.30, the behavior of this API has been changed. This method just removes the key-value pair from the local cache but no longer sets empty string (@"") to the key and does not send specified key-value pair to the cloud when the update method (<[KiiObject saveSynchronous:]> etc.) is called. If you want to have same effect as previous, please execute <setObject:forKey:> with empty string (@"") passed to the object explicitly.
 */
- (void) removeObjectForKey:(NSString*)key;


/** Gets the value associated with the given key
 
 @param key The key to retrieve
 @return An object if the key exists, null otherwise
 */
- (id) getObjectForKey:(NSString*)key;

/** Gets the GeoPoint associated with the given key
 
 @param key The key to retrieve
 @return An object if the key exists, null otherwise
 */
- (KiiGeoPoint*) getGeoPointForKey:(NSString*)key;


/** Asynchronously saves the latest object values to the server
 
 If the object does not yet exist, it will be created. If the object already exists, the fields that have changed locally will be updated accordingly. This is a non-blocking method.
 
     [obj saveWithBlock:^(KiiObject *object, NSError *error) {
         if(error == nil) {
             NSLog(@"Object saved: %@", object);
         }
     }];
 
 @param block The block to be called upon method completion. See example 
 @note This API can not create new KiiObject on cloud when instantiated by <[KiiBucket createObjectWithID:]>, but can only update.
 If you want to create new KiiObject with it, please use <saveAllFieldsSynchronous:withError:>, <saveAllFields:withBlock:> or <saveAllFields:withDelegate:andCallback:> instead.
*/
- (void) saveWithBlock:(KiiObjectBlock)block;


/** Asynchronously saves the latest object values to the server 
 
 If the object does not yet exist, it will be created. If the object already exists, the fields that have changed locally will be updated accordingly. This is a non-blocking method.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) objectSaved:(KiiObject*)object withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the object
         }
         
         else {
             // there was a problem
         }
     }

 @note This API can not create new KiiObject on cloud when instantiated by <[KiiBucket createObjectWithID:]>, but can only update.
 If you want to create new KiiObject with it, please use <saveAllFieldsSynchronous:withError:>, <saveAllFields:withBlock:> or <saveAllFields:withDelegate:andCallback:> instead.
 */
- (void) save:(id)delegate withCallback:(SEL)callback;


/** Synchronously saves the latest object values to the server 
 
 If the object does not yet exist, it will be created. If the object already exists, the fields that have changed locally will be updated accordingly. This is a blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @note This API can not create new KiiObject on cloud when instantiated by <[KiiBucket createObjectWithID:]>, but can only update.
 If you want to create new KiiObject with it, please use <saveAllFieldsSynchronous:withError:>, <saveAllFields:withBlock:> or <saveAllFields:withDelegate:andCallback:> instead.
 */
- (void) saveSynchronous:(NSError**)error;


/** Asynchronously saves the latest object values to the server
 
 If the object does not yet exist, it will be created. If the object already exists and forced is set to TRUE, all fields on the server will be replaced by the fields defined locally. Otherwise, only changed fields will be modified. This is a non-blocking method.
 
     [obj save:TRUE withBlock:^(KiiObject *object, NSError *error) {
         if(error == nil) {
             NSLog(@"Object saved: %@", object);
         }
     }];
 
 @param forced Set to TRUE if the local copy should overwrite the remote copy, even if the remote copy is newer. Set to FALSE otherwise.
 @param block The block to be called upon method completion. See example
 @note This API can not create new KiiObject on cloud when instantiated by <[KiiBucket createObjectWithID:]>, but can only update.
 If you want to create new KiiObject with it, please use <saveAllFieldsSynchronous:withError:>, <saveAllFields:withBlock:> or <saveAllFields:withDelegate:andCallback:> instead.
*/
- (void) save:(BOOL)forced withBlock:(KiiObjectBlock)block;


/** Asynchronously saves the latest object values to the server
 
 If the object does not yet exist, it will be created. If the object already exists and forced is set to TRUE, all fields on the server will be replaced by the fields defined locally. Otherwise, only changed fields will be modified. This is a non-blocking method.
 @param forced Set to TRUE if the local copy should overwrite the remote copy, even if the remote copy is newer. Set to FALSE otherwise.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
    - (void) objectSaved:(KiiObject*)object withError:(NSError*)error {
 
        // the request was successful
        if(error == nil) {
            // do something with the object
        }
 
        else {
            // there was a problem
        }
    }

 @note This API can not create new KiiObject on cloud when instantiated by <[KiiBucket createObjectWithID:]>, but can only update.
 If you want to create new KiiObject with it, please use <saveAllFieldsSynchronous:withError:>, <saveAllFields:withBlock:> or <saveAllFields:withDelegate:andCallback:> instead.
 */
- (void) save:(BOOL)forced withDelegate:(id)delegate andCallback:(SEL)callback;


/** Synchronously saves the latest object values to the server
 
 If the object does not yet exist, it will be created. If the object already exists and forced is set to TRUE, all fields on the server will be replaced by the fields defined locally. Otherwise, only changed fields will be modified. This is a blocking method.
 @param forced Set to TRUE if the local copy should overwrite the remote copy, even if the remote copy is newer. Set to FALSE otherwise.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @note This API can not create new KiiObject on cloud when instantiated by <[KiiBucket createObjectWithID:]>, but can only update.
 If you want to create new KiiObject with it, please use <saveAllFieldsSynchronous:withError:>, <saveAllFields:withBlock:> or <saveAllFields:withDelegate:andCallback:> instead.
 */
- (void) saveSynchronous:(BOOL)forced withError:(NSError**)error;


/** Asynchronously saves the latest object values to the server
 
 If the object does not yet exist, it will be created. If the object already exists, all fields will be removed or changed to match the local values. This is a non-blocking method.
 
     [obj save:TRUE withBlock:^(KiiObject *object, NSError *error) {
         if(error == nil) {
             NSLog(@"Object saved: %@", object);
         }
     }];

 @param forced Set to TRUE if the local copy should overwrite the remote copy, even if the remote copy is newer. Set to FALSE otherwise.
 @param block The block to be called upon method completion. See example
*/
- (void) saveAllFields:(BOOL)forced withBlock:(KiiObjectBlock)block;


/** Asynchronously saves the latest object values to the server
 
 If the object does not yet exist, it will be created. If the object already exists, all fields will be removed or changed to match the local values. This is a non-blocking method.
 @param forced Set to TRUE if the local copy should overwrite the remote copy, even if the remote copy is newer. Set to FALSE otherwise.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
     
     - (void) objectSaved:(KiiObject*)object withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the object
         }
         
         else {
             // there was a problem
         }
     }
 
 */
- (void) saveAllFields:(BOOL)forced withDelegate:(id)delegate andCallback:(SEL)callback;


/** Synchronously saves the latest object values to the server
 
 If the object does not yet exist, it will be created. If the object already exists, all fields will be removed or changed to match the local values. This is a blocking method.
 @param forced Set to TRUE if the local copy should overwrite the remote copy, even if the remote copy is newer. Set to FALSE otherwise.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) saveAllFieldsSynchronous:(BOOL)forced withError:(NSError**)error;

/** Asynchronously updates the local object's data with the object data on the server
 
 The object must exist on the server. Local data will be overwritten.
     
     [obj refreshWithBlock:^(KiiObject *object, NSError *error) {
         if(error == nil) {
             NSLog(@"Object refreshed: %@", object);
         }
     }];
     
 @param block The block to be called upon method completion. See example
*/
- (void) refreshWithBlock:(KiiObjectBlock)block;

/** Asynchronously updates the local object's data with the object data on the server
 
 The object must exist on the server. Local data will be overwritten.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) objectRefreshed:(KiiObject*)object withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something with the object
         }
         
         else {
             // there was a problem
         }
     }
 
 */
- (void) refresh:(id)delegate withCallback:(SEL)callback;


/** Synchronously updates the local object's data with the object data on the server
 
 The object must exist on the server. Local data will be overwritten. This is a blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) refreshSynchronous:(NSError**)error;

/** Asynchronously deletes an object from the server.
 
 Delete an object from the server. This method is non-blocking.
 
     [obj deleteWithBlock:^(KiiObject *object, NSError *error) {
         if(error == nil) {
             NSLog(@"Object deleted!");
         }
     }];
 
 @param block The block to be called upon method completion. See example
*/
- (void) deleteWithBlock:(KiiObjectBlock)block;


/** Asynchronously deletes an object from the server.
 
 Delete an object from the server. This method is non-blocking.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) objectDeleted:(KiiObject*)object withError:(NSError*)error {
         
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


/** Synchronously deletes an object from the server.
 
 Delete an object from the server. This method is blocking.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) deleteSynchronous:(NSError**)error;

/** Synchronously deletes an object's body from the server.
 
 Delete an object's body from the server. This method is blocking.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 */
- (void) deleteBodySynchronous:(NSError**)error;

/** Asynchronously deletes an object's body from the server.
 
 Delete an object's body from the server. This method is non-blocking.
 
     [obj deleteBodyWithBlock:^(KiiObject *object, NSError *error) {
         if(error == nil) {
            NSLog(@"Object's body deleted!");
         }
     }];
 
 @param block The block to be called upon method completion. See example
 */
- (void) deleteBodyWithBlock:(KiiObjectBlock)block;

/** Gets a dictionary value of all attributes of this object including read only value obtained from server response */
- (NSDictionary*) dictionaryValue;

/** Prints the contents of this object to log
 
 For developer purposes only, this method prints the object in a readable format to the log for testing.
 */
- (void) describe;

///---------------------------------------------------------------------------------------
/// @name Object Body Handling
///---------------------------------------------------------------------------------------

/** Synchronously Publish the KiiObject body and return the public URL. URL will not be expired.
 
 This is blocking method.
 URL of published object body refers to the original object body, any updates on original will reflect.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the object does not have body, an error (Error code : 517) will be returned.
 
 @param error An NSError object, set to nil to test for errors.
 @return the URL for the KiiObject body contents.
 */
- (NSString*) publishBodySynchronous: (NSError**) error;

/** Asynchronously Publish the KiiObject body and return the public URL. URL will not be expired.
 
 This is non blocking method. 
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the object does not have body, an error (Error code : 517) will be returned.
 
        [object publishBodyWithBlock:^(KiiObject *object, NSString *url, NSError *error) {
             if (error){
             // error handling;
             }else{
             NSLog(@"Published URL : %@",url);
             //do something with the URL
            }
        }];
 
 @param block completion block.
 @param error An NSError object, set to nil to test for errors.
 @exception NSInvalidArgumentException will be thrown if completion block is nil.
 */
- (void) publishBodyWithBlock:(KiiObjectPublishBodyBlock) block;


/** Synchronously Publish the KiiObject body and return the public URL. URL will be expired at the specified expiration date.
 
 This is blocking method.
 URL of published object body refers to the original object body, any updates on original will reflect.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the object does not have body, an error (Error code : 517) will be returned.
 
 If the expirationDate is not future date, an error (Error code : 521) will be returned.
 
 @param expirationDate defined expiration date.
 @param error An NSError object, set to nil to test for errors.
 @return the URL for the KiiObject body contents.
 */
- (NSString*) publishBodySynchronousExpiresAt:(NSDate*) expirationDate andError: (NSError**) error;

/** Asynchronously Publish the KiiObject attached file and return the file URL. URL will be expired at the specified expiration date.
 
 This is non blocking method.
 URL of published object body refers to the original object body, any updates on original will reflect.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the object does not have body, an error (Error code : 517) will be returned.
 
 If the expirationDate is not future date, an error (Error code : 521) will be returned.
 
        [object publishBodyExpiresAt:expirationDate andBlock:^(KiiObject *obj, NSString *url, NSError *error) {
            if (error){
            // error handling;
            }else{
            NSLog(@"Published URL : %@",url);
            //do something with the URL
            }
        }];
 
 @param expirationDate defined expiration date.
 @param block completion block. See code snippet above.
 @return the URL for the KiiObject body contents.
 @exception NSInvalidArgumentException will be thrown if completion block is nil.
 */
- (void) publishBodyExpiresAt:(NSDate*) expirationDate withBlock:(KiiObjectPublishBodyBlock) block;

/** Synchronously Publish the KiiObject body and return the public URL. URL will be expired in the specified time interval since now.
 
 This is blocking method.
 URL of published object body refers to the original object body, any updates on original will reflect.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the object does not have body, an error (Error code : 517) will be returned.
 
 If the timeInterval is 0, an error (Error code : 522) will be returned.

 @param timeInterval NSUInteger time interval since now (in seconds).
 @param error An NSError object, set to nil to test for errors.
 @return the URL for the KiiObject body contents.
 */
- (NSString*) publishBodySynchronousExpiresIn:(NSUInteger) timeInterval andError: (NSError**) error;

/** Asynchronously Publish the KiiObject body and return the public URL. URL will be expired in time interval since now.
 
 This is non blocking method. 
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the object does not have body, an error (Error code : 517) will be returned.
 
 If the file url path is not writable, an error (Error code : 519) will be returned.
 
 If the timeInterval is 0, an error (Error code : 522) will be returned.
 
        NSUinteger nextOneHour = 60 * 60;
        [object publishBodyExpiresIn:nextOneHour andBlock:^(KiiObject *obj,NSString *url, NSError *error) {
            if (error){
            // error handling;
            }else{
            NSLog(@"Published URL : %@",url);
            //do something with the URL
            }
        }];
 
 @param timeInterval NSUInteger time interval since now (in seconds).
 @param completion block.
 @exception NSInvalidArgumentException will be thrown if completion block is nil.
 */
- (void) publishBodyExpiresIn:(NSUInteger) timeInterval withBlock:(KiiObjectPublishBodyBlock) block;

/** Synchronously download object body.
 
 This is blocking method. If destination file exist, it will be overwriten.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the object does not have body, an error (Error code : 517) will be returned.
 
 If the file url path is not writable, an error (Error code : 519) will be returned.
 
 @param destinationFileURL url for destination file, it should have 'file://' scheme.
 @param error An NSError object, set to nil to test for errors.
 */
- (void) downloadBodySynchronousWithURL:(NSURL*) destinationFileURL andError:(NSError**) error;

/** Asynchronously download object body with progress and completion.
 
 This is non blocking method. If destination file exist, it will be overwriten.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the object does not have body, an error (Error code : 517) will be returned.
 
 If the file url path is not writable, an error (Error code : 519) will be returned.

        [object downloadBodyWithURL:destinationFileURL
                       andCompletion:^(KiiObject *obj, NSError *error)
        {
            // do something during download progress
        }
        andProgess:^(KiiObject *obj, NSUInteger completedSizeInBytes, NSUInteger totalSizeInBytes, NSError *error)
        {
            // do something on completion
        }];
 
 @param destinationFileURL url for destination file, it should have 'file://' scheme.
 @param completion block for handling completion.
 @param progress block for handling progress.
 */
- (void) downloadBodyWithURL:(NSURL*) destinationFileURL andCompletion:(KiiObjectBodyCompletionBlock) completion andProgess:(KiiObjectBodyProgressBlock) progess ;

/** Asynchronously download object body with completion.
 
 This is non blocking method. If destination file exist, it will be overwriten.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the object does not have body, an error (Error code : 517) will be returned.
 
 If the file url path is not writable, an error (Error code : 519) will be returned.
 
        [object downloadBodyWithURL:destinationFileURL
                       andCompletion:^(KiiObject *obj, NSError *error)
        {
            // do something on completion
        }];
 
 @param destinationFileURL url for destination file, it should have 'file://' scheme.
 @param completion block for handling completion. 
 */
- (void) downloadBodyWithURL:(NSURL*) destinationFileURL andCompletion:(KiiObjectBodyCompletionBlock) completion;

/** Generate NSURLRequest instance for downloading object body.
 
 The generated request can be used to implement iOS 7 background transfer feature.
 
 If the object does not have id, nil will be returned.
 
 If the object body does not exist, it fails on execution.
 
 Note that refresh token won't be executed even if the login users access token
 is going to expired.<br>

 @return NSURLRequest instance to download object body.
 */
- (NSURLRequest*) generateDownloadRequest;

/** Synchronously upload object body.
 
 This is blocking method.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the file url path is not exist or is a directory, an error (Error code : 518) will be returned.
 
 If the file url path is not readable, an error (Error code : 520) will be returned.

 @param sourceFileURL url for source file, it should have 'file://' scheme.
 @param contentType Content type of the object body. Please refer to http://www.iana.org/assignments/media-types/media-types.xhtml for the standard. If nil is passed, it will be parsed from URL's file extension.
 @param error An NSError object, set to nil to test for errors.
 @note After this operation, KiiObject version on cloud will be updated. If you want to use <saveSynchronous:withError:> or <saveAllFieldsSynchronous:withError:> with overwrite=NO argument, please do <refreshSynchronous:> before saving.
 */
- (void) uploadBodySynchronousWithURL:(NSURL*) sourceFileURL andContentType: (NSString*) contentType andError:(NSError**) error;

/** Synchronously upload object body.
 
 This is blocking method.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 @param data NSData to upload. This is mandatory, can not be nil. An invalid parameter exception will be thrown if nil is passed.
 @param contentType Content type of the object body. Please refer to http://www.iana.org/assignments/media-types/media-types.xhtml for the standard. If nil is passed, it will be set as "application/octet-stream".
 @param error An NSError object, set to nil to test for errors.
 @exception NSInvalidArgumentException if data is nil.
 @note After this operation, KiiObject version on cloud will be updated. If you want to use <saveSynchronous:withError:> or <saveAllFieldsSynchronous:withError:> with overwrite=NO argument, please do <refreshSynchronous:> before saving.
 */
- (void) uploadBodySynchronousWithData:(NSData*) data andContentType: (NSString*) contentType andError:(NSError**) error;

/** Asynchronously upload object body with progress and completion.
 
 This is non blocking method.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the file url path is not exist or is a directory, an error (Error code : 518) will be returned.
 
 If the file url path is not readable, an error (Error code : 520) will be returned.
 
        [object uploadBodyWithURL:sourceFileURL
    andCompletion:^(KiiObject *obj, NSError *error)
            {
            // do something during download progress
            }
    andProgess:^(KiiObject *obj, NSUInteger completedSizeInBytes, NSUInteger totalSizeInBytes, NSError *error)
            {
            // do something on completion
        }];
 
 @param sourceFileURL url for source file, it should have 'file://' scheme.
 @param contentType Content type of the object body. Please refer to http://www.iana.org/assignments/media-types/media-types.xhtml for the standard. If nil is passed, it will be set as "application/octet-stream".
 @param completion block for handling completion.
 @param contentType Content type of the object body. Please refer to http://www.iana.org/assignments/media-types/media-types.xhtml for the standard. If nil is passed, it will be parsed from URL's file extension.
 @param progress block for handling progress.
 @note After this operation, KiiObject version on cloud will be updated. If you want to use <saveSynchronous:withError:> or <saveAllFieldsSynchronous:withError:> with overwrite=NO argument, please do <refreshSynchronous:> before saving.
 */
- (void) uploadBodyWithURL:(NSURL*) sourceFileURL andContentType: (NSString*) contentType andCompletion:(KiiObjectBodyCompletionBlock) completion andProgess:(KiiObjectBodyProgressBlock) progess;

/** Asynchronously upload object body with progress and completion.
 
 This is non blocking method.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
         [object uploadBodyWithData:data
             andCompletion:^(KiiObject *obj, NSError *error)
             {
             // do something during download progress
             }
             andProgess:^(KiiObject *obj, NSUInteger completedSizeInBytes, NSUInteger totalSizeInBytes, NSError *error)
             {
             // do something on completion
         }];
 
 @param data NSData to upload. This is mandatory, can not be nil. An invalid parameter exception will be thrown if nil is passed.
 @param contentType Content type of the object body. Please refer to http://www.iana.org/assignments/media-types/media-types.xhtml for the standard. If nil is passed, it will be set as "application/octet-stream".
 @param completion block for handling completion.
 @param progress block for handling progress.
 @exception NSInvalidArgumentException if data is nil.
 @note After this operation, KiiObject version on cloud will be updated. If you want to use <saveSynchronous:withError:> or <saveAllFieldsSynchronous:withError:> with overwrite=NO argument, please do <refreshSynchronous:> before saving.
 */
- (void) uploadBodyWithData:(NSData*) data andContentType: (NSString*) contentType andCompletion:(KiiObjectBodyCompletionBlock) completion andProgess:(KiiObjectBodyProgressBlock) progess;

/** Asynchronously upload object body with completion.
 
 This is non blocking method.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
 If the file url path is not exist or is a directory, an error (Error code : 518) will be returned.

 If the file url path is not readable, an error (Error code : 520) will be returned.
 
        [object uploadBodyWithURL:sourceFileURL
            andCompletion:^(KiiObject *obj, NSError *error)
            {
            // do something on completion
        }];
 
 @param sourceFileURL url for source file, it should have 'file://' scheme.
 @param contentType Content type of the object body. Please refer to http://www.iana.org/assignments/media-types/media-types.xhtml for the standard. If nil is passed, it will be parsed from URL's file extension.
 @param completion block for handling completion.
 @note After this operation, KiiObject version on cloud will be updated. If you want to use <saveSynchronous:withError:> or <saveAllFieldsSynchronous:withError:> with overwrite=NO argument, please do <refreshSynchronous:> before saving.
 */
- (void) uploadBodyWithURL:(NSURL*) sourceFileURL andContentType: (NSString*) contentType andCompletion:(KiiObjectBodyCompletionBlock) completion;

/** Asynchronously upload object body with completion.
 
 This is non blocking method.
 
 If the object does not have id, an error (Error code : 501) will be returned.
 
 If the object is not found on the server, an error (Error code : 510) will be returned.
 
        [object uploadBodyWithData:data
                     andCompletion:^(KiiObject *obj, NSError *error)
                    {
                    // do something during download progress
        }];
 
 @param data NSData to upload. This is mandatory, can not be nil. An invalid parameter exception will be thrown if nil is passed.
 @param contentType Content type of the object body. Please refer to http://www.iana.org/assignments/media-types/media-types.xhtml for the standard. If nil is passed, it will be set as "application/octet-stream".
 @param completion block for handling completion.
 @exception NSInvalidArgumentException if data is nil.
 @note After this operation, KiiObject version on cloud will be updated. If you want to use <saveSynchronous:withError:> or <saveAllFieldsSynchronous:withError:> with overwrite=NO argument, please do <refreshSynchronous:> before saving.
 */
- (void) uploadBodyWithData:(NSData*) data andContentType: (NSString*) contentType andCompletion:(KiiObjectBodyCompletionBlock) completion;

/** Generate NSURLRequest instance for uploading object body.
 
 The generated request can be used to implement iOS 7 background transfer feature.
 
 If the object does not have id, nil will be returned.
 
 If the object does not exist, it fails on execution.

 Note that refresh token won't be executed even if the login users access token
 is going to expired.<br>

 @return NSURLRequest instance to upload object body.
 @note If you upload object body with this method, object body content-type value is set always "application/octet-stream". If you want to set object body content type, please use <[KiiObject generateUploadRequest:]> instead.
 */
- (NSURLRequest*) generateUploadRequest;

/** Generate NSURLRequest instance for uploading object body with specified content type.

 The generated request can be used to implement iOS 7 background transfer feature.

 If the object does not have id, nil will be returned. If the object does not exist, it fails on execution.

 Note that refresh token won't be executed even if the login users access token
 is going to expired.<br>

 @param contentType Content type of the object body. Please refer to [http://www.iana.org/assignments/media-types/media-types.xhtml](http://www.iana.org/assignments/media-types/media-types.xhtml) for the standard. If nil is passed, it will be set as "application/octet-stream".
 @return NSURLRequest instance to upload object body.
 */
- (NSURLRequest*) generateUploadRequest:(NSString *)contentType;

///---------------------------------------------------------------------------------------
/// @name Resumable Transfer Handling
///---------------------------------------------------------------------------------------

/**
 Get uploader. If there is no uploader in the app, it will be created new instance
 @param localPath Path that will be used by the uploader.
 @return A KiiUploader instance associated to this object
 */
-(KiiUploader*) uploader : (NSString*) localPath;

/**
 Get downloader. If there is no downloader in the app, it will be created new instance
 @param localPath Path that will be used by the downloader. If file exists, will be overwritten.
 @return A KiiDownloader instance associated to this object
 */
-(KiiDownloader*) downloader : (NSString*) localPath;
@end
