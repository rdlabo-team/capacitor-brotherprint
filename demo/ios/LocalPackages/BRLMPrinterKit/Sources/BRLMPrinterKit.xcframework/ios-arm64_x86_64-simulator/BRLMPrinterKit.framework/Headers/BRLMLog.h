//
//  BRLMLog.h
//  BRLMPrinterKit
//
//  Created by matsuo yu on 2022/07/19.
//  Copyright © 2022 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, BRLMLogLevel) {
    BRLMLogLevelNotice = 1,
    BRLMLogLevelWarning = 2,
    BRLMLogLevelError = 3,
};
typedef int BRLMVariantErrorCode;
@interface BRLMLog: NSObject
@property (nonatomic, readonly, nonnull) NSString* timeStamp;
@property (nonatomic, readonly) BRLMLogLevel level;
@property (nonatomic, readonly, nonnull) NSString* errorDescription;
@property (nonatomic, readonly) BRLMVariantErrorCode errorCode;
@property (nonatomic, readonly, nonnull) NSString* whereCause;
@property (nonatomic, readonly, nonnull) NSString* identificationCode;

- (nonnull instancetype)init __unavailable;
- (nonnull instancetype)copy __unavailable;
@end
