//
//  KiiEncryptedBucket.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 1/15/15.
//  Copyright (c) 2015 Kii Corporation. All rights reserved.
//

#import "KiiBucket.h"

/**Class represents encrypted bucket.<br>
 * Contents inside this bucket is stored as encrypted data.<br>
 * Automatically decrypted when you refer contents so you don't have to decrypt data.<br>
 * Encryption rule of KiiObject Key-value pairs:<br>
 * <ul>
 *     <li>All keys are encrypted.</li>
 *     <li>Only string values are encrypted. Numbers, Boolean, Objects, Arrays won't be encrypted.</li>
 * </ul>
 * @note Object body inside this bucket won't be encrypted.<br><br>
 *
 * @warning Following limitations are applied to encrypted bucket: <br>
 * <ul>
 * <li>Query clauses using STRING properties will be restricted in the following way:</li>
 *  <ul>
 *      <li>Only In clause and Equals clause and its combination (AND, OR, NOT) can be used.</li>
 *      <li>Sorting with string String properties can not be used.</li>
 * </ul>
 * (Query clauses using other data types works same as non encrypted bucket.)
 * <li>Key-value pair whose key is greater than 334KB won't be indexed.</li>
 * <li>String key-value pair whose value is greater than 334KB won't be indexed.</li>
 * <li>If the key-value pair is not indexed, query for the key-value pair doesn't works.
 * (Behaves as if there's no matching data with no error.)</li>
 * </ul>
 */
@interface KiiEncryptedBucket : KiiBucket

@end
