//
//  KiiUploader.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 3/25/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiRTransfer.h"
#import "KiiResumableTransfer.h"

/** Implementation of resumable transfer operation.
 */
@class KiiResumableTransfer;
@interface KiiUploader : KiiResumableTransfer<KiiRTransfer>

/** @name Get local file path */

/** Get local file path that is used by the uploader.
 @return A string value of local file path that is used by the uploader.
 */
-(NSString*) localPath;

@end
