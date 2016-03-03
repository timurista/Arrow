//
//  KiiError.h
//  KiiSDK-Private
//
//  Created by Chris Beauchamp on 12/21/11.
//  Copyright (c) 2011 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/** Find error codes and messages that can be returned by Kii SDK.
 If error happen, SDK will throw a NSError object that contains code, descriptions and server response informations (if available) in NSError's userInfo property.
 
 <h3>Application Errors (1xx)</h3>
 - *101* - The application received invalid credentials and was not initialized
 - *102* - The application was not found on the server. Please ensure your app id and key were entered correctly.
 - *103* - The required operation is failed due to unexpected error. Should not thrown if using latest SDK.

 <h3>Connectivity and Requests Errors (2xx)</h3>
 - *201* - Unable to connect to the internet
 - *202* - Unable to parse server response
 - *203* - Unable to authorize request
 - *204* - Invalid input data, the given parameter and/or data is not valid.
 
 <h3>User API Errors (3xx)</h3>
 - *301* - Unable to retrieve valid access token
 - *302* - Unable to authenticate user
 - *303* - Unable to retrieve file list
 - *304* - Invalid password format. Password must be 4-50 printable ASCII characters.
 - *305* - Invalid email format. Email must be a valid address
 - *306* - Invalid user object. Please ensure the credentials were entered properly
 - *307* - Invalid username format. The username must be 3-64 alphanumeric characters - the first character must be a letter
 - *308* - Invalid phone format. The phone number must be numeric between 7 and 20 digits, and it can begin with '+'
 - *309* - Unable to verify account. Please ensure the verification code provided is correct
 - *310* - Invalid displayname format. The displayname length is 4-50 chars (not byte), and allow Multi-Byte input.
 - *311* - The user's email was unable to be updated on the server
 - *312* - The user's phone number was unable to be updated on the server
 - *313* - Invalid format of userIdentifier.
 - *314* - The request could not be made - the key associated with the social network is invalid
 - *315* - Invalid country code.
 - *316* - Invalid local phone format. The phone number numerical and must be at least 7 digits
 - *317* - Invalid credentials, please check whether the credentials associated with social network is valid
 - *318* - Social network account has been already linked.
 - *319* - Social network account is not linked.
 - *320* - Social network authentication was canceled.
 - *321* - Server side login is failed with error. Additional error info is available in userInfo[@"description"] and userInfo[@"server_code"].
 - *322* - Unable to load authentication page. Additional error info is available in userInfo[@"description"].
 - *323* - Unsupported Application structure. Server-side authentication needs rootViewController to be assigned on Application main window.
 - *324* - User not found. The User object that is processed by the request is not available on the cloud.
 - *325* - Group not found. The Group object that is processed by the request is not available on the cloud.
 - *326* - Local phone number requires country code.
 - *327* - Non logged-in user can not use this method.
 - *328* -
 - *329* - Invalid user status, the user should have either verified email or phone number.
 - *330* - Fail to refresh access token;
 - *331* - Valid stored user not found;
 - *332* - ACAccount authentication is not granted by user
 - *333* - ACAccount is not set by user
 - *340* - User is not logged, this operations does not allow anonymous user.
 
 <h3>File API Errors (4xx)</h3>
 - *401* - Unable to delete file from cloud
 - *402* - Unable to upload file to cloud
 - *403* - Unable to retrieve local file for uploading. May not exist, or may be a directory
 - *404* - Unable to shred file. Must be in the trash before it is permanently deleted
 - *405* - Unable to perform operation - a valid container must be set first
 - *406* - Insufficient space in cloud to store data
 
 <h3>Core Object Errors (5xx)</h3>
 - *501* - Invalid objects passed to method. Must be already saved on server
 - *502* - Unable to parse object. Must be JSON-encodable
 - *503* - Duplicate entry exists
 - *504* - Invalid remote path set for KiiFile. Must be of form: /root/path/subpath
 - *505* - Unable to delete object from cloud
 - *506* - Invalid KiiObject type - the type does not match the regex [A-Za-z]{1}[A-Za-z0-9-_]{4,49}
 - *507* - Unable to set an object as a child of itself
 - *508* - The key of the object being set is being used by the system. Please use a different key
 - *509* - The container you are trying to operate on does not exist
 - *510* - The object you are trying to operate on does not exist on the server
 - *511* - The URI provided is invalid
 - *512* - The object you are saving is older than what is on the server. Use one of the KiiObject#save:forced: methods to forcibly override data on the server
 - *513* - The group name provided is not valid. Ensure it is alphanumeric and more than 0 characters in length
 - *514* - At least one of the ACL entries saved to an object failed. Please note there may also have been one or more successful entries
 - *515* - Bucket parent(user/group) of the bucket does not exist in the cloud.
 - *516* - The object you are trying to operate is illegal state. If you want to update KiiObject, please call <[KiiObject refreshSynchronous:]> before call this method.
 - *517* - The object body does not exist.
 - *518* - Unable to access file URL. May not exist, or may be a directory.
 - *519* - File URL is not writable.
 - *520* - File URL is not readable.
 - *521* - Invalid expiration date. It must be on the future.
 - *522* - Invalid expiration interval, should be greather than 0.
 - *523* - User has no refresh token
 - *524* - Valid group ID is a string composed of 1 to 100 characters include lower-case letters (a-z), numbers (0-9) and '.', '-', '_'.
 - *525* - Members must be an array of <KiiUser> objects.
 - *526* - Same ID group already exists.

 <h3>Query Errors (6xx)</h3>
 - *601* - No more query results exist
 - *602* - Query limit set too high
 - *603* - Query clauses is empty. Make sure "OR" and/or "AND" clauses have at least one correct sub-clauses
 - *604* - Query is not supported.
 
 <h3>Push Notification Errors (7xx)</h3>
 - *701* - Push installation error. Installation already exist
 - *702* - Push subscription already exists
 - *703* - Push subscription does not exist
 - *704* - Topic already exists
 - *705* - Topic does not exist
 - *706* - Invalid push message data
 - *707* - APNS field is required
 - *708* - Push data is required
 - *709* - Device token is not set
 - *710* - Push installation does not exist
 - *711* - Topic ID is invalid. Topic ID must be alphanumeric character and between 1-64 length
 - *712* - GCM payload key contains google reserved words
 - *713* - Topic parent(user/group) does not exist in the cloud.

 <h3>Resumable Transfer Errors (8xx)</h3>
 - *801* - Resumable transfer object has already transferred completely
 - *802* - File has been modified during transfer
 - *803* - File path is invalid, can not get the file attribute
 - *804* - File does not exist
 - *805* - File path is a directory
 - *806* - File size is 0 byte
 - *807* - Invalid transfer state, transfer is already suspended
 - *808* - Invalid transfer state, transfer is already terminated
 - *809* - Transfer was suspended
 - *810* - Transfer was terminated
 - *811* - Transfer has already started
 - *812* - Object body integrity not assured. ClientHash must be same during transfer.
 - *813* - Object body range not satisfiable. Transfer has terminated, please start transfer again.
 - *814* - File path is not writable.
 - *815* - Invalid destination file, file range is not assured
 - *816* - Unable to operate transfer manager. The transfer manager can not operate since current user is nil or different from the user who instantiate.

 <h3>AB Testing Errors (9xx)</h3>
 - *901* - Experiment with specified ID is not found.
 - *902* - The experiment is in draft. you need to run experiment before starting A/B testing.
 - *903* - The experiment has been paused.
 - *904* - The experiment has been terminated with no specified variation.
 - *905* - Variation with specified name is not found.
 - *906* - Failed to apply variation due to no user logged in.

 <h3>PhotoColle Errors (10xx)</h3>
 - *1001* - Unsupported MIME type for PhotoColle transfer.

 <h3>Thing Errors (11xx)</h3>
 - *1101* - Specified Thing is not found on the cloud.
 - *1102* - Specified Thing owner's (user/group) is not found on the cloud.
 - *1103* - Thing ownership for the given owner's (user/group) already exists.
 - *1104* - Thing ownership for the given owner's (user/group) does not exist.
 - *1105* - Thing already exists.
 - *1106* - Invalid input data for thing's field.
 
 
  <h3> Keychain Storage errors (12xx)</h3>
 - *1201* - >Unexpected Keychain errors.
 - *1202* - >Failed to unarchived object.
 - *1203* - >Failed to archived object.
 - *1204* - >Keychian item not found.
 - *1205* - >Not allowed to access keychain item.

 */
