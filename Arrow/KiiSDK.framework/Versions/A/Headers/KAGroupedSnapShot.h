//
//  KAGroupedSnapShot.h
//  KiiAnalytics
//
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>


/** Structured single snapshot from querying for analytics sets
 */
@interface KAGroupedSnapShot : NSObject

/** The data retrieved by the query */
@property (readonly, strong) NSArray *data;

/** The name of the dimension */
@property (readonly, strong) NSString *name;

/** The date (in milliseconds since UNIX epoch) which the data starts */
@property (readonly, assign) double pointStart;

/** The interval (in milliseconds) between data points */
@property (readonly, assign) double pointInterval;

@end
