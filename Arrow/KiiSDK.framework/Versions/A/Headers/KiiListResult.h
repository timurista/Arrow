//
//  KiiListResult.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 5/21/15.
//  Copyright (c) 2015 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
@class KiiListResult;
/**Block type for returning generic NSObject from the server, currently <KiiTopic> is supported.
 */
typedef void (^KiiListResultBlock)(KiiListResult *objects, id callerObject, NSError *error);
/** Class for holding data from the cloud.
 */
@interface KiiListResult : NSObject

/**Array of object obtained from the server. Currently, it is can be used to obtain <KiiTopic> instances of subscribable topic in particular scope.. It will return empty aray if there is no result.
 */
@property (nonatomic,readonly) NSArray* results;

/**Pagination key for result list. If hasNext value is NO, then paginationKey will always nil.
 */
@property (nonatomic,readonly) NSString* paginationKey;

/**Indication whether results has next colection. It returns YES if has next collection, NO otherwise.
 */
@property (nonatomic,readonly) BOOL hasNext;

@end
