//
//  BRLMPtouchDeviceDependedDataHeader.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2023/06/12.
//  Copyright © 2023 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

///This enum  is for Brother applications. We do not recommend you to use it.
typedef NS_ENUM(NSInteger, BRLMPtouchDeviceDependedDataHeaderDataKind ) {
    BRLMPtouchDeviceDependedDataHeaderDataKindTemplate,
    BRLMPtouchDeviceDependedDataHeaderDataKindDatabase,
    BRLMPtouchDeviceDependedDataHeaderDataKindMedia,
    BRLMPtouchDeviceDependedDataHeaderDataKindFont,
    BRLMPtouchDeviceDependedDataHeaderDataKindMainFirm,
    BRLMPtouchDeviceDependedDataHeaderDataKindBootFirm,
    BRLMPtouchDeviceDependedDataHeaderDataKindBluetoothFirm,
    BRLMPtouchDeviceDependedDataHeaderDataKindAnyFirm,
    BRLMPtouchDeviceDependedDataHeaderDataKindUserData,
    BRLMPtouchDeviceDependedDataHeaderDataKindOther,
};
///This class  is for Brother applications. We do not recommend you to use it.
@interface BRLMPtouchDeviceDependedDataHeader : NSObject
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@property (nonatomic, readonly) BRLMPtouchDeviceDependedDataHeaderDataKind dataKind;
@property (nonatomic, readonly) NSString *version;
@property (nonatomic, readonly) NSString *name;
@property (nonatomic, readonly) NSDate *modifiedDate;
@property (nonatomic, readonly) NSUInteger dataSize;
@property (nonatomic, readonly) NSString *destinationCode;
@end
