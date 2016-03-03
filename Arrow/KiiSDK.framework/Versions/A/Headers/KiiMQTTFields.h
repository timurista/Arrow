//
//  KiiMQTTFields.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 1/16/15.
//  Copyright (c) 2015 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 */
@interface KiiMQTTFields : NSObject

/** Boolean	if YES it will send the message to the MQTT installations. NO by default.
 */
@property (nonatomic) NSNumber* enabled;

/** Specific Data to be sent to MQTT.
 */
@property (nonatomic,copy) NSDictionary* data;

/** Field creation constructor, automatically set enabled = true to MQTT Field
 */
+(instancetype) createFields;

/** generate output dictionary object
 @return dict a Dictionary object contains one level json dictionary data
 */
-(NSDictionary*) generateFields;

@end
