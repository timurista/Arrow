//
//  KiiRandomVariationSampler.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 6/4/14.
//  Copyright (c) 2014 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiVariationSampler.h"

/** Sampler using Time based sampler.
 This does not require user login. Random seed will be generated based on execution timestamp.
 */
@interface KiiRandomVariationSampler : NSObject<KiiVariationSampler>

/** Do sampling. Returns random variation based on the percentage configured in portal.
 If the experiment has terminated and fixed variation has chosen, returns chosen variation. (Returned variation is same as <[KiiExperiment chosenVariation]>)
 If you want to customize the logic of Sampler, implement <KiiVariationSampler> and pass it to <[KiiExperiment appliedVariationWithError:]>.
 If the experiment is in draft an error (code 902) will be returned.
 If the experiment has paused an error (code 903) will be returned.
 If the experiment has been terminated without specified variant an error (code 904) will be returned.
 Application has to decide the behavior when this error returned. (ex. apply default UI, etc.)
 
 @param experiment that requires sampling. must not be nil.
 @param error An NSError object, passed by reference. can be nil, but
 not recommended.
 @return applied variation for this time.
 @exception NSInvalidArgumentException if experiment is nil.
 */
- (KiiVariation*) chooseVariation:(KiiExperiment*) experiment withError:(NSError**) error;
@end
