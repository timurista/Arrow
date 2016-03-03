//
//  KiiFileBucket.h
//  KiiSDK-Private
//
//  Created by Chris Beauchamp on 6/6/12.
//  Copyright (c) 2012 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiPushSubscription.h"
#import "KiiBaseBucket.h"

@class KiiFile, KiiUser, KiiQuery, KiiACL, KiiFileBucket, KiiRTransferManager;

typedef void (^KiiFileQueryResultBlock)(KiiQuery *query, KiiFileBucket *bucket, NSArray *results, NSError *error) __attribute__((deprecated("Use KiiQueryResultBlock instead.")));
typedef void (^KiiFileBucketBlock)(KiiFileBucket *bucket, NSError *error) __attribute__((deprecated("Use KiiBucketBlock instead.")));

/** A reference to a bucket within a user's scope which contains <KiiFile> objects
 @deprecated This class is deprecated. Use <KiiBucket> instead.
 */
__attribute__((deprecated("Use KiiBucket instead.")))
@interface KiiFileBucket : KiiBaseBucket <KiiSubscribable>


/** Get the ACL handle for this bucket. Any <KiiACLEntry> objects added or revoked from this ACL object will be appended to/removed from the server on ACL save.
 @deprecated This property is deprecated. Use <[KiiBucket bucketACL]> instead.
 */
@property (readonly) KiiACL *bucketACL __attribute__((deprecated));


/** Create a <KiiFile> within the current bucket based on the given local path
 
 The object will not be created on the server until the <KiiFile> is explicitly saved. This method returns a working <KiiFile> with local attributes pre-filled. For empty file creation, the -file method is also available.
 @param filePath The path of the file to use
 @return An empty <KiiObject> with the specified type
 @deprecated This method is deprecated.
 */
- (KiiFile*) fileWithLocalPath:(NSString*)filePath __attribute__((deprecated));

/** Create a <KiiFile> within the current bucket using the passed data
 
 The object will not be created on the server until the <KiiFile> is explicitly saved. This method returns a working <KiiFile> with local attributes pre-filled. For empty file creation, the -file method is also available.

 @param fileData The data for the file to use
 @return An empty <KiiObject> with the specified type
 @deprecated This method is deprecated.
 */
- (KiiFile*) fileWithData:(NSData*)fileData __attribute__((deprecated));


/** Create a <KiiFile> within the current bucket
 
 The file will not be created on the server until the <KiiFile> is explicitly saved. This method simply returns an empty working <KiiFile>.
 @return An empty <KiiFile>
 @deprecated This method is deprecated.
 */
- (KiiFile*) file __attribute__((deprecated));


/** Execute a query on the current bucket
 
 The query will be executed against the server, returning a result set. This is a blocking method
 
     [bucket executeQuery:query
                withBlock:^(KiiQuery *query, KiiFileBucket *bucket, NSArray *results, NSError *error) {
     
         if(error == nil) {
             // do something with the results
             NSLog(@"Got results: %@", results);
         }
     }];
 
 @param query The query to execute
 @param block The block to be called upon method completion. See example
 @deprecated This method is deprecated. Use <[KiiBucket executeQuery:withBlock:]> instead.
 */
- (void) executeQuery:(KiiQuery*)query withBlock:(KiiFileQueryResultBlock)block __attribute__((deprecated("Use [KiiBucket executeQuery:withBlock:] instead.")));


/** Execute a query on the current bucket
 
 The query will be executed against the server, returning a result set. This is a blocking method
 @param query The query to execute
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return An NSArray of objects returned by the query
 @deprecated This method is deprecated. Use <[KiiBucket executeQuerySynchronous:withError:andNext:]> instead.
 */
- (NSArray*) executeQuerySynchronous:(KiiQuery*)query withError:(NSError**)error __attribute__((deprecated("Use [KiiBucket executeQuerySynchronous:withError:andNext:] instead.")));


/** Execute a query on the current bucket
 
 The query will be executed against the server, returning a result set. This is a non-blocking method
 @param query The query to execute
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) queryFinished:(KiiQuery*)query onBucket:(KiiFileBucket*)bucket withResults:(NSArray*)results andError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
         
             // do something with the results
             for(KiiObject *o in results) {
                // use this object
             }
         }
         
         else {
            // there was a problem
         }
     }

 @deprecated This method is deprecated. Use <[KiiBucket executeQuery:withDelegate:andCallback:]> instead.
 */
- (void) executeQuery:(KiiQuery*)query withDelegate:(id)delegate andCallback:(SEL)callback __attribute__((deprecated("Use [KiiBucket executeQuery:withDelegate:andCallback:] instead.")));

/** Asynchronously deletes a bucket from the server.
 
 Delete a bucket from the server. This method is non-blocking.
 
     [b deleteWithBlock:^(KiiFileBucket *bucket, NSError *error) {
         if(error == nil) {
             NSLog(@"Bucket deleted!");
         }
     }];
 
 @param block The block to be called upon method completion. See example
 @deprecated This method is deprecated. Use <[KiiBucket deleteWithBlock:]> instead.
 */
- (void) deleteWithBlock:(KiiFileBucketBlock)block __attribute__((deprecated("Use [KiiBucket deleteWithBlock:] instead.")));

/** Synchronously deletes a file bucket from the server.
 
 Delete a file bucket from the server. This method is blocking.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @deprecated This method is deprecated. Use <[KiiBucket deleteSynchronous:]> instead.
 */
- (void) deleteSynchronous:(NSError**)error __attribute__((deprecated("Use [KiiBucket deleteSynchronous:] instead.")));


/** Asynchronously deletes a file bucket from the server.
 
 Delete a file bucket from the server. This method is non-blocking.
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) bucketDeleted:(KiiFileBucket*)bucket withError:(NSError*)error {
         
         // the request was successful
         if(error == nil) {
             // do something
         }
         
         else {
             // there was a problem
         }
     }

 @deprecated This method is deprecated. Use <[KiiBucket delete:withCallback:]> instead.
 */
- (void) delete:(id)delegate withCallback:(SEL)callback __attribute__((deprecated("Use [KiiBucket delete:withCallback:] instead.")));

/** Get transfer manager object based on this file bucket
 @return A transfer manager object based on this file bucket.
 @deprecated This method is deprecated. Use <[KiiBucket transferManager]> instead.
 */
- (KiiRTransferManager *) transferManager __attribute__((deprecated("Use [KiiBucket transferManager] instead.")));
@end
