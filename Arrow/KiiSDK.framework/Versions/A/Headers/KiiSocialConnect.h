//
//  KiiSocialConnect.h
//  KiiSDK-Private
//
//  Created by Chris Beauchamp on 7/3/12.
//  Copyright (c) 2012 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class KiiSocialConnectNetwork;
@class KiiSCNFacebook;
@class KiiUser;
@class KiiSCNQQ;

#ifndef KII_SWIFT_ENVIRONMENT
/**
* This enum represents social networks identifier.
*/
typedef NS_ENUM(NSUInteger, KiiSocialNetworkName) {
    /** Use Facebook */
    kiiSCNFacebook = 100,
    /** Use Twitter */
    kiiSCNTwitter = 101,
    /** Use QQ */
    kiiSCNQQ = 102,
    /** Use Kii Social Network Connect */
    kiiSCNConnector = 103
};
#else
/**
 * This enum represents social networks identifier.
 */
typedef NS_ENUM(NSUInteger, KiiSocialNetworkName) {
    /** Use Facebook */
    SCNFacebook = 100,
    /** Use Twitter */
    SCNTwitter = 101,
    /** Use QQ*/
    SCNQQ = 103,
    /** Use Kii Social Network Connect */
    SCNConnector = 104
};
#endif
    

#ifndef KII_SWIFT_ENVIRONMENT
/**
 * This enum represents social network that is supported by Kii Social Network Connector
 */
typedef NS_ENUM(NSUInteger, KiiConnectorProvider) {
    /** Use Facebook to authenticate */
    kiiConnectorFacebook,
    /** Use Twitter to authenticate */
    kiiConnectorTwitter,
    /** Use LinkedIn to authenticate */
    kiiConnectorLinkedIn,
    /** Use Yahoo to authenticate */
    kiiConnectorYahoo,
    /** Use Google to authenticate @deprecated Please use kiiConnectorGoogleplus instead*/
    kiiConnectorGoogle,
    /** Use Dropbox to authenticate */
    kiiConnectorDropbox,
    /** Use Box to authenticate */
    kiiConnectorBox,
    /** Use Renren to authenticate */
    kiiConnectorRenren,
    /** Use Sina Weibo to authenticate */
    kiiConnectorSina,
    /** Use Live to authenticate */
    kiiConnectorLive,
    /** Use QQ to authenticate. */
    kiiConnectorQQ,
    /** Use Googleplus to authenticate. */
    kiiConnectorGoogleplus,
    /** Use Open ID provider configured for the App.
     * It is configurable only with Kii Cloud Enterprise subscription.
     */
    kiiConnectorOpenIDConnectSimple,
    /** Use Kii to authenticate. */
    kiiConnectorKii
};
#else
/**
 * This enum represents social network that is supported by Kii Social Network Connector
 */
typedef NS_ENUM(NSUInteger, KiiConnectorProvider) {
    /** Use Facebook to authenticate */
    KiiFacebook,
    /** Use Twitter to authenticate */
    KiiTwitter,
    /** Use LinkedIn to authenticate */
    KiiLinkedIn,
    /** Use Yahoo to authenticate */
    KiiYahoo,
    /** Use Google to authenticate */
    KiiGoogle __attribute__((deprecated("Please use kiiConnectorGoogleplus instead"))),
    /** Use Dropbox to authenticate */
    KiiDropbox,
    /** Use Box to authenticate */
    KiiBox,
    /** Use Renren to authenticate */
    KiiRenren,
    /** Use Sina Weibo to authenticate */
    KiiSina,
    /** Use Live to authenticate */
    KiiLive,
    /** Use QQ to authenticate */
    KiiQQ,
    /** Use Googleplus to authenticate. */
    KiiGoogleplus,
    /** Use Open ID provider configured for the App.
     * It is configurable only with Kii Cloud Enterprise subscription.
     */
    KiiOpenIDConnectSimple,
    /** Use Kii to authenticate. */
    KiiKii
};
#endif


/**
 * The block to be called upon method completion.
 */
typedef void (^KiiSocialConnectBlock)(KiiUser *user, KiiSocialNetworkName name, NSError *error);

/**
 * The block to be called upon method completion.
 */
typedef void (^KiiSCNBlock)(KiiUser *user, KiiConnectorProvider provider, NSError *error);

