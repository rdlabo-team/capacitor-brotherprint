//
//  BRLMValidatePrintSettings.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol BRLMPrintSettingsProtocol;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, BRLMValidatePrintSettingsIssueRank) {
    BRLMValidatePrintSettingsIssueRankError,
    BRLMValidatePrintSettingsIssueRankWarning,
    BRLMValidatePrintSettingsIssueRankNotice,
};
@interface BRLMValidatePrintSettingsIssue : NSObject
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@property(nonatomic, readonly) BRLMValidatePrintSettingsIssueRank rank;
@property(nonatomic, readonly) NSString* message;
@property(nonatomic, readonly) NSInteger codeID;
- (NSString *)description;
@end

@interface BRLMValidatePrintSettingsReport : NSObject
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@property(nonatomic, readonly) NSArray<BRLMValidatePrintSettingsIssue*>* issues;
@property(nonatomic, readonly) NSUInteger errorCount;
@property(nonatomic, readonly) NSUInteger warningCount;
@property(nonatomic, readonly) NSUInteger noticeCount;
- (NSString *)description;
@end

@interface BRLMValidatePrintSettings : NSObject
+ (BRLMValidatePrintSettingsReport*) validate:(id<BRLMPrintSettingsProtocol>)printSettings;
@end

NS_ASSUME_NONNULL_END