@interface KiiError : NSError

#pragma mark - static factory

+ (NSError*) errorWithCode:(NSInteger)code userInfo:(NSDictionary*)userInfo;

#pragma mark - 100 codes (Application Errors)
/* Application Errors (1xx) */

/* The application received invalid credentials and was not initialized. Make sure you have called [Kii begin...] with the proper app id and key before making any requests */
+ (NSInteger) codeInvalidApplication;

/* The application was not found on the server. Please ensure your app id and key were entered correctly. */

+ (NSInteger) codeAppNotFound;

/* The required operation is failed due to unexpected error. Should not thrown if using latest SDK. */

+ (NSInteger) codeUndefinedError;

#pragma mark - 200 codes (Connectivity Errors)
/* Connectivity Errors (2xx) */

/* Unable to connect to the internet */
+ (NSInteger) codeUnableToConnectToInternet;

/* Unable to parse server response */
+ (NSInteger) codeUnableToParseResponse;

/* Unable to authorize request */
+ (NSInteger) codeUnauthorizedRequest;

#pragma mark - 300 codes (User API Errors)
/* User API Errors (3xx) */

/* Unable to retrieve valid access token */
+ (NSInteger) codeInvalidAccessToken;

/* Unable to authenticate user */
+ (NSInteger) codeUnableToAuthenticateUser;

