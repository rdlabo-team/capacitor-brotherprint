//
//  BRLMChannel.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BRLMPrinterKit/BRLMPrinterDefine.h>

typedef NSString BRLMExternalAccessorySerialNumber;

typedef NSString BRLMChannelExtraInfoKey;

extern BRLMChannelExtraInfoKey * _Nonnull const BRLMChannelExtraInfoKeyModelName;
extern BRLMChannelExtraInfoKey * _Nonnull const BRLMChannelExtraInfoKeySerialNumber;
extern BRLMChannelExtraInfoKey * _Nonnull const BRLMChannelExtraInfoKeyMacAddress;
extern BRLMChannelExtraInfoKey * _Nonnull const BRLMChannelExtraInfoKeyNodeName;
extern BRLMChannelExtraInfoKey * _Nonnull const BRLMChannelExtraInfoKeyLocation;


NS_ASSUME_NONNULL_BEGIN

@interface BRLMChannel : NSObject

@property (nonatomic, readonly) BRLMChannelType channelType;
@property (nonatomic, readonly) NSString *channelInfo;
@property (nonatomic, readonly, nullable) NSMutableDictionary<BRLMChannelExtraInfoKey*, NSString*> *extraInfo;


- (instancetype)initWithWifiIPAddress:(NSString *)ipAddress;
- (instancetype)initWithBluetoothSerialNumber:(BRLMExternalAccessorySerialNumber *)serialNumber;
- (instancetype)initWithBLELocalName:(NSString *)localName;

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

@end

NS_ASSUME_NONNULL_END
