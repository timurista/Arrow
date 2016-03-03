//
//  KiiPushMessage.h
//  KiiSDK-Private
//
//  Created by Riza Alaudin Syah on 1/24/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
/** Enumeration of KiiMessageScope that represents KiiPushMessage Object/Topic Scope.
 */
typedef NS_ENUM(NSInteger, KiiMessageScope) {
    
    /** Scope is not provided */
    KiiMessageScope_UNKNOWN = -1,
    /** App Scope */
    KiiMessageScope_APP,
    /** Group Scope */
    KiiMessageScope_APP_AND_GROUP,
    /** User Scope */
    KiiMessageScope_APP_AND_USER,
    /** Thing Scope */
    KiiMessageScope_APP_AND_THING,
};

/** Enumeration of KiiMessageField that represents KiiCloud specific fields.

 KiiMessageField list is as follows:

  Key           | Short key | Push to App | Push to User                        | Direct Push                           | Description                                          | Possible values
 ---------------|-----------|-------------|-------------------------------------|---------------------------------------|------------------------------------------------------|------
 KiiMessage_APP_ID         | a         | -           | -<br>(Depends on "sendAppID")       | -                                     | Source app which generated the notification.         |
 KiiMessage_SENDER         | s         | -           | X<br>(Depends on "sendSender")      | X<br>(Depends on "sendSender")        | The user or thing who caused the notification.                |
 KiiMessage_ORIGIN         | o         | -           | -<br>(Depends on "sendOrigin")      | -<br>(Depends on "sendOrigin")        | Origin of push. "EVENT" for "Push to App" notification. "EXPLICIT" for "Push to User" and "Direct Push" notification. | - EVENT<br> - EXPLICIT
 KiiMessage_WHEN           | w         | X           | -<br>(Depends on "sendWhen")        | -<br>(Depends on "sendWhen")          | The timestamp of the notification in milliseconds. (Since January 1, 1970 00:00:00 UTC) |
 KiiMessage_TYPE           | t         | X           | -<br>(Depends on "pushMessageType") | -<br>(Depends on "pushMessageType")   | The type of notification and the additional data.    | [Push to App]<br>- DATA_OBJECT_CREATED<br> - DATA_OBJECT_DELETED<br> - DATA_OBJECT_UPDATED<br> - DATA_OBJECT_BODY_UPDATED<br> - DATA_OBJECT_BODY_DELETED<br> - DATA_OBJECT_ACL_MODIFIED<br> - BUCKET_DELETED
 KiiMessage_TOPIC          | to        | -           | X<br>(Depends on "sendTopicID")     | -                                     | TopicID that is the source of this notification. TopicID is only for "Push to User" push messages.    |
 KiiMessage_SCOPE_APP_ID   | sa        | X           | X<br>(Depends on "sendObjectScope") | -                                     | AppID of topic/object scope.                               |
 KiiMessage_SCOPE_USER_ID  | su        | X           | X<br>(Depends on "sendObjectScope") | -                                     | UserID of topic/topic/object scope. Push message has this field only if the subscribed bucket/topic is user scope.|
 KiiMessage_SCOPE_GROUP_ID | sg        | X           | X<br>(Depends on "sendObjectScope") | -                                     | GroupID of topic/object scope. Push message has this field only if the subscribed bucket/topic is group scope. |
 KiiMessage_SCOPE_THING_ID | sth        | X           | X<br>(Depends on "sendObjectScope") | -                                     | ThingID of topic/object scope. Push message has this field only if the subscribed bucket/topic is thing scope. |
 KiiMessage_SCOPE_TYPE     | st        | X           | X<br>(Depends on "sendObjectScope") | -                                     | Type of topic/object scope.                                 | - APP<br> - APP_AND_USER<br> - APP_AND_GROUP<br> - APP_AND_THING
 KiiMessage_BUCKET_ID      | bi        | X           | -                                   | -                                     | Bucket name of push subscribed.                      |
 KiiMessage_BUCKET_TYPE    | bt        | X           | -                                   | -                                     | Type of source bucket.                    | - rw<br> - sync
 KiiMessage_OBJECT_ID      | oi        | X           | -                                   | -                                     | ID of the object operated.                           |
 KiiMessage_OBJECT_MODIFIED_AT | om    | -           | -                                   | -                                     | Timestamp of the modification of object in milliseconds. (Since January 1, 1970 00:00:00 UTC)  | - DATA_OBJECT_CREATED<br> - DATA_OBJECT_UPDATED<br> - DATA_OBJECT_BODY_UPDATE

 */
