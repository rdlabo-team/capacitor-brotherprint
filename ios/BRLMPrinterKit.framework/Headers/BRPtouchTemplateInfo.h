//
//  BRPtouchTemplateInfo.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

// 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601.
typedef unsigned long long int BRFILETIMEDate;
extern const BRFILETIMEDate NULLOfBRFILETIMEDate;

@interface BRPtouchTemplateInfo : NSObject

@property (nonatomic, readonly) NSUInteger key;
@property (nonatomic, readonly) NSUInteger fileSize;
@property (nonatomic, readonly) NSUInteger checksum;
@property (nonatomic, readonly) NSDate *modifiedDate;
@property (nonatomic, readonly) BRFILETIMEDate rawModifiedDate;
@property (nonatomic, readonly) NSString *fileName;

@end
