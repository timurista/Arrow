//
//  KAGetResultError.h
//  KiiAnalytics
//
//  Created by Ryuji OCHI on 6/14/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/** GetResult API error that can be returned by KiiAnalyticsSDK */
@interface KAGetResultError : NSError

/** The GetResult API error code of invalid data error.
 @return The GetResult API error code of invalid data error.
*/
+(NSUInteger)invalidDataErrorCode;

/** The GetResult API error code of unauthorized error.
 @return The GetResult API error code of unauthorized error.
*/
+(NSUInteger)unauthorizedErrorCode;

/** The GetResult API error code of not found error.
 @return The GetResult API error code of not found error.
*/
+(NSUInteger)notFoundErrorCode;

/** The GetResult API error code of unexpected error.
 @return The GetResult API error code of unexpected error.
*/
+(NSUInteger)unexpectedErrorCode;

/** The domain name of getResult API error.
 @return The domain name of getResult API error. This method always return string of "com.kii.analytics.result".
*/
- (NSString *)domain;

/** The http status code of getResult API error.
@return The http status code of getResult API error.
*/
- (NSInteger)httpStatusCode;

/** The detailed error code of getResult API error.
@return The detailed error code of getResult API error.
*/
- (NSString *)detailedErrorCode;

/** The detailed message of getResult API error.
@return The detailed message of getResult API error.
*/
- (NSString *)detailedMessage;

@end
