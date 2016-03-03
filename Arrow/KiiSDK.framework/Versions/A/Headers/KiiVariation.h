//
//  Variation.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 9/18/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
@class KiiConversionEvent;
/** Class that encapsulate variation of an experiment in AB Testing.
 */
@interface KiiVariation : NSObject

/** Generate event dictionary for conversion by given KiiConversionEvent instance.

 You will generate event when the specified condition in the <KiiExperiment> has achieved.

 ex.) User has signed up, view the message, purchase item, etc.

 You need to call <KiiAnalytics#trackEvent:withExtras:> and pass experiment ID and returned dictionary to send the event to Kii Analytics cloud.
 
     NSError* error = nil;
     KiiExperiment* experiment = [KiiExperiment getExperimentSynchronous:@"MyExperiment" withError:&error];
     if (error != nil) {
         // check error and return.
         return;
     }
     KiiVariation* variation = [experiment appliedVariationWithError:&error];
     if (error != nil) {
         // check error and return.
         return;
     }
     KiiConversionEvent* c1 = [experiment conversionEventAtIndex:0];
     NSDictionary* dict = [variation eventDictionaryForConversion:c1];
     
     [KiiAnalytics trackEvent:[experiment experimentID] withExtras:dict];
 
 @param conversionEvent an instance of KiiConversionEvent.
 @return a dictionary consist of events. Returns nil if index is out of range.
 */
- (NSDictionary*) eventDictionaryForConversion:(KiiConversionEvent*) conversionEvent;

/** Generate event dictionary for conversion by given index.

 You will generate event when the specified condition in the <KiiExperiment> has achieved.

 ex.) User has signed up, view the message, purchase item, etc.

 You need to call <KiiAnalytics#trackEvent:withExtras:> and pass experiment ID and returned dictionary to send the event to Kii Analytics cloud.
     
     NSError* error = nil;
     KiiExperiment* experiment = [KiiExperiment getExperimentSynchronous:@"MyExperiment" withError:&error];
     if (error != nil) {
         // check error and return.
         return;
     }
     KiiVariation* variation = [experiment appliedVariationWithError:&error];
     if (error != nil) {
         // check error and return.
         return;
     }
     NSDictionary* dict = [variation eventDictionaryForConversionAtIndex:0];
     
     [KiiAnalytics trackEvent:[experiment experimentID] withExtras:dict];
     
 @param index conversion event index.
 @return a dictionary consist of events. Returns nil if index is out of range.
 */
- (NSDictionary*) eventDictionaryForConversionAtIndex:(int) index;

/** Generate event dictionary for conversion by given index.

 You will generate event when the specified condition in the <KiiExperiment> has achieved.

 ex.) User has signed up, view the message, purchase item, etc.

 You need to call <KiiAnalytics#trackEvent:withExtras:> and pass experiment ID and returned dictionary to send the event to Kii Analytics cloud.

     NSError* error = nil;
     KiiExperiment* experiment = [KiiExperiment getExperimentSynchronous:@"MyExperiment" withError:&error];
     if (error != nil) {
         // check error and return.
         return;
     }
     KiiVariation* variation = [experiment appliedVariationWithError:&error];
     if (error != nil) {
         // check error and return.
         return;
     }
     KiiConversionEvent* c1 = [experiment conversionEventAtIndex:0];
     NSDictionary* dict = [variation eventDictionaryForConversionWithName: [c1 name]];
     
     [KiiAnalytics trackEvent:[experiment experimentID] withExtras:dict];

 @param name conversion event name.
 @return a dictionary consist of events. Returns nil if not found.
 */
- (NSDictionary*) eventDictionaryForConversionWithName:(NSString*) name;

/** Get the name of variation.
 */
@property(nonatomic,readonly) NSString* name;
/** Get the percentage of variation to be applied.
 */
@property(nonatomic,readonly) int percentage;
/** Get the test of variation defined by you on the developer portal.
 */
@property(nonatomic,readonly) NSDictionary* variableDictionary;
@end
