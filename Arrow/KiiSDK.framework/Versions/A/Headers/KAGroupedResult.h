//
//  KAGroupedResult.h
//  KiiAnalytics
//
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/** The result container that holds the structured data when analytics are queried for
 */
@interface KAGroupedResult : NSObject

/** Get the snapshots from the resultset */
@property (readonly, strong) NSArray *snapshots;

@end