/** An interface to link users to social networks
 
 The SDK currently support the following social networks :
  
 1. Facebook 
 2. Twitter 
 3. LinkedIn
 4. Yahoo
 5. Dropbox
 6. Box
 7. Renren
 8. Sina Weibo
 9. Microsoft Live
 10. QQ
 11. Googleplus
 12. OpenID Connect Simple
*/
@interface KiiSocialConnect : NSObject;


/** Required method by KiiSocialNetwork
 
 This method must be placed in your AppDelegate file in order for the SNS to properly authenticate with KiiSocialConnect:

    // Pre iOS 4.2 support
    - (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
        return [KiiSocialConnect handleOpenURL:url];
    }
 
    // For iOS 4.2+ support
    - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url
                                           sourceApplication:(NSString *)sourceApplication 
                                                  annotation:(id)annotation {
        return [KiiSocialConnect handleOpenURL:url];
    }

@param url The URL that is returned by Facebook authentication through delegate.
@deprecated Not usable on new social connect login mechanism from v2.2.1.
 */
+ (BOOL) handleOpenURL:(NSURL*)url __attribute__((deprecated("Not usable on new social connect login mechanism from v2.2.1")));


/** Set up a reference to one of the supported KiiSocialNetworks.
 
 The user will not be authenticated or linked to a <KiiUser>
 until one of those methods are called explicitly.
 @param network One of the supported <KiiSocialNetworkName> values.
 @param key The SDK key assigned by the social network provider. It should not be nil or empty except for Kii Social Network Connect.In case of QQ just pass nil.
 @param secret The SDK secret assigned by the social network provider. In case of Twitter, It should not be nil or empty. In case of QQ and Kii Social Network Connect just pass nil.
 @param options Extra options that can be passed to the SNS, this is not mandatory. Examples could be (Facebook) an NSDictionary of permissions to grant to the authenticated user. In case of qq, twitter and Kii Social Network Connect, options parameter will not be used, please set to nil.
 @exception NSInvalidParameterException will be thrown if key and/or secret is not valid (see description above).
 @deprecated Not usable on new social connect login mechanism from v2.2.1.
 */
+ (void) setupNetwork:(KiiSocialNetworkName)network 
              withKey:(NSString*)key 
            andSecret:(NSString*)secret 
           andOptions:(NSDictionary*)options __attribute__((deprecated("Not usable on new social connect login mechanism from v2.2.1")));


