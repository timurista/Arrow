//
//  KADateRange.h
//  KiiAnalytics
//
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/** Use this class to create a date range for your analytics queries
 */
@interface KADateRange : NSObject

/** Create a range object using start/end dates
 
 @param startDate The beginning date to retrieve data for
 @param endDate The end date to retrieve data for
 @return A proper date range object with bounds using the given params
 */
+ (KADateRange*) rangeWithStart:(NSDate*)startDate andEnd:(NSDate*)endDate;

@end
