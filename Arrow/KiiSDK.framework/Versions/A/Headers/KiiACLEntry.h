//
//  KiiACLEntry.h
//  KiiSDK-Private
//
//  Created by Chris Beauchamp on 6/11/12.
//  Copyright (c) 2012 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
* This enum represents ACL action.
*/
typedef NS_ENUM(NSUInteger, KiiACLAction) {
    /** Action of create object in the bucket. */
    KiiACLBucketActionCreateObjects,
    /** Action of query object in the bucket. */
    KiiACLBucketActionQueryObjects,
    /** Action of drop the bucket. It will also remove all contents in the bucket. */
    KiiACLBucketActionDropBucket,
    /** Action of read file in the bucket.
    * @deprecated This enum is deprecated. Use KiiACLObjectActionRead instead.
    */
    KiiACLFileActionRead,
    /** Action of modify, delete file in the bucket.
    * @deprecated This enum is deprecated. Use KiiACLObjectActionWrite instead.
    */
    KiiACLFileActionWrite,
    /** Action of read object in the bucket. */
    KiiACLObjectActionRead,
    /** Action of modify, delete object in the bucket. */
    KiiACLObjectActionWrite,
    /** Action of subscribe to the topic. */
    KiiACLTopicActionSubscribe,
    /** Action of send message to the topic. */
    KiiACLTopicActionSend
};

/** Entry of ACL.

 This KiiACLEntry is used to control permissions on an object, file or topic at a user or group level.
 */
@interface KiiACLEntry : NSObject 

/** The action that is being permitted/restricted.

 Possible values are defined in KiiACLAction.
 @see KiiACLAction
 */
@property (nonatomic, assign) KiiACLAction action; 

/** A <KiiUser>, <KiiGroup>, <KiiAnyAuthenticatedUser> or <KiiAnonymousUser> that is being permitted/restricted. */
@property (nonatomic, strong) id subject; 

/** When TRUE, the associated <action> is granted (enabled). When FALSE, the <action> is not granted (disabled). */
@property (nonatomic, assign) BOOL grant; 

// internal use
@property (readonly) BOOL updated;


/** Create a KiiACLEntry object with a <subject> and <action>
 
 The entry will not be applied on the server until the <KiiACL> object is explicitly saved. This method simply returns a working KiiACLEntry with a specified <subject> and <action>.
 @param subject A <KiiUser>, <KiiGroup>, <KiiAnyAuthenticatedUser> or <KiiAnonymousUser> object to which the action/grant is being applied.
 @param action One of the specified KiiACLAction values the permissions is being applied to.
 @return A KiiACLEntry object with the specified attributes. nil if the subject is not an accepted type.
 */
+ (KiiACLEntry*) entryWithSubject:(id)subject andAction:(KiiACLAction)action; 

@end