/** Log a user into the social network provided
 
 This will initiate the login process for the given network, which provides OAuth like Facebook/Twitter,
 will send the user to the Facebook/Twitter app for authentication. If the social network user has already linked with a KiiUser,
 that user will be used as signed user. Otherwise, KiiCloud creates a new user and link with the specified social network account.
 If successful, the user is cached inside SDK as current user,and accessible via <[KiiUser currentUser]>.
 User token is also cached and can be get by <[KiiUser accessToken]>.
 Access token won't be expired unless you set it explicitly by <[Kii setAccessTokenExpiration:]>
 
 The network must already be set up via <setupNetwork:withKey:andSecret:andOptions:>
 @param network One of the supported <KiiSocialNetworkName> values
 @param options A dictionary of key/values to pass to KiiSocialConnect. Can be nil for Facebook and kiiSCNConnector but should not nil/empty for Twitter and QQ.
 
### Facebook
 Options for passing accessToken directly
 
<table>
<thead>
<tr>
    <th>Key</th>
    <th>Value type</th>
    <th>Value</th>
    <th>Note</th>
</tr>
</thead>
<tbody>
<tr>
    <td>access_token</td>
    <td>NSString</td>
    <td>Access token of Facebook.</td>
    <td>If provided, KiiCloud uses this token while login using Facebook.</td>
</tr>
<tr>
    <td>access_token_expires</td>
    <td>NSDate</td>
    <td>Expire date of access token.</td>
    <td>Use for social network token validation.</td>
</tr>
</tbody>
</table>

 Options by using ACAccount
 
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>permissions</td>
 <td>NSDictionary</td>
 <td>Key-Value pairs of permissions defined by Facebook. If not provided, it will pass "email" as the default permission</td>
 <td>Facebook : <a href="http://developers.facebook.com/docs/authentication/permissions">Facebook permissions</a>.</td>
 </tr>
 <tr>
 <td>use_acaccount</td>
 <td>NSNumber</td>
 <td>Select ACAccount class as loggin mechanism from v2.2.1</td>
 <td>If @YES, KiiCloud uses iOS native Facebook account then use the accessToken.</td>
 </tr>
 </tbody>
 </table>
 
### Twitter
<table>
<thead>
<tr>
    <th>Key</th>
    <th>Value type</th>
    <th>Value</th>
    <th>Note</th>
</tr>
</thead>
<tbody>
<tr>
    <td>twitter_account</td>
    <td>ACAccount</td>
    <td>Twitter account that is obtained from account store.</td>
    <td>This is mandatory if "oauth_token" and "oauth_token_secret" are not provided.</td>
</tr>
<tr>
    <td>oauth_token</td>
    <td>NSString</td>
    <td>OAuth access token of twitter.</td>
    <td>This is mandatory if "twitter_account" is not provided.</td>
</tr>
<tr>
    <td>oauth_token_secret</td>
    <td>NSString</td>
    <td>OAuth access token secret of twitter.</td>
    <td>This is mandatory if "twitter_account" is not provided.</td>
</tr>
</tbody>
</table>
 
 ### QQ
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>access_token</td>
 <td>NSString</td>
 <td>Access token of QQ.</td>
 <td>If provided, KiiCloud uses this token while login using QQ.</td>
 </tr>
 <tr>
 <td>openid</td>
 <td>NSString</td>
 <td>Unique with QQ App ID and user QQ account.</td>
 <td>QQ : <a href="http://wiki.open.qq.com/wiki/%E6%A6%82%E5%BF%B5%E5%92%8C%E6%9C%AF%E8%AF%AD#2.1_OpenID">QQ OpenID</a>.</td>
 </tr>
 </tbody>
 </table>
 
### Kii Social Network Connect
 <table>
 <thead>
 <tr>
    <th>Key</th>
    <th>Value type</th>
    <th>Value</th>
    <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
    <td>provider</td>
    <td>Provider</td>
    <td>Provider enum encapsulated on NSNumber object (ex. kiiConnectorFacebook)</td>
    <td>This is mandatory. </td>
 </tr>
 </tbody>
 </table>
 Snippet for Kii Social Network Connect :<br>
 
    [KiiSocialConnect logIn:kiiSCNConnector
               usingOptions:@{@"provider":@(kiiConnectorFacebook)}
               withDelegate:self
                andCallback:@selector(socialLoggedInWithUser:andNetwork:andError:)];

 
 @param delegate The object to make any callback requests to.
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) loggedIn:(KiiUser*)user usingNetwork:(KiiSocialNetworkName)network withError:(NSError*)error {
         
         // the process was successful - the user is now authenticated
         if(error == nil) {
             // do something with the user
             // you can get information by calling KiiSocialConnect#getAccessTokenDictionaryForNetwork:
             // snippet for kiiSCNConnector :
                NSDictionary* tokenDict = [KiiSocialConnect getAccessTokenDictionaryForNetwork:kiiSCNConnector];
                NSString* token = tokenDict[@"oauth_token"];
                NSString* tokenSecret = tokenDict[@"oauth_token_secret"];
                NSString* providerUserId = tokenDict[@"provider_user_id"];
         }
         else {
             // there was a problem
         }
     }
 @note This method should be called from main thread.
 @exception KiiIllegalStateException will be thrown if setupNetwork: is not called.
 @exception NSInvalidArgumentException will be thrown if options is not valid (see description above).
 @deprecated Use <[KiiSocialConnect logIn:options:block:]> .
 */
+ (void) logIn:(KiiSocialNetworkName)network usingOptions:(NSDictionary*)options withDelegate:(id)delegate andCallback:(SEL)callback __attribute__((deprecated("Use <[KiiSocialConnect logIn:options:block:]>")));

/** Login with specified social network.
 
 This will initiate the login process for the given network, which provides OAuth like Facebook/Twitter,
 will send the user to the Facebook/Twitter app for authentication. If the social network user has already linked with a KiiUser,
 that user will be used as signed user. Otherwise, KiiCloud creates a new user and link with the specified social network account.
 
 If successful, the user is cached inside SDK as current user,and accessible via <[KiiUser currentUser]>.
 User token is also cached and can be get by <[KiiUser accessToken]>.
 Access token won't be expired unless you set it explicitly by <[Kii setAccessTokenExpiration:]>

 Snippet for login with social network :<br>
 
    [KiiSocialConnect logIn:kiiSCNConnector usingOptions:@{@"provider":@(kiiConnectorFacebook)}
        andBlock:^(KiiUser *user, KiiSocialNetworkName name, NSError *error) {
        if (error == nil) {
            // login successful. Do someting with the user.
        } else {
            // something went wrong.
        }
    }];
 
 @param network One of the supported <KiiSocialNetworkName> values
 @param options A dictionary of key/values to pass to KiiSocialConnect. Can be nil for Facebook and kiiSCNConnector but should not nil/empty for Twitter.
 For details about options, refer to <[KiiSocialConnect logIn:usingOptions:withDelegate:andCallback:>
 @param block To be called upon login completion.
 @note This API access to server. Should not be executed in UI/Main thread.
 @exception KiiIllegalStateException will be thrown if setupNetwork: is not called.
 @exception NSInvalidParameterException will be thrown if options is not valid (see description above).
 @deprecated Use <[KiiSocialConnect logIn:options:block:]> .
 */
