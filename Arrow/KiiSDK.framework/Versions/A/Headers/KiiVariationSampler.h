//
//  VariationSampler.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 9/18/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
@class KiiVariation,KiiExperiment;
/** A protocol interface for sampler of variation.
 Used by <KiiExperiment#appliedVariationWithSampler:andError:> . You can implement customized logic of sampling variation by implementing this protocol;
 */
@protocol KiiVariationSampler <NSObject>
@required

/** Called when sampling needed.
 You can decide whether returning an error regarding to experiment state, Application setting, user attributes, etc. 
 Application has to decide the behavior when it returns an error. ex.) Apply default UI, etc.
 Note:<KiiExperiment#appliedVariationWithSampler:andError:> would not return any error.
 Class that implementing this protocol is responsible for checking state of experiment.
 @param experiment that requires sampling.
 @param error An NSError object, passed by reference. 
 @return applied variation for this time.
 */
- (KiiVariation*) chooseVariation:(KiiExperiment*) experiment withError:(NSError**) error;
@end
