//
//  ThingFields.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 12/16/14.
//  Copyright (c) 2014 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiBaseObject.h"

/**Represent fields of thing on KiiCloud.
 */
@interface KiiThingFields : KiiBaseObject

/**Set and get firmwareVersion.
 */
@property(nonatomic) NSString* firmwareVersion;

/**Set and get productName.
 */
@property(nonatomic) NSString* productName;

/**Set and get the lot.
 */
@property(nonatomic) NSString* lot;

/**Set and get the stringField1.
 */
@property(nonatomic) NSString* stringField1;

/**Set and get the stringField2.
 */
@property(nonatomic) NSString* stringField2;

/**Set and get the stringField3.
 */
@property(nonatomic) NSString* stringField3;

/**Set and get the stringField4.
 */
@property(nonatomic) NSString* stringField4;

/**Set and get the stringField5.
 */
@property(nonatomic) NSString* stringField5;

/**Set and get the numberField1.
 */
@property(nonatomic) NSNumber* numberField1;

/**Set and get the numberField2.
 */
@property(nonatomic) NSNumber* numberField2;

/**Set and get the numberField3.
 */
@property(nonatomic) NSNumber* numberField3;

/**Set and get the numberField4.
 */
@property(nonatomic) NSNumber* numberField4;

/**Set and get the numberField5.
 */
@property(nonatomic) NSNumber* numberField5;

/**Set and get vendor.
 */
@property(nonatomic) NSString* vendor;


@end