+ (void) logIn:(KiiSocialNetworkName)network usingOptions:(NSDictionary*)options andBlock: (KiiSocialConnectBlock) block __attribute__((deprecated("Use <[KiiSocialConnect logIn:options:block:]>")));

/** Login with specified social network.
 
 This will initiate the login process for the given network, with or without UI handled by SDK. If you prefer to handle login UI or using provider specific SDK to obtain access token, pass required params (acces token, access token secret, open ID) according to each provider. Other than <b> kiiConnectorQQ</b>, Kii SDK can handle the UI by passing nil into the options. If the social network user has already linked with a <KiiUser>,
 that user will be used as signed user. Otherwise, KiiCloud creates a new user and link with the specified social network account.
 The provider should be valid <KiiConnectorProvider> values. Otherwise, an exception will be raised. <br>
 Snippet for Login with social network without UI:<br>
 
    [KiiSocialConnect logIn:kiiConnectorFacebook 
                    options:@{@"accessToken":@"access_token"}
                      block:^(KiiUser *user, KiiConnectorProvider provider, NSError *error) {
        if (error == nil) {
            // link successful. Do someting with the user.
        } else {
            // something went wrong.
        }
    }];

 Snippet for Login with social network with UI:<br>
 
    [KiiSocialConnect logIn:kiiConnectorFacebook 
                    options:nil
                      block:^(KiiUser *user, KiiConnectorProvider provider, NSError *error) {
        if (error == nil) {
            // link successful. Do someting with the user.
        } else {
            // something went wrong.
        }
    }];

 Following parameters can be assigned to NSDictionary's key.<br><br>
 ### Facebook, Renren, Googleplus
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>accessToken</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 </tbody>
 </table>
 
 ### Twitter
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>accessToken</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 <tr>
 <td>accessTokenSecret</td>
 <td>String</td>
 <td>Required to generate signature when you call social network API.</td>
 <td></td>
 </tr>
 </tbody>
 </table>
 
 ### QQ
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>accessToken</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 <tr>
 <td>openID</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 </tbody>
 </table>
 
 @param provider One of the supported <KiiConnectorProvider> values.
 @param options A dictionary of key/values to pass to KiiSocialConnect. This can be nil if using UI approach.
 @param block To be called upon login completion.
 @exception NSInvalidParameterException will be thrown if options is not valid.
 @exception NSInvalidParameterException will be thrown if block is nil.
 @exception NSInvalidParameterException will be thrown if KiiSocialNetworkName is passed as provider.
 @warning Dropbox, Box, Yahoo, LinkedIn, Microsoft Live, Sina Weibo can only use login with UI.
 */
+ (void) logIn:(KiiConnectorProvider)provider options:(NSDictionary*)options block: (KiiSCNBlock) block;

