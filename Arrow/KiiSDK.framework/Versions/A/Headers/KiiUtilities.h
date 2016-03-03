//
//  KiiUtilities.h
//  SampleApp
//
//  Created by Chris Beauchamp on 12/11/11.
//  Copyright (c) 2011 Kii Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface KiiUtilities : NSObject


+ (NSString *)urlEncode:(NSString*)string usingEncoding:(NSStringEncoding)encoding;
+ (UIImage*)generateThumbnail:(NSString*)filePath ofSize:(CGFloat)thumbSize;

+ (void) callMethod:(SEL)method onDelegate:(id)delegate withObjects:(id)firstObj, ...;
+ (void) performRequestMethod:(BOOL)async withBlock:(void (^)(void))block;

+ (void)performRequestMethodAsync:(void (^)(void))block;

+ (void)performRequestMethodSynchronous:(void (^)(void))block;

+ (NSString *)generateUUID;

+ (NSString *)currentTimeMillis;

+ (BOOL)isNilOrEmptyString:(NSString *)testString;

+ (BOOL)isNilOrNSNullOrEmptyString:(NSString *)testString;

+ (BOOL)isNilOrEmptyArray:(NSArray *)array;

+ (void) _callMethod:(SEL)method onDelegate:(id)delegate waitUntilDone : (BOOL) shouldWait withFirstObjects:(id)firstObj andArguments:(va_list)args;

+ (NSString*) mimeTypeForFileAtPath:(NSString*)path;

+ (NSDictionary *)readPropertyFile:(NSString *)plistFileName;

+(double) safeAddWithLeft:(double) left rigth:(double) right min:(double)max max:(double)min isRounded:(BOOL *) round;

+ (double) safeMultiplyWithLeft:(double) left rigth:(double) right min:(double)max max:(double)min isRounded:(BOOL *) round;

+ (double) safeCalculateExpiresAt:(long long) expirationInSeconds withDate: (NSDate*) dateTime;

+ (NSDate*) dateSafeCalculateExpiresAt:(long long) expirationInSeconds withDate: (NSDate*) dateTime;

+(double) safeMultiplyWithLeft:(double) left right:(double) right overflow:(int*) overflow;
+(double) safeAddWithLeft:(double) left right:(double) right overflow:(int*) overflow;

+ (NSDate*) milliSecToDate:(NSNumber*)milliSec;

+ (BOOL) convertFromContentType:(NSString*)contentType
                   toObjectType:(NSString**)outObjectType;

@end
