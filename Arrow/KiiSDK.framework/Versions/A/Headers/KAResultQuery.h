//
//  KAResultQuery.h
//  KiiAnalytics
//
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

@class KADateRange, KAFilter;

/** Use this class to generate a query against the analytics data
 */
@interface KAResultQuery : NSObject

/** The grouping key associated with the query object.
 @exception NSException Named NSInvalidArgumentException is thrown if groupingKey is invalid. GroupingKey must match with pattern ^[a-zA-Z][a-zA-Z0-9_]{0,63}$
 */
@property(nonatomic, strong) NSString *groupingKey;

/** The filter associated with the query object.
 @exception NSException Named NSInvalidArgumentException is thrown if filter is nil or has no element.
 */
@property(nonatomic, strong) KAFilter *filter;

/** The date range for the query object.
 @exception NSException Named NSInvalidArgumentException is thrown if dateRange is nil.
 */
@property(nonatomic, strong) KADateRange *dateRange;

@end