/** Link the currently logged in user with a social network
 
 This will initiate the login process for the given network, which for SSO-enabled services like Facebook/Twitter, will send the user to the Facebook/Twitter app for authentication. There must be a currently authenticated <KiiUser>. Otherwise, you can use the logIn: method to create and log in a <KiiUser> using Facebook/Twitter. The network must already be set up via <setupNetwork:withKey:andSecret:andOptions:>
 @param network One of the supported <KiiSocialNetworkName> values.
 @param options A dictionary of key/values to pass to KiiSocialConnect. Can be nil for Facebook but should not nil/empty for Twitter.

### Facebook
 Options for passing accessToken directly
 
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>access_token</td>
 <td>NSString</td>
 <td>Access token of Facebook.</td>
 <td>If provided, KiiCloud uses this token while login using Facebook.</td>
 </tr>
 <tr>
 <td>access_token_expires</td>
 <td>NSDate</td>
 <td>Expire date of access token.</td>
 <td>Use for social network token validation.</td>
 </tr>
 </tbody>
 </table>
 
 Options by using ACAccount
 
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>permissions</td>
 <td>NSDictionary</td>
 <td>Key-Value pairs of permissions defined by Facebook. If not provided, it will pass "email" as the default permission</td>
 <td>Facebook : <a href="http://developers.facebook.com/docs/authentication/permissions">Facebook permissions</a>.</td>
 </tr>
 <tr>
 <td>use_acaccount</td>
 <td>NSNumber</td>
 <td>Select ACAccount class as loggin mechanism from v2.2.1</td>
 <td>If @YES, KiiCloud uses iOS native Facebook account then use the accessToken.</td>
 </tr>
 </tbody>
 </table>

### Twitter
<table>
<thead>
<tr>
    <th>Key</th>
    <th>Value type</th>
    <th>Value</th>
    <th>Note</th>
</tr>
</thead>
<tbody>
<tr>
    <td>twitter_account</td>
    <td>ACAccount</td>
    <td>Twitter account that is obtained from account store.</td>
    <td>This is mandatory if "oauth_token" and "oauth_token_secret" are not provided.</td>
</tr>
<tr>
    <td>oauth_token</td>
    <td>NSString</td>
    <td>OAuth access token of twitter.</td>
    <td>This is mandatory if "twitter_account" is not provided.</td>
</tr>
<tr>
    <td>oauth_token_secret</td>
    <td>NSString</td>
    <td>OAuth access token secret of twitter.</td>
    <td>This is mandatory if "twitter_account" is not provided.</td>
</tr>
</tbody>
</table>
 
 ### QQ
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>access_token</td>
 <td>NSString</td>
 <td>Access token of QQ.</td>
 <td>If provided, KiiCloud uses this token while login using QQ.</td>
 </tr>
 <tr>
 <td>openid</td>
 <td>NSString</td>
 <td>Unique with QQ App ID and user QQ account.</td>
 <td>QQ : <a href="http://wiki.open.qq.com/wiki/%E6%A6%82%E5%BF%B5%E5%92%8C%E6%9C%AF%E8%AF%AD#2.1_OpenID">QQ OpenID</a>.</td>
 </tr>
 </tbody>
 </table>
 
 
### Kii Social Network Connect
&nbsp;&nbsp;&nbsp;&nbsp;This operation is not supported for kiiSCNConnector network name.
 
 
 @param delegate The object to make any callback requests to.
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) userLinked:(KiiUser*)user withNetwork:(KiiSocialNetworkName)network andError:(NSError*)error {
         
         // the process was successful - the user is now linked to the network
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 @exception KiiIllegalStateException will be thrown if setupNetwork: is not called.
 @exception NSInvalidParameterException will be thrown if options is not valid (see description above) or if kiiSCNConnector network name is passed.
 @deprecated Use <[KiiSocialConnect linkCurrentUser:options:block:]>
 */
+ (void) linkCurrentUserWithNetwork:(KiiSocialNetworkName)network
                       usingOptions:(NSDictionary*)options
                       withDelegate:(id)delegate
                        andCallback:(SEL)callback __attribute__((deprecated("Use <[KiiSocialConnect linkCurrentUser:options:block:]>")));


/** Link the currently logged in user with a social network
 
 This will initiate the login process for the given network, which for SSO-enabled services like Facebook/Twitter, will send the user to the Facebook/Twitter app for authentication. There must be a currently authenticated <KiiUser>. Otherwise, you can use the logIn: method to create and log in a <KiiUser> using Facebook/Twitter. The network must already be set up via <setupNetwork:withKey:andSecret:andOptions:>
 
 Snippet for link with social network :<br>
 
    [KiiSocialConnect linkCurrentUserWithNetwork:kiiSCNConnector usingOptions:@{@"provider":@(kiiConnectorFacebook)}
        andBlock:^(KiiUser *user, KiiSocialNetworkName name, NSError *error) {
        if (error == nil) {
            // link successful. Do someting with the user.
        } else {
            // something went wrong.
        }
    }];
 
 @param network One of the supported <KiiSocialNetworkName> values.
 @param options A dictionary of key/values to pass to KiiSocialConnect. Can be nil for Facebook but should not nil/empty for Twitter.
 For details about options, refer to <[KiiSocialConnect linkCurrentUserWithNetwork:usingOptions:withDelegate:andCallback:>
 @param block To be called upon link completion.
 @note This API access to server. Should not be executed in UI/Main thread.
 @exception KiiIllegalStateException will be thrown if setupNetwork: is not called.
 @exception NSInvalidParameterException will be thrown if options is not valid (see description above) or if kiiSCNConnector network name is passed.
 @deprecated Use <[KiiSocialConnect linkCurrentUser:options:block:]>
 */