/* Unable to retrieve file list */
+ (NSInteger) codeUnableToRetrieveUserFileList;

/* Invalid password format. Password must be 4-50 printable ASCII characters */
+ (NSInteger) codeInvalidPasswordFormat;

/* Invalid email format. Email must be a valid address */
+ (NSInteger) codeInvalidEmailFormat;

/* Invalid format of userIdentifier */
+ (NSInteger) codeInvalidUserIdentifier;

/* Invalid username format. The username must be 3-64 alphanumeric characters - the first character must be a letter. */
+ (NSInteger) codeInvalidUsername;

/* Invalid user object. Please ensure the credentials were entered properly */
+ (NSInteger) codeInvalidUserObject;

/* Invalid phone format. The phone number must be numeric between 7 and 20 digits, and it can begin with '+'. */
+ (NSInteger) codeInvalidPhoneFormat;

/* Invalid Country code. 2-letters country code, capital letters*/
+ (NSInteger) codeInvalidCountryCode;
/* Invalid local phone format. The phone number numerical and must be at least 7 digits*/
+ (NSInteger) codeInvalidLocalPhoneFormat;

/* Invalid verification code */
+ (NSInteger) codeUnableToVerifyUser;

/* Invalid displayname format. The displayname length is 4-50 chars (not byte), and allow Multi-Byte input. */
+ (NSInteger) codeInvalidDisplayName;

/* The user's email was unable to be updated on the server */
+ (NSInteger) codeUnableToUpdateEmail;

/* The user's phone number was unable to be updated on the server */
+ (NSInteger) codeUnableToUpdatePhoneNumber;

/* The request could not be made - the key associated with the social network is invalid. */
+ (NSInteger) codeInvalidSocialNetworkKey;

/* Invalid credentials, please check whether the credentials associated with social network is valid */
+ (NSInteger) codeInvalidCredentials;

/* Social network account has been already linked */
+ (NSInteger) codeSocialAccountAlreadyLinked;

/* Social network account has is not linked */
+ (NSInteger) codeSocialAccountNotLinked;

/* Social network server-side authentication was canceled. */
+ (NSInteger) codeSocialNetworkAuthCanceled;

/* Social network server-side login failed with error. */
+ (NSInteger) codeServerSideLoginFailed;

