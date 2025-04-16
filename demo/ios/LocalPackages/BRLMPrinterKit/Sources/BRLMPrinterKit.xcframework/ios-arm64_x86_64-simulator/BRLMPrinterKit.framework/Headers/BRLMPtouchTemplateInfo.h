//
//  BRLMPtouchTemplateInfo.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2023/07/10.
//  Copyright © 2023 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

// 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601.
typedef unsigned long long int BRLMFILETIMEDate;
extern const BRLMFILETIMEDate NULLOfBRFILETIMEDate;

@interface BRLMPtouchTemplateInfo : NSObject
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@property (nonatomic, readonly) NSUInteger key;
@property (nonatomic, readonly) NSUInteger fileSize;
@property (nonatomic, readonly) NSUInteger checksum;
@property (nonatomic, readonly) NSDate *modifiedDate;
@property (nonatomic, readonly) BRLMFILETIMEDate rawModifiedDate;
@property (nonatomic, readonly) NSString *fileName;
@end
