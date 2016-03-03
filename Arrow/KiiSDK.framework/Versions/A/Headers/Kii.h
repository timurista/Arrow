//
//  Kii.h
//  SampleApp
//
//  Created by Chris Beauchamp on 12/11/11.
//  Copyright (c) 2011 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "KiiBucket.h"
#import "KiiError.h"
#import "KiiObject.h"
#import "KiiUser.h"
#import "KiiGroup.h"
#import "KiiFile.h"
#import "KiiFileBucket.h"
#import "KiiUtilities.h"
#import "KiiRequest.h"
#import "KiiClause.h"
#import "KiiQuery.h"
#import "KiiAnyAuthenticatedUser.h"
#import "KiiAnonymousUser.h"
#import "KiiACL.h"
#import "KiiACLEntry.h"
#import "KiiSocialConnect.h"
#import "KiiPushInstallation.h"
#import "KiiTopic.h"
#import "KiiPushMessage.h"
#import "KiiAPNSFields.h"
#import "KiiGCMFields.h"
#import "KiiRTransfer.h"
#import "KiiUploader.h"
#import "KiiRTransferManager.h"
#import "KiiResumableTransfer.h"
#import "KiiRTransferInfo.h"
#import "KiiDownloader.h"
#import "KiiBaseBucket.h"
#import "KiiServerCodeEntry.h"
#import "KiiServerCodeEntryArgument.h"
#import "KiiServerCodeExecResult.h"
#import "KiiExperiment.h"
#import "KiiVariation.h"
#import "KiiVariationSampler.h"
#import "KiiVariationSamplerByKiiUser.h"
#import "KiiRandomVariationSampler.h"
#import "KiiConversionEvent.h"
#import "KiiIllegalStateException.h"
#import "KiiGeoPoint.h"
#import "KiiPushSubscription.h"
#import "FileHolder.h"
#import "KiiCloudPhotoColle.h"
#import "KiiPhotoColleRTransferManager.h"
#import "KiiPhotoColleSocialConnect.h"
#import "KiiUserFields.h"
#import "KiiIdentityData.h"
#import "KiiIdentityDataBuilder.h"
#import "KiiSocialAccountInfo.h"
#import "KiiThing.h"
#import "KiiThingOwner.h"
#import "KiiThingFields.h"
#import "KiiBaseObject.h"
#import "KiiMQTTFields.h"
#import "KiiEncryptedBucket.h"
#import "KiiListResult.h"
#import "KiiAnalytics.h"


@class KiiFile, KiiUser, KiiBucket, KiiGroup;

#ifndef KII_SWIFT_ENVIRONMENT
/**
* This enum represents KiiCloud server location.
* This is used for <[Kii beginWithID:andKey:andSite:]>.
*/
typedef NS_ENUM(NSUInteger, KiiSite) {

    /** Use cloud in US. */
    kiiSiteUS = 0,
    /** Use cloud in Japan. */
    kiiSiteJP,
    /** Use cloud in China. */
    kiiSiteCN,
    /** Use cloud in Singapore. */
    kiiSiteSG,
    /** Use cloud in cn3 site of China. */
    kiiSiteCN3
};
#else
/**
 * This enum represents KiiCloud server location.
 * This is used for <[Kii beginWithID:andKey:andSite:]>.
 */
typedef NS_ENUM(NSUInteger, KiiSite) {
    /** Use cloud in US. */
    KiiUS,
    /** Use cloud in Japan. */
    KiiJP,
    /** Use cloud in China. */
    KiiCN,
    /** Use cloud in Singapore. */
    KiiSG,
    /** Use cloud in cn3 site of China. */
    KiiCN3
};
#endif


/** The main SDK class
 
 This class must be initialized on application launch using <beginWithID:andKey:>. This class also allows the application to make some high-level user calls and access some application-wide data at any time using static methods.
 */
@interface Kii : NSObject


/** Initialize the Kii SDK
 
 Defaults to the US deployment. Should reside in applicationDidFinishLaunching:withResult: 
 Meanwhile, Kii Analytics is initialized.
 @param appId The application ID found in your Kii developer console
 @param appKey The application key found in your Kii developer console
 */
+ (void) beginWithID:(NSString*)appId andKey:(NSString*)appKey;