typedef NS_ENUM(NSUInteger, KiiMessageField) {
    /** Source app which generated the notification. */
    KiiMessage_APP_ID,
    /** The user who caused the notification. */
    KiiMessage_SENDER,
    /** The type of notification and the additional data. */
    KiiMessage_TYPE,
    /** The timestamp of the notification in milliseconds.
     (Since January 1, 1970 00:00:00 UTC) */
    KiiMessage_WHEN,
    /** Origin of push. "EVENT" for "Push to App" notification.
     "EXPLICIT" for "Push to User" and "Direct Push" notification. */
    KiiMessage_ORIGIN,
    /** TopicID that is the source of this notification.
     TopicID is only for "Push to User" push messages. */
    KiiMessage_TOPIC,
    /** AppID of topic/object scope. */
    KiiMessage_SCOPE_APP_ID,
    /** UserID of topic/object scope.
     Push message has this field only if the subscribed bucket/topic is user scope. */
    KiiMessage_SCOPE_USER_ID,
    /** GroupID of topic/object scope.
     Push message has this field only if the subscribed bucket/topic is group scope. */
    KiiMessage_SCOPE_GROUP_ID,
    /** Type of topic/object scope. */
    KiiMessage_SCOPE_TYPE,
    /** Bucket name of push subscribed. */
    KiiMessage_BUCKET_ID,
    /** Type of source bucket. */
    KiiMessage_BUCKET_TYPE,
    /** ID of the object operated. */
    KiiMessage_OBJECT_ID,
    /** Timestamp of the modification of object in milliseconds.
     (Since January 1, 1970 00:00:00 UTC) */
    KiiMessage_OBJECT_MODIFIED_AT,
    /** ThingID of topic/object scope.
     Push message has this field only if the subscribed bucket/topic is thing scope. */
    KiiMessage_SCOPE_THING_ID
};

@class KiiAPNSFields,KiiGCMFields,KiiMQTTFields,KiiBucket,KiiObject,KiiTopic,KiiUser,KiiThing,KiiGroup;

/**
 Class for encapsulating incoming and outgoing push notification message
Three types of push message supported by KiiCloud.

- **Push to App** : Message sent to the subscribers when an event happens in the <KiiBucket>.
- **Push to User** : Message sent to the subscribers of <KiiTopic> that is created explicitly.
- **Direct Push** : Message sent to a certain user by manipulating the developer portal. (Only app developer can send this message.)

Following section describes the contents of "Push to App", "Push to User" and "Direct Push" message.

### Contents of Push Message

Push message of "Push to App", "Push to User" and "Direct Push" contains the KiiCloud specific fields that are selected by the message sender.
And also push message of "Push to User" contains the basic message as key-value pair that is sent to the <KiiTopic>.

KiiCloud specific fields are as follows: 

  Key           | Short key | Push to App | Push to User                        | Direct Push                           | Description                                          | Possible values
 ---------------|-----------|-------------|-------------------------------------|---------------------------------------|------------------------------------------------------|------
 KiiMessage_APP_ID         | a         | -           | -<br>(Depends on "sendAppID")       | -                                     | Source app which generated the notification.         |
 KiiMessage_SENDER         | s         | -           | X<br>(Depends on "sendSender")      | X<br>(Depends on "sendSender")        | The user or thing who caused the notification.                |
 KiiMessage_ORIGIN         | o         | -           | -<br>(Depends on "sendOrigin")      | -<br>(Depends on "sendOrigin")        | Origin of push. "EVENT" for "Push to App" notification. "EXPLICIT" for "Push to User" notification. | - EVENT<br> - EXPLICIT
 KiiMessage_WHEN           | w         | X           | -<br>(Depends on "sendWhen")        | -<br>(Depends on "sendWhen")          | The timestamp of the notification in milliseconds. (Since January 1, 1970 00:00:00 UTC) |
 KiiMessage_TYPE           | t         | X           | -<br>(Depends on "pushMessageType") | -<br>(Depends on "pushMessageType")   | The type of notification and the additional data.    | [Push to App]<br>- DATA_OBJECT_CREATED<br> - DATA_OBJECT_DELETED<br> - DATA_OBJECT_UPDATED<br> - DATA_OBJECT_BODY_UPDATED<br> - DATA_OBJECT_BODY_DELETED<br> - DATA_OBJECT_ACL_MODIFIED<br> - BUCKET_DELETED
 KiiMessage_TOPIC          | to        | -           | X<br>(Depends on "sendTopicID")     | -                                     | TopicID is only for "Push to User" push messages.    |
 KiiMessage_SCOPE_APP_ID   | sa        | X           | X<br>(Depends on "sendObjectScope") | -                                     | AppID of topic/object scope.                               |
 KiiMessage_SCOPE_USER_ID  | su        | X           | X<br>(Depends on "sendObjectScope") | -                                     | UserID of topic/object scope. Push message has this field only if the subscribed bucket/topic is user scope.|
 KiiMessage_SCOPE_GROUP_ID | sg        | X           | X<br>(Depends on "sendObjectScope") | -                                     | GroupID of topic/object scope. Push message has this field only if the subscribed bucket/topic is group scope. |
 KiiMessage_SCOPE_THING_ID | sth        | X           | X<br>(Depends on "sendObjectScope") | -                                     | ThingID of topic/object scope. Push message has this field only if the subscribed bucket/topic is thing scope. |
 KiiMessage_SCOPE_TYPE     | st        | X           | X<br>(Depends on "sendObjectScope") | -                                     | Type of topic/object scope.                                 | - APP<br> - APP_AND_USER<br> - APP_AND_GROUP<br> - APP_AND_THING
 KiiMessage_BUCKET_ID      | bi        | X           | -                                   | -                                     | Bucket name of push subscribed.                      |
 KiiMessage_BUCKET_TYPE    | bt        | X           | -                                   | -                                     | Type of bucket has been modified.                    | - rw<br> - sync
 KiiMessage_OBJECT_ID      | oi        | X           | -                                   | -                                     | ID of the object operated.                           |
 KiiMessage_OBJECT_MODIFIED_AT | om    | -           | -                                   | -                                     | Timestamp of the modification of object in milliseconds. (Since January 1, 1970 00:00:00 UTC)  | - DATA_OBJECT_CREATED<br> - DATA_OBJECT_UPDATED<br> - DATA_OBJECT_BODY_UPDATE
 
 ### GCM restriction for reserved keyword
 Based on Google GCM specification, there are reserved payload keys that should not be used inside data/specific data.
 If GCM is enabled and the data contains one or more reserve keys, an error (code 712) will be thrown.
 Following are the list of GCM reserved keys:
 
    - any key prefix with 'google'
    - from
    - registration_ids
    - collapse_key
    - data
    - delay_while_idle
    - time_to_live
    - restricted_package_name
    - dry_run

 */
