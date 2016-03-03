//
//  KiiPushInstallation.h
//  KiiSDK-Private
//
//  Created by Riza Alaudin Syah on 1/21/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

@class KiiPushInstallation;
/** Block type for Kii Push installation*/
typedef void (^KiiPushCallbackBlock)(KiiPushInstallation *installation, NSError *error);


/** Install APNS feature on user scope*/
@interface KiiPushInstallation : NSObject



/** Asynchronously install APNS feature.
Install APNS using deviceToken captured by Kii +(void) setAPNSDeviceToken. This method is non-blocking method.
 
    [KiiPushInstallation installWithBlock:^(KiiPushInstallation *installation, NSError *error) {
        // do something with the result
    }];
 
 @param completion Block for installation process
 @deprecated This mehod is deprecated. Use <[KiiPushInstallation installWithDeviceToken:andDevelopmentMode:andCompletion:]> instead.
 */
+(void) installWithBlock:(KiiPushCallbackBlock) completion __attribute__((deprecated("Use [KiiPushInstallation installWithDeviceToken:andDevelopmentMode:andCompletion:]")));

/** Asynchronously install APNS feature with device token and development mode.
 This method is non-blocking method.
 
    [KiiPushInstallation installWithDeviceToken:deviceToken andDevelopmentMode:YES andCompletion:^(KiiPushInstallation *installation, NSError *error) {
    // do something with the result
    }];
 
 @param deviceToken device token that is given by APNS server.
 @param isDevelopmentMode YES if APNS development environment mode or NO for production mode.
 @param completion Block for installation process.
 @exception NSInvalidArgumentException Thrown if given deviceToken is nil.
 */
+(void) installWithDeviceToken:(NSData*) deviceToken andDevelopmentMode:(BOOL) isDevelopmentMode andCompletion:(KiiPushCallbackBlock) completion;

/** Asynchronously install APNS feature with device token and development mode.
 Install asynchronously using delegate and callback. This method is non-blocking.
 
 @param deviceToken device token that is given by APNS server.
 @param isDevelopmentMode YES if APNS development environment mode or NO for production mode.
 @param delegate The object to make any callback requests to.
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
        
        - (void)installCompleted:(KiiPushInstallation *) installation withError: (NSError *) error {
            if (nil == error) {
                NSLog(@" Installation succeeded");
            }
        }
 @exception NSInvalidArgumentException Thrown if given deviceToken is nil.
 */
+(void) installWithDeviceToken:(NSData*) deviceToken andDevelopmentMode:(BOOL) isDevelopmentMode andDelegate:(id) delegate andCallback:(SEL) callback;

/** Asynchronously install APNS feature.
 Install asynchronously using delegate and callback. This method is non-blocking.
 
 @param delegate The object to make any callback requests to
 @param callback The callback method to be called when the request is completed. The callback method should have a signature similar to:
 
        - (void)installCompleted:(KiiPushInstallation *) installation withError: (NSError *) error {
            if (nil == error) {
                NSLog(@" Installation succeeded");
            }
        }
 @deprecated This mehod is deprecated. Use <[KiiPushInstallation installWithDeviceToken:andDevelopmentMode:andDelegate:andCallback:]> instead.
 */
+(void)install:(id) delegate withCallback:(SEL) callback __attribute__((deprecated("Use [KiiPushInstallation installWithDeviceToken:andDevelopmentMode:andDelegate:andCallback:]")));

/** Synchronously installs APNS feature with device token and development mode. This method is blocking method.
 
 @param deviceToken device token that is given by APNS server.
 @param isDevelopmentMode YES if APNS development environment mode or NO for production mode.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @exception NSInvalidArgumentException Thrown if given deviceToken is nil.
 */
+(KiiPushInstallation*)installSynchronousWithDeviceToken:(NSData*) deviceToken andDevelopmentMode:(BOOL) isDevelopmentMode andError:(NSError**) error;

/** Synchronously installs APNS feature.
 Install APNS using deviceToken captured by Kii +(void) setAPNSDeviceToken. This method is blocking method.
 
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @deprecated This mehod is deprecated. Use <[KiiPushInstallation installSynchronousWithDeviceToken:andDevelopmentMode:andError:]> instead.
 */
+(KiiPushInstallation*)installSynchronous:(NSError**) error __attribute__((deprecated("Use [KiiPushInstallation installSynchronousWithDeviceToken:andDevelopmentMode:andError:]")));

/** Asynchronously uninstall APNs feature.
 Uninstall APNs using deviceToken captured by Kii +(void) setAPNSDeviceToken.
 This method is non-blocking method.

    [KiiPushInstallation uninstallWithBlock:^(KiiPushInstallation *uninstallation, NSError *error) {
        // do something with the result
        if (error == nil) {
            NSLog(@" Uninstallation succeeded");
        }
    }];

@param completion Block for uninstallation process
@deprecated This mehod is deprecated. Use <[KiiPushInstallation uninstallWithDeviceToken:andCompletion:]> instead.
 */
+(void) uninstallWithBlock:(KiiPushCallbackBlock) completion __attribute__((deprecated("Use [KiiPushInstallation uninstallWithDeviceToken:andCompletion:]")));

/** Synchronously uninstall APNs feature.
 Uninstall APNs using deviceToken captured by Kii +(void) setAPNSDeviceToken.
 This method is blocking method.

 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @deprecated This mehod is deprecated. Use <[KiiPushInstallation uninstallSynchronousWithDeviceToken:andError:]> instead.
 */
+(KiiPushInstallation*)uninstallSynchronous:(NSError**) error __attribute__((deprecated("Use [KiiPushInstallation uninstallSynchronousWithDeviceToken:andError:]")));

/** Asynchronously uninstall APNs feature for given device token.
 This method is non-blocking method.
 
    [KiiPushInstallation uninstallWithDeviceToken: deviceToken andCompletion:^(KiiPushInstallation *uninstallation, NSError *error) {
    // do something with the result
        if (error == nil) {
            NSLog(@" Uninstallation succeeded");
        }
    }];

 @param deviceToken device token that is given by APNS server.
 @param completion Block for uninstallation process.
 @exception NSInvalidArgumentException Thrown if given deviceToken is nil.
 */
+(void) uninstallWithDeviceToken:(NSData*) deviceToken andCompletion:(KiiPushCallbackBlock) completion;

/** Synchronously uninstall APNs featurefor given device token.
 This method is blocking method.

 @param deviceToken device token that is given by APNS server.
 @param error On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You can not specify nil for this parameter or it will cause runtime error.
 @exception NSInvalidArgumentException Thrown if given deviceToken is nil.
 */
+(KiiPushInstallation*)uninstallSynchronousWithDeviceToken:(NSData*) deviceToken andError:(NSError**) error;

@end
