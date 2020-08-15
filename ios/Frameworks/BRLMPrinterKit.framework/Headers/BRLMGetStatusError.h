//
//  BRLMGetStatusError.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import "BRLMError.h"

typedef NS_ENUM(NSInteger, BRLMGetStatusErrorCode) {
    BRLMGetStatusErrorCodeNoError,
    BRLMGetStatusErrorCodePrinterNotFound,
    BRLMGetStatusErrorCodeTimeout,
};

NS_ASSUME_NONNULL_BEGIN

@interface BRLMGetStatusError : BRLMError

@property(nonatomic, readonly) BRLMGetStatusErrorCode code;

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

@end

NS_ASSUME_NONNULL_END
