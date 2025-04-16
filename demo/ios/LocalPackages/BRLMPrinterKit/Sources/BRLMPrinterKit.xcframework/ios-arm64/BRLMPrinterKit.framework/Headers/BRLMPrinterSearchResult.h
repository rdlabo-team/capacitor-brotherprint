//
//  BRLMPrinterSearchResult.h
//  BRLMPrinterKit
//
//  Copyright © 2022 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMChannel.h"
#import "BRLMPrinterSearchError.h"

typedef NS_ENUM(NSInteger, BRLMSearchPrintErrorCode) {
    BRLMSearchPrintErrorCodeNoError = 0,
};

NS_ASSUME_NONNULL_BEGIN

@interface BRLMPrinterSearchResult : NSObject
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

@property (nonatomic, readonly) NSArray<BRLMChannel *>* channels;
@property (nonatomic, readonly) BRLMPrinterSearchError* error;

@end

NS_ASSUME_NONNULL_END