/* Unable to load authentication page : <error message> */
+ (NSInteger) codeUnableToLoadAuthPage;

/* Unsupported Application structure. Server-side authentication needs rootViewController to be assigned on Application main window. */
+ (NSInteger) codeRootViewControllerNotSet;

/* User not found. The User object that is processed by the request is not available on the cloud. */
+ (NSInteger) codeUserNotFound;

/* Group not found. The Group object that is processed by the request is not available on the cloud.*/
+ (NSInteger) codeGroupNotFound;

/* Local phone number requires country code. */
+ (NSInteger) codeLocalPhoneRequiresCountryCode;

/* Non logged-in user can not use this method. */
+ (NSInteger) codeNonLoggedInUserIsNotAllowed;

/* Non pseudo user can not use this method. */
+ (NSInteger) codePseudoUserIsNotAllowed;

/* Invalid user status, the user should have either verified email or phone number. */
+ (NSInteger) codeInvalidUserStatus;

/* Fail to refresh acess token. */
+ (NSInteger) codeRefreshTokenFailed;

/* Stored credentials is invalid. */
+ (NSInteger) codeValidStoredUserNotFound;

/* ACAccount authentication is not granted by user*/
+ (NSInteger) codeForACAccountNotGranted;

/* ACAccount is not set by user*/
+ (NSInteger) codeForACAccountNotSet;

/* User is not logged, this operations does not allow anonymous user. */
+ (NSInteger) codeLoginRequired;

#pragma mark - 400 codes (File API Errors)
/* File API Errors (4xx) */

/* Unable to delete file from cloud */
+ (NSInteger) codeUnableToDeleteFile;

/* Unable to upload file to cloud */
+ (NSInteger) codeUnableToUploadFile;

/* Unable to retrieve local file for uploading. May not exist, or may be a directory. */
+ (NSInteger) codeLocalFileInvalid;

/* Unable to shred file. Must be in the trash before it is permanently deleted. */
+ (NSInteger) codeShreddedFileMustBeInTrash;

/* Insufficient space in cloud to store data */
+ (NSInteger) codeFileContainerNotSpecified;

+ (NSInteger) codeInsufficientSpaceInCloud;

#pragma mark - 500 codes (Object Errors)
/* Core Object Errors (5xx) */

/* Invalid objects passed to method. Must be already saved on server. */
+ (NSInteger) codeInvalidObjects;

/* Unable to parse object. Must be JSON-encodable */
+ (NSInteger) codeUnableToParseObject;

/* Duplicate entry exists */
+ (NSInteger) codeDuplicateEntry;

/* Invalid remote path set for KiiFile. Must be of form:  /root/path/subpath    */
+ (NSInteger) codeInvalidRemotePath;

/* Unable to delete object from cloud */
+ (NSInteger) codeUnableToDeleteObject;

/* Invalid KiiObject - the class name contains one or more spaces */
+ (NSInteger) codeInvalidObjectType;

/* Unable to set an object as a child of itself */
+ (NSInteger) codeUnableToSetObjectToItself;

/* The key of the object being set is a preferred key, please try a different key */
+ (NSInteger) codeInvalidAttributeKey;

/* The container you are trying to operate on does not exist */
+ (NSInteger) codeInvalidContainer;

/* The object you are trying to operate on does not exist */
+ (NSInteger) codeObjectNotFound;

/* The URI provided is invalid */
+ (NSInteger) codeInvalidURI;

/* The group name provided is not valid. Ensure it is alphanumeric and more than 0 characters in length */
+ (NSInteger) codeInvalidGroupName ;

/* At least one of the ACL entries saved to an object failed. Please note there may also have been one or more successful entries. */
+ (NSInteger) codePartialACLFailure;

/* Bucket parent of the bucket(user/group) does not exist in the cloud. */
+ (NSInteger) codeBucketParentNotExistInCloud;

/* The object you are trying to operate is illegal state. If you want to update KiiObject, please call <[KiiObject refreshSynchronous:]> before call this method. */
+ (NSInteger) codeIllegalStateObject;

