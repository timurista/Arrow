//
//  VariationSamplerByKiiUser.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 9/18/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiVariationSampler.h"
/** Sampler using KiiUser attributes.
 This uses current login KiiUser ID to randomise the variation.
 */
@interface KiiVariationSamplerByKiiUser : NSObject<KiiVariationSampler>
/** Do sampling. Returns random variation based on the percentage configured in portal. 
 If the experiment has terminated and fixed variation has chosen, returns chosen variation. (Returned variation is same as <KiiExperiment#chosenVariation>)
 If you want to customize the logic of Sampler, implement <VariationSampler> and pass it to <KiiExperiment#getAppliedVariation:>.
 If the experiment is in draft an error (code 902) will be returned.
 If the experiment has paused an error (code 903) will be returned.
 If the experiment has been terminated without specified variant an error (code 904) will be returned.
 Application has to decide the behavior when this error returned. (ex. apply default UI, etc.)
 
 @param experiment that requires sampling.
 @param error An NSError object, passed by reference.
 @return applied variation for this time.
 */
- (KiiVariation*) chooseVariation:(KiiExperiment*) experiment withError:(NSError**) error;
@end
