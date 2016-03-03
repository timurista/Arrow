//
//  KiiExperiment.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 9/18/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiVariationSampler.h"

/**
* This enum represents the status of experiment.
*/
typedef NS_ENUM(NSInteger, KiiExperimentStatus) {
    /** The experiment is in draft. You need to run Experiment in the developer portal before starting A/B testing. */
    KiiExperimentDraft,
    /** The experiment is running. */
    KiiExperimentRunning,
    /** The experiment is paused. */
    KiiExperimentPaused,
    /** The experiment is terminated. */
    KiiExperimentTerminated
};

@class KiiExperiment;

typedef void(^KiiExperimentBlock)(KiiExperiment* experiment, NSError* error);

@class KiiVariation,KiiConversionEvent;
/** KiiExperiment is a class that handle experiment for AB testing feature. */
@interface KiiExperiment : NSObject

/** Get experiment synchronously having the specified id.
  This is blocking methods.
 @param experimentID experiment id.
 @param error An NSError object, passed by reference.
 @return an instant of KiiExperiment.
 @exception NSInvalidArgumentException Thrown if given experiment id is <br>
  - Nil or empty <br>
  - Does not match with the pattern [a-zA-Z0-9\-\_\.]\{2,100\}"<br>
 */
+ (KiiExperiment*) getExperimentSynchronous:(NSString*) experimentID withError:(NSError**) error;

/** Get experiment asynchronously having the specified id with block.
 This is non-blocking methods.
 
     [KiiExperiment getExperiment:experimentID withBlock:^(KiiExperiment *experiment, NSError *error) {
         if(error == nil) {
             NSLog(@"Experiment: %@", experiment);
         }
     }];
 
 @param experimentID experiment id.
 @param completion The block to be called upon method completion. See example.
 @return an instant of KiiExperiment.
 @exception NSInvalidArgumentException Thrown if given experiment id is <br>
  - Nil or empty <br>
  - Does not match with the pattern [a-zA-Z0-9\-\_\.]\{2,100\}" <br>
 */
+ (void) getExperiment:(NSString*) experimentID withBlock:(KiiExperimentBlock) completion;

/** Get the variation applied to this trial.
 Get the variation applied to this trial. Variation will be determined by specified rate of each variation in this experiment.
 
 Current login user information will be used for sampling. If the experiment has finished with specified variant, the specified variant will be returned regardless of login user information. 

 - If the experiment is in draft an error (code 902) will be returned.
 - If the experiment has been paused an error (code 903) will be returned.
 - If the experiment has been terminated without specified variant an error (code 904) will be returned.
 - If No user logged in, an error (code 906) will be returned.

 @param error An NSError object, passed by reference.
 */
- (KiiVariation*) appliedVariationWithError:(NSError**) error;

/** Get the variation applied to this trial. <br>
Sampler should return the variation according to the rate defined in this experiment.
If you use <KiiVariationSamplerByKiiUser> with current login user, it will be same as <appliedVariationWithError:> .
 @note An error will be returned if the experiment is not applied, depending on implementation of KiiVariationSampler.
 @param sampler an instance of object that conform KiiVariationSampler protocol.
 @param error An NSError object, passed by reference.
 */
- (KiiVariation*) appliedVariationWithSampler:(id<KiiVariationSampler>) sampler andError:(NSError**) error;

/** Get the variation applied to this trial. <br>
    Sampler should return the variation according to the rate defined
    in this experiment. If sampler fails to return applied variation,
    then fallback is returned. If you use
    <KiiVariationSamplerByKiiUser> with current login user, it will be
    same as <appliedVariationWithError:>.

    @param sampler an instance of object that conform
    KiiVariationSampler protocol.
    @param fallback fallback The variation to return when failed to
    get the applied variation.
    @return applied variation for this trial or fallback.
 */
- (KiiVariation*) appliedVariationWithSampler:(id<KiiVariationSampler>) sampler
                                     fallback:(KiiVariation*) fallback;

/** Get the variation which has the specified name.
 
 @param name variation name.
 @return an instance of KiiVariation. nil if there is no variation which have specified name.
 */
- (KiiVariation*) variationByName:(NSString*)name;


/** Get conversion event by index.
 @param index an index of conversion. Should be on variations array bounds.
 @return an instance of KiiVariation, nil if there is no element found for specified index.
 */
- (KiiConversionEvent*) conversionEventAtIndex:(int) index;

/** Get conversion event by its name.
 @param name of the Conversion Event.
 @return an instance of KiiVariation, nil if there is no conversion found for specified name.
 */
- (KiiConversionEvent*) conversionEventByName:(NSString*) name;


/** Get the variations associated with this experiment.
 @return an array of KiiVariation, nil if there is no variation found.
 */
@property(nonatomic,readonly) NSArray* variations;

/** Get the version number of the experiment.
 */
@property(nonatomic,readonly) int version;

/** Get the status of the experiment.
 */
@property(nonatomic,readonly) KiiExperimentStatus status;

/** Get the id of the experiment.
 */
@property(nonatomic,readonly) NSString* experimentID;

/** Get the description of the experiment.
 */
@property(nonatomic,readonly) NSString* experimentDescription;

/** Get all conversion events in this experiment.
 */
@property(nonatomic,readonly) NSArray* conversionEvents;

/** Returns chosen variation if the experiment has finished with specified variation.
 @return Variation chosen or nil if when the no variation has not been chosen.
 */
@property(nonatomic,readonly) KiiVariation* chosenVariation;

@end
