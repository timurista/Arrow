//
//  KiiRTransferManaging.h
//  KiiSDK-Private
//
//  Copyright (c) 2014 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "KiiDownloadTransferManaging.h"
#import "KiiUploadTransferManaging.h"

/**
 * A protocol to manage status of resumable transfer.
 */
@protocol KiiRTransferManaging <KiiDownloadTransferManaging,
        KiiUploadTransferManaging>

@end