@interface KiiPushMessage : NSObject

/** Dictionary representation of APNs received message.
*/
@property(nonatomic,readonly) NSDictionary* rawMessage;

/** Dictionary representation of JSON Object with only one-level of nesting. Required if no system-specific “data” fields has been provided for all the systems enabled.	Dictionary with the data that will be sent to all the push systems enabled in this request.
 If gcmFields is defined, the data would be validated for GCM reserved payload keys. 
 */
@property(nonatomic,strong) NSDictionary* data;

/** APNS-specific fields.
 */
@property(nonatomic,strong) KiiAPNSFields* apnsFields;

/** GCM-specific fields.
 */
@property(nonatomic,strong) KiiGCMFields* gcmFields;

/** MQTT-specific fields.
 */
@property(nonatomic,strong) KiiMQTTFields* mqttFields;

/**Boolean. Not required.
 If true this message will be sent to the devices that have the property "development" to "true" in their installations. Default is true.
 */
@property(nonatomic,strong) NSNumber* sendToDevelopment;

/** Boolean. Not required.
 If true this message will be sent to the devices that have the property "development" to "false" or nil in their installations. Default is true.
 */
@property(nonatomic,strong) NSNumber* sendToProduction;

/** String. Not required.
 Value that will optionally indicate what is the type of the message. Event-generated push messages contain it.
 */
@property(nonatomic,strong) NSString* pushMessageType;

/** Boolean. Not required.
 If true, the appID field will also be sent. Default is false.
 */
@property(nonatomic,strong) NSNumber* sendAppID;

/**Boolean. Not required.
 If true, send the “sender” field (userID of the user that triggered the notification). Default is true.
 */
@property(nonatomic,strong) NSNumber* sendSender;

/** Boolean. Not required.
 If true, send the “when” field (when the push message was sent). Default is false.
 */
@property(nonatomic,strong) NSNumber* sendWhen;

/**Boolean. Not required.
 If true, send the “origin” field (indicates if the message is the result of an event or sent explicitly by someone. Default is false.
 */
@property(nonatomic,strong) NSNumber* sendOrigin;

/**Boolean. Not required.
 If true, send the “objectScope”-related fields that contain the topic that is the source of this notification. Default is true.
 */
@property(nonatomic,strong) NSNumber* sendObjectScope;

/** Boolean. Not required.
 If true, send the “topicID” field, which contains the topicID that is the source of this notification. Default is true.
 */
@property(nonatomic,strong) NSNumber* sendTopicID;

/** Parse incoming APNs message.
 @param userInfo An userInfo instance that received from APNs as a push message.
 @return A KiiPushMessage instance that is associated to the userInfo.
 */
+(KiiPushMessage*) messageFromAPNS:(NSDictionary*) userInfo;

/** Constructor method that composes a message for explicit push
 @param apnsfields The message data for APNS
 @param gcmfields The message data for GCM
 @return A KiiPushMessage instance that is associated to the message data.
 */