+ (void) linkCurrentUserWithNetwork:(KiiSocialNetworkName)network
                       usingOptions:(NSDictionary*)options
                          andBlock:(KiiSocialConnectBlock) block __attribute__((deprecated("Use <[KiiSocialConnect linkCurrentUser:options:block:]>")));

/** Link the currently logged in user with supported social networks (Facebook, Twitter, Renren, Google and QQ).
 
 The provider should be valid <KiiConnectorProvider> values. Otherwise, an exception will be raised. <br>
 Snippet for link with social network:<br>
 
    [KiiSocialConnect linkCurrentUser:kiiConnectorFacebook 
                              options:@{@"accessToken":@"access_token"}
                                block:^(KiiUser *user, KiiConnectorProvider provider, NSError *error) {
        if (error == nil) {
            // link successful. Do someting with the user.
        } else {
            // something went wrong.
        }
    }];
 
 Following parameters can be assigned to NSDictionary's key.<br><br>
 ### Facebook, Renren, GooglePlus
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>accessToken</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 </tbody>
 </table>
 
 ### Twitter
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>accessToken</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 <tr>
 <td>accessTokenSecret</td>
 <td>String</td>
 <td>Required to generate signature when you call social network API.</td>
 <td></td>
 </tr>
 </tbody>
 </table>
 
 ### QQ
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>accessToken</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 <tr>
 <td>openID</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 </tbody>
 </table>
 
 @param provider One of the supported <KiiConnectorProvider> values.
 @param options A dictionary of key/values to pass to KiiSocialConnect. This is mandatory, can not be nil.
 @param block To be called upon link completion. This is mandatory.
 @exception NSInvalidParameterException will be thrown if options is not valid.
 @exception NSInvalidParameterException will be thrown if block is nil.
 @exception NSInvalidParameterException will be thrown if unsupported provider or KiiSocialNetworkName is passed as provider.
 @warning Dropbox, Box, Yahoo, LinkedIn, Microsoft Live, Sina Weibo is not supported, passing it will throw an exception.
 */
+ (void) linkCurrentUser:(KiiConnectorProvider)provider
                 options:(NSDictionary*)options
                   block:(KiiSCNBlock) block;


/** Unlink the currently logged in user from the social network. This operation is not supported for kiiSCNConnector network name.
 
 The network must already be set up via <setupNetwork:withKey:andSecret:andOptions:>
 @param network One of the supported <KiiSocialNetworkName> values.
 @param delegate The object to make any callback requests to.
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
     - (void) userUnLinked:(KiiUser*)user fromNetwork:(KiiSocialNetworkName)network withError:(NSError*)error {
         
         // the process was successful - the user is no longer linked to the network
         if(error == nil) {
             // do something with the user
         }
         
         else {
             // there was a problem
         }
     }
 @exception KiiIllegalStateException will be thrown if setupNetwork: is not called.
 @exception NSInvalidParameterException will be thrown if kiiSCNConnector network name is passed.
 @deprecated Use <[KiiSocialConnect unLinkCurrentUser:block:]>
 */
+ (void) unLinkCurrentUserWithNetwork:(KiiSocialNetworkName)network
                         withDelegate:(id)delegate
                          andCallback:(SEL)callback __attribute__((deprecated("Use <[unLinkCurrentUser:block:]>")));


/** Unlink the currently logged in user from the social network. This operation is not supported for kiiSCNConnector network name.
 
 The network must already be set up via <setupNetwork:withKey:andSecret:andOptions:>
 
 Snippet for unlink current user with network. :<br>
 
    [KiiSocialConnect unLinkCurrentUserWithNetwork:kiiSCNConnector
        andBlock:^(KiiUser *user, KiiSocialNetworkName name, NSError *error) {
        if (error == nil) {
            // unlink successful.
        } else {
            // something went wrong.
        }
    }];
 @param network One of the supported <KiiSocialNetworkName> values.
 @param block To be called upon unlink completion.
 @note This API access to server. Should not be executed in UI/Main thread.
 @exception KiiIllegalStateException will be thrown if setupNetwork: is not called.
 @exception NSInvalidParameterException will be thrown if kiiSCNConnector network name is passed.
 @deprecated Use <[KiiSocialConnect unLinkCurrentUser:block:]>
 */