/** Initialize the Kii SDK
 
 Should reside in applicationDidFinishLaunching:withResult:
 If Kii has provided a custom URL, use this initializer to set it.
 Meanwhile, Kii Analytics is initialized.
 @param appId The application ID found in your Kii developer console
 @param appKey The application key found in your Kii developer console
 @param kiiSite One of the enumerator constants kiiSiteUS (United States),
 kiiSiteJP (Japan), kiiSiteCN (China) or kiiSiteSG (Singapore) based
 on your desired location.
 */
+ (void) beginWithID:(NSString*)appId andKey:(NSString*)appKey andSite:(KiiSite)kiiSite;
+ (void) beginWithID:(NSString*)appId andKey:(NSString*)appKey andCustomURL:(NSString*)customURL;

/**
 Set the access token lifetime in seconds.
 
 If you don't call this method or call it with 0, token won't be expired.
 Call this method if you like the access token to be expired after a certain period.
 Once called, token retrieved by each authentication will have the specified lifetime.
 Note that, it will not update the lifetime of token received prior calling this method.
 Once expired, you have to login again to renew the token.
 @param expiresIn  The life time of access token in seconds.
 @exception throw NSInvalidArgumentException if expiresIn is negative value or greater than maximum long long.
 @exception throw KiiIllegalStateException if Kii initialization is not called.
 */
+(void) setAccessTokenExpiration:(long long) expiresIn;


/**
 Returns access token lifetime. If access token lifetime has not
 set explicitly by <setAccessTokenExpiration:>, returns 0.
 @return access token lifetime in seconds.
 @exception throw KiiIllegalStateException if Kii initialization is not called.
 */
+(long long) accessTokenExpiration;

/** Get or create a bucket at the application level
 
 @param bucketName The name of the bucket you'd like to use
 @return An instance of a working <KiiBucket>
 */
+ (KiiBucket*) bucketWithName:(NSString*)bucketName;

/** Get or create an encrypted bucket at the application level.
 
 @param bucketName The name of the encrypted bucket you'd like to use.
 @return An instance of a working <KiiEncryptedBucket>
 @exception NSInvalidArgumentException when bucketName is not acceptable format. For details please refer to <[KiiBucket isValidBucketName:(NSString*) bucketName]>.
 */
+ (KiiEncryptedBucket*) encryptedBucketWithName:(NSString*)bucketName;


/** Get a Topic at the application level
 Creation of App-scope topic is only supported by REST API calls, iOS SDK only has ability to get the app-scope topic object.
 
 @param topicName The name of the topic you'd like to use. It has to match the pattern ^[A-Za-z0-9_-]{1,64}$, that is letters, numbers, '-' and '_' and non-multibyte characters with a length between 1 and 64 characters.
 @return An instance of a working <KiiTopic>
 */
+ (KiiTopic*) topicWithName:(NSString*)topicName;

/** Kii SDK Build Number
 @return An NSString object representing the current build number of the SDK
 */
+ (NSString*) getBuildNumber;

/** Kii SDK Version Number
 @return An NSString object representing the current version number of the SDK
 */
+ (NSString*) getSDKVersion;


/** Creates a reference to a group with the given name
 
 If the group already exists, it should be be 'refreshed' to fill the data from the server
 @param groupName An application-specific group name
 @return a working <KiiGroup>
 */
+ (KiiGroup*) groupWithName:(NSString*)groupName;


/** Creates a reference to a group with the given name with default members
 
 If the group already exists, it should be be 'refreshed' to fill the data from the server
 @param groupName An application-specific group name
 @param members An array of members to automatically add to the group upon creation
 @return a working <KiiGroup>
 */
+ (KiiGroup*) groupWithName:(NSString*)groupName andMembers:(NSArray*)members;

+ (void) setLogLevel:(int)level;

/** Enable Kii APNS with APNS environment setting.
 @param isDevelopmentMode YES if APNS development environment mode or NO for production mode.
 @param types of ui remote notification type.
 @deprecated This method is deprecated. Use default iOS SDK <[UIApplication registerForRemoteNotificationTypes]> then call installation with <[KiiPushInstallation installSynchronousWithDeviceToken:andDevelopmentMode:andError:]>
 */
+(void)enableAPNSWithDevelopmentMode:(BOOL) isDevelopmentMode
               andNotificationTypes:(UIRemoteNotificationType) types __attribute__((deprecated("Use default iOS SDK [UIApplication registerForRemoteNotificationTypes] then call installation with [KiiPushInstallation installSynchronousWithDeviceToken:andDevelopmentMode:andError:]")));

