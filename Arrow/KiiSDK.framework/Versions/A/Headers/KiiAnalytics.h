//
//  KiiAnalytics.h
//  KiiAnalytics
//
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "KAGroupedResult.h"
#import "KAGroupedSnapShot.h"
#import "KAResultQuery.h"
#import "KAFilter.h"
#import "KADateRange.h"

#ifndef KII_SWIFT_ENVIRONMENT
/**
 * This enum represents KiiCloud server location.
 */
typedef NS_ENUM(NSUInteger, KiiAnalyticsSite ) {

    /** Use cloud in US. */
    kiiAnalyticsSiteUS=0,
    /** Use cloud in Japan. */
    kiiAnalyticsSiteJP,
    /** Use cloud in China. */
    kiiAnalyticsSiteCN,
    /** Use cloud in Singapore. */
    kiiAnalyticsSiteSG,
    /** Use cloud in cn3 site of China. */
    kiiAnalyticsSiteCN3
};
#else
/**
 * This enum represents KiiCloud server location.
 */
typedef NS_ENUM(NSUInteger, KiiAnalyticsSite ) {
    /** Use cloud in US. */
    KiiAnalyticsUS,
    /** Use cloud in Japan. */
    KiiAnalyticsJP,
    /** Use cloud in China. */
    KiiAnalyticsCN,
    /** Use cloud in Singapore. */
    KiiAnalyticsSG
};
#endif



typedef void (^KAResultBlock)(KAGroupedResult *results, NSError *error);

/** The main SDK class
 
This class allows the application to make some high-level user calls and access some application-wide data at any time using static methods.
 */
@interface KiiAnalytics : NSObject

/** Log a single event to be uploaded to KiiAnalytics
 
 Use this method if you'd like to track an event by name only. If you'd like to track other attributes/dimensions, please use trackEvent:withExtras:
 Will return TRUE every time unless there was an error writing to the cache.
 @param event A string representing the event name for later tracking. This must not nil or empty, and length must be less than or equals 128bytes in UTF-8.
 @return TRUE if the event was added properly, FALSE otherwise
 */
+ (BOOL) trackEvent:(NSString*)event;


/** Log a single event to be uploaded to KiiAnalytics
 
 Use this method if you'd like to track an event by name and add extra information to the event.
 Will return TRUE every time unless there was an error writing to the cache OR if one of the extra key/value pairs was not JSON-encodable.
 @param event A string representing the event name for later tracking. This must not nil or empty, and length must be less than or equals 128bytes in UTF-8.
 @param extras A dictionary of JSON-encodable key/value pairs to be attached to the event
 @return TRUE if the event was added properly, FALSE otherwise
 */
+ (BOOL) trackEvent:(NSString *)event withExtras:(NSDictionary*)extras;

/** Get a result set of analytics based on a specific query.

 This method allows you to use analytics results within your application. This method is blocking.
 Here is a sample snippet that get analytics data and output it to console log.

 If you want to know more information of this analytics data,
 please see the GroupedResult item in [Flex analytics Guide](http://documentation.kii.com/en/guides/rest/managing-analytics/flex-analytics/analyze-application-data/)

    NSError *error = nil;
    KAGroupedResult *results = [KiiAnalytics getResultSynchronousWithID:@"my-aggregation-rule" andQuery:myQuery andError:&error];
    if (error != nil) {
        // There is something wrong...
        return;
    }
    NSLog(@"Got results: %@", results);

    // Get snapshots array
    NSArray *snapshotArray = results.snapshots;
    for (KAGroupedSnapShot *snapshot in snapshotArray) {
        // Get the label of the dimension/grouping key.
        NSLog(@"Name : %@", snapshot.name);
        // Get the date in which the data starts.
        NSLog(@"PointStart : %f", snapshot.pointStart);
        // Get interval between data points.
        NSLog(@"PointInterval : %f", snapshot.pointInterval);
        // Get the JSON Array that contains the data retrieved by the query.
        for (NSNumber *data in snapshot.data) {
            NSLog(@"Data : %@", data);
        }
    }


 @param aggregationRuleID The aggregation rule to slice data by
 @param query The query object to use for retrieving data
 @param error An NSError object, set to nil, to test for errors
 @return A result object with the segmented data broken into manageable data structures. Usually KAGroupedSnapShots.
 */
+ (KAGroupedResult*) getResultSynchronousWithID:(NSString*)aggregationRuleID
                                       andQuery:(KAResultQuery*)query
                                       andError:(NSError**)error;


/** Get a result set of analytics based on a specific query.

 This method allows you to use analytics results within your application. This method is non-blocking.
 Here is a sample snippet that get analytics data and output it to console log.

 If you want to know more information of this analytics data,
 please see the GroupedResult item in [Flex analytics Guide](http://documentation.kii.com/en/guides/rest/managing-analytics/flex-analytics/analyze-application-data/)

    [KiiAnalytics getResultWithID:@"my-aggregation-rule"
                         andQuery:myQuery
                         andBlock:^(KAGroupedResult *results, NSError *error) {
        if (error != nil) {
            // There is something wrong...
            return;
        }
        NSLog(@"Got results: %@", results);

        // Get snapshots array
        NSArray *snapshotArray = results.snapshots;
        for (KAGroupedSnapShot *snapshot in snapshotArray) {
            // Get the label of the dimension/grouping key.
            NSLog(@"Name : %@", snapshot.name);
            // Get the date in which the data starts.
            NSLog(@"PointStart : %f", snapshot.pointStart);
            // Get interval between data points.
            NSLog(@"PointInterval : %f", snapshot.pointInterval);
            // Get the JSON Array that contains the data retrieved by the query.
            for (NSNumber *data in snapshot.data) {
                NSLog(@"Data : %@", data);
            }
        }
 
 @param aggregationRuleID The aggregation rule to slice data by
 @param query The query object to use for retrieving data
 @param block The block to be called upon method completion. See example
 */
+ (void) getResultWithID:(NSString*)aggregationRuleID
                andQuery:(KAResultQuery*)query
                andBlock:(KAResultBlock)block;

+ (void) setLogLevel:(int)level;

@end
