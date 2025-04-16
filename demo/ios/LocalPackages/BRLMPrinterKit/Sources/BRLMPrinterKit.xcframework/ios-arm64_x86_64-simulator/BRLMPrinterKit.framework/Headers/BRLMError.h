//
//  BRLMError.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface BRLMError : NSObject

- (NSString *)description;
@property(nonatomic, readonly, copy, nullable) NSString *errorRecoverySuggestion;

@property(nonatomic, readonly, copy, nullable) NSDictionary *errorUserInfo;


- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

@end

NS_ASSUME_NONNULL_END