/** Set APNS device token it is called on AppDelegate's didRegisterForRemoteNotificationsWithDeviceToken
 @param deviceToken device token that is given by APNS server.
 @deprecated This method is deprecated. Use <[KiiPushInstallation installSynchronousWithDeviceToken:andDevelopmentMode:andError:]> instead.
 */
+(void) setAPNSDeviceToken:(NSData*) deviceToken __attribute__((deprecated("Use [KiiPushInstallation installSynchronousWithDeviceToken:andDevelopmentMode:andError:]")));

/** Create KiiServerCodeEntry instance with the given entry name.
 @param entryName a specific entry name for this server code. Can not be nil and valid entryName pattern is "[a-zA-Z][_a-zA-Z0-9]*$"
 @return KiiServerCodeEntry instance.
 @exception NSInvalidArgumentException Thrown if given entryName is not valid.
 */
+(KiiServerCodeEntry*)serverCodeEntry:(NSString*) entryName;

/** Create KiiServerCodeEntry instance with the given entry name and version name.
 @param entryName a specific entry name for this server code. Can not be nil and valid entryName pattern is "[a-zA-Z][_a-zA-Z0-9]*$" .
 @param version a string that represent version of the server code, must not nil or empty.
 @return KiiServerCodeEntry instance.
 @exception NSInvalidArgumentException Thrown if given entryName is not valid or version is nil/empty.
 @exception NSInvalidArgumentException Thrown if given version is nil or empty.
 */
+(KiiServerCodeEntry*)serverCodeEntry:(NSString*) entryName withVersion:(NSString*) version;

/**
* URL of KiiApps Server.
* @return A NSString object representing the URL of KiiApps Server.
*/
+ (NSString *)kiiAppsBaseURL;

/**Returns the topics in this app scope. This is blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @return  a <KiiListResult> object representing list of topics in this app scope.
 */
+ (KiiListResult*) listTopicsSynchronous:(NSError**) error;

/**Returns the topics in this app scope. This is blocking method.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @param paginationKey pagination key. If nil or empty value is specified, this API regards no paginationKey specified.
 @return  a <KiiListResult> object representing list of topics in this app scope.
 */
+ (KiiListResult*) listTopicsSynchronous:(NSString*) paginationKey error:(NSError**) error;

/**Returns the topics in this App scope asynchronously.

 Receives a <KiiListResult> object representing list of topics. This is a non-blocking request.

    [Kii listTopics:^(KiiListResult *topics, id callerObject, NSError *error){
       //at this scope, callerObject will always be nil.
       NSLog(@"%@",callerObject);
       if(error == nil) {
            NSLog(@"Got Results: %@", topics);
            NSLog(@"Total topics: %@", topics.results.count);
            NSLog(@"Has Next: %@ next paginationKey: %@", topics.hasNext?@"Yes":@"No", topics.paginationKey);
            KiiTopic *firstTopic = topics.result.firstObject;
            if (firstTopic){
                NSLog(@"topic name :%@", firstTopic.name);
            }
       }
    }];

 @param completion The block to be called upon method completion, this is mandatory. See example.
 @exception NSInvalidArgumentException if completion is nil.
 */
+ (void) listTopics:(KiiListResultBlock) completion;

/**Returns the topics in this app scope asynchronously.

 Receives a <KiiListResult> object representing list of topics. This is a non-blocking request.

    [Kii listTopics:paginationKey block:^(KiiListResult *topics, id callerObject, NSError *error){
       //at this scope, callerObject will always be nil.
       NSLog(@"%@",callerObject);
       if(error == nil) {
            NSLog(@"Got Results: %@", topics);
            NSLog(@"Total topics: %@", topics.results.count);
            NSLog(@"Has Next: %@ next paginationKey: %@", topics.hasNext?@"Yes":@"No", topics.paginationKey);
            KiiTopic *firstTopic = topics.result.firstObject;
            if (firstTopic){
                NSLog(@"topic name :%@", firstTopic.name);
            }
       }
    }];

 @param paginationKey pagination key. If nil or empty value is specified, this API regards no paginationKey specified.
 @param completion The block to be called upon method completion, this is mandatory. See example.
 @exception NSInvalidArgumentException if completion is nil.
 */
+ (void) listTopics:(NSString*) paginationKey block:(KiiListResultBlock) completion;

@end