+(KiiPushMessage*) composeMessageWithAPNSFields:(KiiAPNSFields*) apnsfields andGCMFields:(KiiGCMFields*)gcmfields;

/** Constructor method that composes a message for explicit push.
 @param apnsFields The message data for APNS.
 @param KiiGCMFields The message data for GCM.
 @param mqttFields The message data for MQTT.
 @return A KiiPushMessage instance that is associated to the message data.
 */
+(instancetype) composeMessageWithAPNSFields:(KiiAPNSFields*) apnsFields
                                   andGCMFields:(KiiGCMFields*)gcmFields
                                  andMQTTFields:(KiiMQTTFields*) mqttFields;

/** Get specific value of received message meta data.
@param field Enumeration of <KiiMessageField>.
@return A NSString object that is associated to the message fields.
*/
-(NSString*) getValueOfKiiMessageField:(KiiMessageField) field;

/** Get alert body's text message.
 @return A NSString object of alert body text message.
 */
-(NSString*) getAlertBody;

/** Show simple alert to display alert body's message.
 @param title A alert title to display.
 */
-(void) showMessageAlertWithTitle:(NSString*) title;

/**Obtain <KiiBucket> based on the information parsed from push message.
 
 The payloads can contain the subscribed bucket informations including the scope and the type (encrypted or not). This API provides convenience methods to obtain <KiiBucket> if the payload contains the subscribed bucket information.
 
 @return <KiiBucket> instance when the event of KiiBucket or <KiiObject> inside <KiiBucket> happened. In other cases returns nil.
 */
-(KiiBucket*) eventSourceBucket;

/**Obtain <KiiObject> based on the information parsed from push message.
 
 The payloads can contain the subscribed bucket's object informations including the scope. This API provides convenience methods to obtain <KiiObject> if the payload contains the object information.
 
 @return <KiiObject> instance when the event of <KiiObject>
 * inside <KiiBucket> happened. In other cases returns nil.
 */
-(KiiObject*) eventSourceObject;

/**Obtain <KiiTopic> based on the information parsed from push message.
 
 The payloads can contain the subscribed topic informations including the scope. This API provides convenience methods to obtain <KiiTopic>  if the payload contains the subscribed topic information.
 
 @return <KiiTopic> instance when the event of KiiTopic (Push to User). In other cases returns nil.
 */
-(KiiTopic*) eventSourceTopic;

/**Obtain <KiiUser> based on the information parsed from push message.
 
 The payloads can contain the subscribed bucket/topic informations including the scope. This API provides convenience methods to obtain <KiiUser> if the payload contains user scoped bucket/topic information.
 
 @return <KiiUser> instance when the subscribed bucket/topic is user scope. In other cases returns nil.
 */
-(KiiUser*) eventSourceUser;

/**Obtain <KiiThing> based on the information parsed from push message.
 
 The payloads can contain the subscribed bucket/topic informations including the scope. This API provides convenience methods to obtain <KiiThing> if the payload contains thing scoped bucket/topic information.
 
 @return <KiiThing> instance when the subscribed bucket/topic is thing scope. In other cases returns nil.
 */
-(KiiThing*) eventSourceThing;

/**Obtain <KiiGroup> based on the information parsed from push message scope.
 
 The payloads can contain the subscribed bucket/topic informations including the scope. This API provides convenience methods to obtain <KiiGroup> if the payload contains group scoped bucket/topic information.
 
 @return <KiiGroup> instance when the subscribed bucket/topic is group scope. In other cases returns nil.
 */
-(KiiGroup*) eventSourceGroup;

/**Obtain KiiUser based on the information parsed from push message .
 
 @return <KiiUser> instance when the sender of the message is a <KiiUser>. In other cases returns nil.
 */
-(KiiUser*) senderUser;

/**Obtain <KiiThing> based on the information parsed from push message.
 
 @return <KiiThing> instance when the sender of the message is a <KiiThing>. In other cases returns nil.
 */

-(KiiThing*) senderThing;
/**
 * Checks whether the push message contains <KiiBucket> or not.
 
 @return YES if the push message contains <KiiBucket>, otherwise NO.
 *
 */
-(BOOL) containsKiiBucket;

/**
 * Checks whether the push message contains <KiiObject> or not.
 
 @return YES if the push message contains <KiiObject>, otherwise NO.
 */
-(BOOL) containsKiiObject;

/**
 * Checks whether the push message contains <KiiTopic> or not.
 
 @return YES if the push message contains <KiiTopic>, otherwise NO.
 */
-(BOOL) containsKiiTopic;

/**Obtain scope of the bucket/topic.
 
 @return enumeration of message scope. If not provided, it will return KiiMessageScope_UNKNOWN;
 */
-(KiiMessageScope) messageScope;

@end
