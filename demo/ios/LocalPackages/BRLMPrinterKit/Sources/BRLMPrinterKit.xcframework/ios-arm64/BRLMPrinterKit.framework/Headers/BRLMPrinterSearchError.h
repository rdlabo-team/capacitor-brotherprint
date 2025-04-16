//
//  BRLMPrinterSearchError.h
//  BRLMPrinterKit
//
//  Copyright © 2022 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, BRLMPrinterSearchErrorCode) {
    BRLMPrinterSearchErrorNoError = 30100,
    BRLMPrinterSearchErrorCanceled,
    BRLMPrinterSearchErrorAlreadySearching,
    BRLMPrinterSearchErrorUnsupported,
    BRLMPrinterSearchErrorUnknownError,
};


NS_ASSUME_NONNULL_BEGIN

@interface BRLMPrinterSearchError : NSObject
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@property(nonatomic, readonly)BRLMPrinterSearchErrorCode code;
@end

NS_ASSUME_NONNULL_END
