//
//  BRLMPrinterAddress.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMPrinterDefine.h"

// Set the "serialNumber" that can be getting from EAAccessory.
// Or set the "Bluetooth Address" that can be getting from your printer (*by self-printing and so on).
// Please note that it is NOT the serial number of your Printer.
typedef NSString BRLMExternalAccessorySerialNumber;


NS_ASSUME_NONNULL_BEGIN

@interface BRLMChannel : NSObject

@property (nonatomic, readonly) BRLMChannelType channelType;
@property (nonatomic, readonly) NSString *channelInfo;

- (instancetype)initWithWifiIPAddress:(NSString *)ipAddress;
- (instancetype)initWithBluetoothSerialNumber:(BRLMExternalAccessorySerialNumber *)serialNumber;
- (instancetype)initWithBLELocalName:(NSString *)localName;

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

@end

NS_ASSUME_NONNULL_END