+ (void) unLinkCurrentUserWithNetwork:(KiiSocialNetworkName)network
                         andBlock:(KiiSocialConnectBlock)block __attribute__((deprecated("Use <[unLinkCurrentUser:block:]>")));

/** Unlink the currently logged in user from the social network.
 
 The provider should be valid <KiiConnectorProvider> values. Otherwise, an exception will be raised.
 
 Snippet for unlink current user with network. :<br>
 
    [KiiSocialConnect unLinkCurrentUser:kiiConnectorFacebook
                                  block:^(KiiUser *user, KiiConnectorProvider name, NSError *error) {
        if (error == nil) {
            // unlink successful.
        } else {
            // something went wrong.
        }
    }];
 @param provider One of the supported <KiiConnectorProvider> values.
 @param block To be called upon unlink completion. This is mandatory.
 @note This API access to server. Should not be executed in UI/Main thread.
 @exception NSInvalidParameterException will be thrown if block is nil.
 @exception NSInvalidParameterException will be thrown if unsupported provider or KiiSocialNetworkName is passed as provider.
 @warning Dropbox, Box, Yahoo, LinkedIn, Microsoft Live, Sina Weibo is not supported, passing it will throw an exception.
 */
+ (void) unLinkCurrentUser:(KiiConnectorProvider)provider
                     block:(KiiSCNBlock)block;

/** Retrieve the current user's access token from a social network
 
 The network must be set up and linked to the current user. It is recommended you save this to preferences for multi-session use.
 @param network One of the supported <KiiSocialNetworkName> values.
 @return An NSString representing the access token, nil if none available.
 @deprecated This method is deprecated. Use <[KiiSocialConnect accessTokenDictionary:]> instead.
 */
+ (NSString*) getAccessTokenForNetwork:(KiiSocialNetworkName)network __attribute__((deprecated("Use <[accessTokenDictionary:]> instead.")));

/** Retrieve the current user's access token expiration date from a social network
 
 The network must be set up and linked to the current user. It is recommended you save this to preferences for multi-session use.
 @param network One of the supported <KiiSocialNetworkName> values.
 @return An NSDate representing the access token's expiration date, nil if none available.
 @deprecated This method is deprecated. Use <[KiiSocialConnect accessTokenDictionary:]> instead.
 */
+ (NSDate*) getAccessTokenExpiresForNetwork:(KiiSocialNetworkName)network __attribute__((deprecated("Use <[accessTokenDictionary:]> instead.")));

/** Retrieve the current user's access token object by NSDictionary from a social network

 The network must be set up and linked to the current user. It is recommended you save this to preferences for multi-session use.
 Following parameters can be assigned to NSDictionary's key.<br><br>
 ### Facebook
 <table>
 <thead>
 <tr>
    <th>Key</th>
    <th>Value type</th>
    <th>Value</th>
    <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
    <td>access_token</td>
    <td>String</td>
    <td>Required for accessing social network API.</td>
    <td></td>
 </tr>
 <tr>
    <td>access_token_expires</td>
    <td>String</td>
    <td>Expiration date for this token</td>
    <td></td>
 </tr>
 </tbody>
 </table>
 
 ### Twitter
 <table>
 <thead>
 <tr>
    <th>Key</th>
    <th>Value type</th>
    <th>Value</th>
    <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
    <td>oauth_token</td>
    <td>String</td>
    <td>Required for accessing social network API.</td>
    <td></td>
 </tr>
 <tr>
    <td>oauth_token_secret</td>
    <td>String</td>
    <td>Required to generate signature when you call social network API.</td>
    <td></td>
 </tr>
 </tbody>
 </table>

 ### QQ
 <table>
 <thead>
 <tr>
 <th>Key</th>
 <th>Value type</th>
 <th>Value</th>
 <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
 <td>access_token</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 <tr>
 <td>openID</td>
 <td>String</td>
 <td>Required for accessing social network API.</td>
 <td></td>
 </tr>
 </tbody>
 </table>
 
 ### Kii Social Network Connect
 <table>
 <thead>
 <tr>
    <th>Key</th>
    <th>Value type</th>
    <th>Value</th>
    <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
    <td>oauth_token</td>
    <td>String</td>
    <td>Required for accessing social network API.</td>
    <td></td>
 </tr>
 <tr>
    <td>oauth_token_secret</td>
    <td>String</td>
    <td>Required to generate signature when you call social network API.</td>
    <td>Present in the bundle for Twitter, LinkedIn, and Yahoo.</td>
 </tr>
 <tr>
    <td>provider_user_id</td>
    <td>String</td>
    <td>User id provided by social network. ex.) 'xoauth_yahoo_guid' used by Yahoo profile API.</td>
    <td></td>
 </tr>
 <tr>
    <td>kii_new_user</td>
    <td>NSNumber(BOOL)</td>
    <td>Indicates if user was created during connection.</td>
    <td></td>
 </tr>
 <tr>
     <td>id_token</td>
     <td>NSString</td>
     <td>ID token provided by OpenID Provider. This field is provided
     when <kiiConnectorOpenIDConnectSimple> is used.</td>
     <td>Present in OpenID Connect</td>
 </tr>
 <tr>
     <td>refresh_token</td>
     <td>NSString</td>
     <td>Refresh token provided by OpenID Provider. This field is
     provided when <kiiConnectorOpenIDConnectSimple> is used and
     configured OpenID Provider supports refresh token.</td>
     <td>Present in OpenID Connect</td>
 </tr>
 </tbody>
 </table>

 @param network One of the supported <KiiSocialNetworkName> values.
 @return An NSDictionary representing the access token's object.
 @deprecated This method is deprecated. Use <[KiiSocialConnect accessTokenDictionary:]> instead.
 */
