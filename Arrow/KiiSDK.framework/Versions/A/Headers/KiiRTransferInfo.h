//
//  KiiRTransferInfo.h
//  KiiSDK-Private
//
//  Created by Riza Alaudin Syah on 3/21/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
* This enum represents status of the transfer.
*/
typedef NS_ENUM(NSUInteger, KiiRTransferStatus) {
    /** No transfer entry exist for the file. */
    KiiRTStatus_NOENTRY,
    /** The transfer is ongoing. */
    KiiRTStatus_ONGOING,
    /** The transfer has suspended and can be resumed. */
    KiiRTStatus_SUSPENDED,
};

/** Class that is encapsulated transfer information.
 <h3> Transfer info properties : </h3>
  - completedSizeInBytes
  - totalSizeInBytes
  - status (enum)
 
 <h3> enumeration value of "status" : </h3>
  - KiiRTStatus_NOENTRY
  - KiiRTStatus_ONGOING
  - KiiRTStatus_SUSPENDED
 */
@interface KiiRTransferInfo : NSObject

/** Get a transfer completed size in bytes.
 @return NSUInteger completed size in bytes
 */
-(NSUInteger) completedSizeInBytes;

/** Get a transfer total size in bytes.
 @return NSUInteger total size in bytes.
 */
-(NSUInteger) totalSizeInBytes;

/** Get a transfer status.
 @return <KiiRTransferStatus> transfer status.
 */
-(KiiRTransferStatus) status;

@end
