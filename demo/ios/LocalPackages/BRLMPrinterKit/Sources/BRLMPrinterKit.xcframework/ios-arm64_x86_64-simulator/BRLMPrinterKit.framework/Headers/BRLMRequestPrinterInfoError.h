//
//  BRLMRequestPrinterInfoError.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2023/05/30.
//  Copyright © 2023 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMPrintError.h"

typedef NS_ENUM(NSUInteger, BRLMRequestPrinterInfoErrorCode) {
    BRLMRequestPrinterInfoErrorNoError = 20100,
    BRLMRequestPrinterInfoErrorConnectionFailed,
    BRLMRequestPrinterInfoErrorUnsupported,
    BRLMRequestPrinterInfoErrorUnknownError,
};

@interface BRLMRequestPrinterInfoError : NSObject
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@property(nonatomic, readonly) BRLMRequestPrinterInfoErrorCode code;
@property(nonatomic, readonly) NSArray<BRLMLog*>* allLogs;
@property(nonatomic, readonly) NSString* errorDescription;

@end
