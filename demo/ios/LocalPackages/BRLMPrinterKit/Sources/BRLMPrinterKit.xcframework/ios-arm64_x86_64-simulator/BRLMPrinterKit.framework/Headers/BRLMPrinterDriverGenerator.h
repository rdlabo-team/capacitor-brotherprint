//
//  BRLMPrinterDriverGenerator.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <BRLMPrinterKit/BRLMPrinterDriver.h>
#import <BRLMPrinterKit/BRLMChannel.h>
#import <BRLMPrinterKit/BRLMOpenChannelError.h>

NS_ASSUME_NONNULL_BEGIN

@interface BRLMPrinterDriverGenerateResult : NSObject
@property (nonatomic, readonly) BRLMOpenChannelError *error;
@property (nonatomic, readonly, nullable) BRLMPrinterDriver *driver;
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@end

@interface BRLMPrinterDriverGenerator : NSObject

@property(nonatomic, readonly, nullable) BRLMOpenChannelError *openChannelError;

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

+ (BRLMPrinterDriverGenerateResult *)openChannel:(BRLMChannel *)channel;

@end

NS_ASSUME_NONNULL_END