/* Object body does not exist */
+ (NSInteger) codeObjectBodyNotExistInCloud;

/* Unable to access file URL. May not exist, or may be a directory.*/
+ (NSInteger) codeNotAccessibleURL;

/* File URL is not writable. */
+ (NSInteger) codeNotWritableURL;

/* File URL is not readable. */

+ (NSInteger) codeNotReadableURL;

/* Invalid date, date is not future */
+ (NSInteger) codeDateNotFuture;

/* Invalid interval, should be greater than zero */
+ (NSInteger) codeIntervalZero;

/* User has no refresh token */
+ (NSInteger) codeNonRefreshToken;

/* Valid group ID is a string composed of 1 to 100 characters include
   lower-case letters (a-z), numbers (0-9) and '.', '-', '_'. */
+ (NSInteger) codeInvalidGroupID;

/* Members must be an array of <KiiUser> objects. */
+ (NSInteger) codeMemberIsNotKiiUser;

/* Same ID group already exists. */
+ (NSInteger) codeGroupAlreadyExists;

#pragma mark - 600 codes (Query errors)
/* Query Errors (6xx) */

/* No more query results exist */
+ (NSInteger) codeNoMoreResults;

/* Query limit set too high */
+ (NSInteger) codeSingleQueryLimitExceeded;

+ (NSInteger) codeEmptyQueryClauses;

+ (NSInteger) codeQueryNotSupported;

/* Push Notification Errors (7xx) */

/*Push installation error. Installation already exist*/
+ (NSInteger) codeInstallationAlreadyExist;

/* Push subscription already exist */
+ (NSInteger) codeSubscriptionAlreadyExist;

/*Push subscription does not exist*/
+ (NSInteger) codeSubscriptionNotExist;

/* Topic already exists */
+ (NSInteger) codeTopicAlreadyExist;

/* Topic does not exist */
+ (NSInteger) codeTopicNotExist;

/* invalid push message data */
+ (NSInteger) codeInvalidPushMessageData;

/* APNS field is required */
+ (NSInteger) codeApnsFieldRequired;

/* Push data is required */
+ (NSInteger) codePushDataRequired;

/* Device token is not set*/
+ (NSInteger) codeDeviceTokenNotSet;

/* Installation does not exist*/
+ (NSInteger) codeInstallationNotFound;

/* Topic ID is invalid. Topic ID must be alphanumeric character and between 1-64 length.*/
+ (NSInteger) codeInvalidTopicID;

/* GCM payload key contains google reserved words.*/
+ (NSInteger) codeContainsGCMReservedKey;

/* Topic parent(user/group) does not exist in the cloud.*/
+ (NSInteger) codeTopicParentNotExistInCloud;

#pragma mark - 800 codes (Resumable Transfer Errors)
/* Resumable Transfer Errors (8xx) */

/* Resumable transfer object has already transferred completely*/
+ (NSInteger) codeTransferAlreadyCompleted;

/* File has been modified during transfer*/
+ (NSInteger) codeFileModifiedDuringTransfer;

/* File path is invalid, can not get the file attribute*/
+ (NSInteger) codeCannotGetFileAttribute;

/* File does not exist*/
+ (NSInteger) codeFileNotExist;

/* File path is a directory*/
+ (NSInteger) codeFilePathIsDirectory;

/* File size is 0 byte*/
+ (NSInteger) codeFileSizeIsZeroByte;

/* Transfer is already suspended*/
+ (NSInteger) codeTransferAlreadySuspended;

/* Transfer is already terminated*/
+ (NSInteger) codeTransferAlreadyTerminated;

/* Transfer was suspended*/
+ (NSInteger) codeTransferWasSuspended;

/* Transfer was terminated*/
+ (NSInteger) codeTransferWasTerminated;

/* Transfer has already started*/
+ (NSInteger) codeTransferAlreadyStarted;

/* Object body integrity not assured. ClientHash must be same during transfer. */
+ (NSInteger) codeObjectBodyIntegrityNotAssured;

