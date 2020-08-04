//
//  BRPtouchBluetoothManager.h
//  BRSearchModule
//
//  Copyright (c) 2015-2018 Brother Industries, Ltd. All rights reserved.
//


#import <Foundation/Foundation.h>

extern NSString *BRDeviceDidConnectNotification;
extern NSString *BRDeviceDidDisconnectNotification;
extern NSString *BRDeviceKey;


@interface BRPtouchBluetoothManager : NSObject



/*!
 * @brief Returns the shared BRPtouchBluetoothManager object for the iOS-based device
 * @discussion You should always use this method to obtain the bluetooth manager object and should not try to create instances directly.
 * @return The shared bluetooth manager object.
 */
+ (BRPtouchBluetoothManager *)sharedManager;



/*!
 * @brief The device objects corresponding to the list of currently connected brother devices
 * @discussion This property contains an array of BRPtouchDeviceInfo objects. Each object corresponds to a brother device that is connected and available for your application to use. Because the contents of this property can change dynamically based on the connection and disconnection of devices, you should not cache the value of this property.
 * @return The device objects corresponding to the list of currently connected brother devices.
 */
- (NSArray *)pairedDevices;



/*!
 * @brief Begins the dilivery of brother-device-related notification to the current application.
 * @discussion You must call this method if you want to be notified when brother devices become connected or disconnected. The system does not send these notifications automatically, so calling this method lets the Brother SDK know that your application is interested in them. Typically, you would call this method only once early in your application, either before or after configuring your notification observers. When you no longer need to monitor these notifications, you should call the matching unregisterForBRDeviceNotifications method.
 */
- (void)registerForBRDeviceNotifications;



/*!
 * @brief Stop the delivery of brother-device-related notification to the current application
 * @discussion Typically, you would call this method either when your application exits or when you no longer want to receive brother-device-related notifications. Calls to this method must be balanced with a preceding call to the registerForBRDeviceNotifications method.
 */
- (void)unregisterForBRDeviceNotifications;


/**
 * Use Bluetooth DeviceSearch.
 **/
- (void)brShowBluetoothAccessoryPickerWithNameFilter:(NSPredicate *)predicate;


@end
