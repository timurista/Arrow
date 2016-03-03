//
//  KiiSocialAccountInfo.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 11/11/14.
//  Copyright (c) 2014 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiSocialConnect.h"

/**A class that represent social account information.
 */
@interface KiiSocialAccountInfo : NSObject

/** Provider enumeration.
 @return <KiiConnectorProvider> enumeration of linked social network provider.
 */
@property(nonatomic,readonly) KiiConnectorProvider provider;

/** Social account identifier.
 @return socialAccountId Idetifier given by social network provider.
 */
@property(nonatomic,readonly) NSString *socialAccountId;

/** The creation date time of social network integration.
 @return NSDate integration date time.
 */
@property(nonatomic,readonly) NSDate *createdAt;
@end
