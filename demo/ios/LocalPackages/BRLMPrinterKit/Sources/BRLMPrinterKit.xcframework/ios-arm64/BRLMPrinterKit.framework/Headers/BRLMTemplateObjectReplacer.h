//
//  BRLMTemplateObjectReplacer.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2023/04/24.
//  Copyright © 2023 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, BRLMTemplateObjectEncode) {
    BRLMTemplateObjectEncode_UTF_8 = 0,
    BRLMTemplateObjectEncode_SHIFT_JIS = 1,
    BRLMTemplateObjectEncode_GB_18030_2000 = 2,
};

NS_ASSUME_NONNULL_BEGIN

@interface BRLMTemplateObjectReplacer : NSObject
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

- (nonnull instancetype)initWithObjectName:(NSString * _Nonnull )name value:( NSString * _Nonnull )value encode:(BRLMTemplateObjectEncode)encode;
- (nonnull instancetype)initWithObjectIndex:(NSUInteger)index value:( NSString * _Nonnull )value encode:(BRLMTemplateObjectEncode)encode;
@end

NS_ASSUME_NONNULL_END