/* Object body range not satisfiable. Transfer has terminated, please start transfer again. */
+ (NSInteger) codeObjectBodyRangeNotSatisfiable;

/* File path is not writable */
+ (NSInteger) codeFilePathIsNotWritable;

/* Invalid destination file, file range is not assured */
+ (NSInteger) codeFileRangeNotValid;

/* Unable to operate transfer manager. The transfer manager can not operate since current user is nil or different from the user who instantiate. */
+ (NSInteger) codeUnableToOperateTransferManager;

#pragma mark - 900 codes (AB testing errors)
/* AB Testing Errors (9xx) */

/**Experiment with specified ID is not found.
 */
+ (NSInteger) codeExperimentNotFound;

/** The experiment has been terminated with no specified variation.
 */
+ (NSInteger) codeExperimentTerminatedWithNoVariation;

/** The experiment has been paused.
 */
+ (NSInteger) codeExperimentPaused;

/** The experiment is in draft. you need to run experiment before starting A/B testing.
 */
+ (NSInteger) codeExperimentIsInDraft;

/** Variation with specified name is not found.
 */
+ (NSInteger) codeVariationNotFound;

/** Failed to apply variation due to no user logged in.
 */
+ (NSInteger) codeFailedToApplyVariationDueToNoUserLoggedIn;

#pragma mark - 1000 codes (PhotoColle errors)
/* PhotoColle errors (1XXX) */

/* Unsupported MIME type for PhotoColle transfer. */
+ (NSInteger) codeUnsupportedMIMETypeForPhotoColleTransfer;

#pragma mark - 1100 codes (Thing errors)

/** Specified Thing is not found on the cloud.*/
+ (NSInteger) codeThingNotFound;

/** Specified Thing owner's (user/group) is not found on the cloud.*/
+ (NSInteger) codeThingOwnerNotFound;


/** Thing ownership for the given owner's (user/group) does not exist.*/
+ (NSInteger) codeThingOwnershipNotFound;

/** Thing ownership for the given owner's (user/group) already exists.*/
+ (NSInteger) codeThingOwnershipExist;

/** Thing already exists.*/
+ (NSInteger) codeThingExist;

/** Invalid input data for thing's field.*/
+ (NSInteger) codeThingInvalidData;
/* Keychain storage errors (11XX) */

/* Unexpected KeyChain error happends. */
+ (NSInteger) codeUnexpectedKeyChainError;

/* Failed to unarchived object */
+ (NSInteger) codeFailedToUnarchived;

/* Failed to archived object */
+ (NSInteger) codeFailedToArchived;

/* No keychain item found */
+ (NSInteger) codeNonKeychainItemFound;

/* Accessing keychian item is not allowed */
+ (NSInteger) codeKeyChainAccessIsNotAllowed;

/* Input data is invalid */
+ (NSInteger) codeInvalidRequestData;

@end

#pragma mark - 
#pragma mark NSError Kii Categories

@interface NSError (KiiError)

/** Error summary returned from Kii Cloud. Please refer to Kii Cloud REST API [documentation](http://docs.kii.com/rest/) for the details.
 
 The corresponding field is 'errorCode' in the response body.
 This methods is equals to error.userInfo[@"server_code"].
 
 @return Error summary returned from Kii Cloud or nil if the error is not returned
 from Kii Cloud (network/ validation error, etc.).
 */
-(NSString*) kiiErrorSummary;

/** Error message from the server, please refer to Kii API REST [documentation](http://docs.kii.com/rest/) for the details.
 
 This methods is equals to error.userInfo[@"server_message"].
 
 @return Code from cloud server or nil if it there is no error message returned from from Kii Cloud (network/ validation error, etc.).
 */
-(NSString*) kiiErrorMessage;

/** HTTP response code returned from Kii Cloud, please refer to Kii API REST [documentation](http://docs.kii.com/rest/) for the details.
 
 This methods is equals to error.userInfo[@"http_status"].
 
 @return HTTP response code from the cloud or 0 if the error is not returned from Kii Cloud (network/ validation error, etc.).
 */
-(NSInteger) kiiHttpStatus;
@end