+ (NSDictionary *)getAccessTokenDictionaryForNetwork:(KiiSocialNetworkName)network __attribute__((deprecated("Use <[KiiSocialConnect accessTokenDictionary:]> instead.")));

/** Retrieve the current user's social network access token object as NSDictionary.
 If the user is not associated with the specified provider, returns nil.
 The dictionary would be cached after the login and link has been executed.
 Cache would be cleared when new login, link or unlink has been executed.
 (Regardless of same/different KiiConnectorProvider is specified)

 Please keep the returned value in your application program before execute new login/
 link session when you sequencially link the several social network providers
 with the same user if you need to use them.
 Following parameters can be assigned to NSDictionary's key.<br><br>
 <table>
 <thead>
 <tr>
    <th>Key</th>
    <th>Value type</th>
    <th>Value</th>
    <th>Note</th>
 </tr>
 </thead>
 <tbody>
 <tr>
    <td>oauth_token</td>
    <td>String</td>
    <td>Required for accessing social network API.</td>
    <td></td>
 </tr>
 <tr>
    <td>oauth_token_secret</td>
    <td>String</td>
    <td>Required to generate signature when you call social network API.</td>
    <td>Present in the bundle for Twitter.</td>
 </tr>
 <tr>
    <td>provider_user_id</td>
    <td>String</td>
    <td>User id provided by social network. ex.) 'xoauth_yahoo_guid' used by Yahoo profile API.</td>
    <td></td>
 </tr>
 <tr>
    <td>kii_new_user</td>
    <td>NSNumber(BOOL)</td>
    <td>Indicates if user was created during connection.</td>
    <td></td>
 </tr>
 <tr>
     <td>openID</td>
     <td>NSString</td>
     <td>OpenId identifier</td>
     <td>Present in QQ</td>
 </tr>
 <tr>
     <td>oauth_token_expires</td>
     <td>NSDate</td>
     <td>Oauth expirations date</td>
     <td>Present only if logged in using UI and selected providers (Facebook, Google, Box, Renren, Sina Weibo, and Microsoft Live)</td>
 </tr>
 <tr>
     <td>id_token</td>
     <td>NSString</td>
     <td>ID token provided by OpenID Provider. This field is provided
     when <kiiConnectorOpenIDConnectSimple> is used.</td>
     <td>Present in OpenID Connect</td>
 </tr>
 <tr>
     <td>refresh_token</td>
     <td>NSString</td>
     <td>Refresh token provided by OpenID Provider. This field is
     provided when <kiiConnectorOpenIDConnectSimple> is used and
     configured OpenID Provider supports refresh token.</td>
     <td>Present in OpenID Connect</td>
 </tr>
 </tbody>
 </table>

 @param provider One of the supported <KiiConnectorProvider> values.
 @return An NSDictionary representing the access token's object.
 @exception NSInvalidParameterException will be thrown if KiiSocialNetworkName is passed as provider.
 */
+ (NSDictionary *)accessTokenDictionary:(KiiConnectorProvider)provider;
@end
