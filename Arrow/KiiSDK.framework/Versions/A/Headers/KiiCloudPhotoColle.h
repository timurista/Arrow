//
//  KiiCloudPhotoColle.h
//  KiiSDK-Private
//
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

@class KiiUploader;
@class DCPhotoColle;

@protocol KiiUploadTransferManaging;

/**
 * KiiCloudPhotoColle provides interfaces to access DCPhotoColle
 * instance and resumable upload.
 *
 * Instance of KiiCloudPhotoColle must be got by
 * <[KiiPhotoColleSocialConnect kiiCloudPhotoColle]>. Applications
 * must not create KiiCloudPhotoColle instance by
 * `[[KiiCloudPhotoColle alloc] init]`.
 */
@interface KiiCloudPhotoColle : NSObject

/**
 * Property for DCPhotoColle.
 */
@property (nonatomic, readonly, strong) DCPhotoColle *photocolle;

/**
 * Returns a new instance of <KiiUploader>.
 * @param localPath Path of the file to be uploaded. Following file formats can be uploaded.
 *
 * **[Images]**
 *
 *   - .jpg / .jpeg / .jpe
 *
 * **[Movies]**
 *
 *   - .3gp
 *   - .mov
 *   - .mp4
 *   - .avi
 *   - .mts
 *   - .m2ts
 *
 * @return A new instance of <KiiUploader> or nil if an error occurs while instantiation.
 * @exception NSInvalidArgumentException Thrown if specified localPath is nil or empty.
 * @see uploader:error:
 */
- (KiiUploader *)uploader:(NSString *)localPath;

/**
 * Returns a new instance of <KiiUploader>.
 * @param localPath Path of the file to be uploaded. Following file formats can be uploaded.
 *
 * **[Images]**
 *
 *   - .jpg / .jpeg / .jpe
 *
 * **[Movies]**
 *
 *   - .3gp
 *   - .mov
 *   - .mp4
 *   - .avi
 *   - .mts
 *   - .m2ts
 * @param error If an error occurs while instantiation, upon return contains an `NSError` object that describes the problem.
 *
 * @return A new instance of <KiiUploader> or nil if an error occurs while instantiation.
 * @exception NSInvalidArgumentException Thrown if specified localPath is nil or empty.
 * @see uploader:
 */

- (KiiUploader *)uploader:(NSString *)localPath error:(NSError **) error;

/**
 * Get upload transfer manager object for PhotoColle.
 *
 * @return A upload transfer manager object for PhotoColle.
 */
- (id<KiiUploadTransferManaging>)uploadTransferManager;

@end
