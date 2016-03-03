//
//  KiiThingOwner.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 12/18/14.
//  Copyright (c) 2014 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * A protocol to represent the owner of thing.
 */
@protocol KiiThingOwner <NSObject>
@required
/**An identifier of thing owner.
 If this instance is KiiUser, then this method returns "user:" +
 <[KiiUser userID]>.  If this instance is KiiGroup, then this method returns
 "group:" + <[KiiGroup groupID]>.
 @return "user:" + <[KiiUser userID]> or "group:" + <[KiiGroup groupID]> that own the thing.
 */
-(NSString*) thingOwnerID;
@end
