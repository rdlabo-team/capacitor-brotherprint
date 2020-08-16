//
//  BRLMPrinterDriver.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BRLMPrinterDefine.h"
#import "BRLMPrintError.h"
#import "BRLMPrintSettingsProtocol.h"
#import "BRLMPrinterStatus.h"
#import "BRLMGetStatusError.h"


NS_ASSUME_NONNULL_BEGIN

@interface BRLMGetPrinterStatusResult : NSObject
@property (nonatomic, readonly) BRLMGetStatusError *error;
@property (nonatomic, readonly, nullable) BRLMPrinterStatus *status;
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@end

@interface BRLMPrinterDriver : NSObject

- (instancetype)init __attribute__((unavailable("Use [BRLMPrinterDriverGenerator openChannel:] instead")));
- (instancetype)copy __attribute__((unavailable("Use [BRLMPrinterDriverGenerator openChannel:] instead")));

- (BRLMPrintError *)printImageWithURL:(NSURL *)url settings:(id<BRLMPrintSettingsProtocol>)settings;
- (BRLMPrintError *)printImageWithURLs:(NSArray<NSURL *> *)urls settings:(id<BRLMPrintSettingsProtocol>)settings;
- (BRLMPrintError *)printImageWithImage:(CGImageRef)imageRef settings:(id<BRLMPrintSettingsProtocol>)settings;

- (BRLMPrintError *)printPDFWithURL:(NSURL *)url settings:(id<BRLMPrintSettingsProtocol>)settings;
- (BRLMPrintError *)printPDFWithURLs:(NSArray<NSURL *> *)urls settings:(id<BRLMPrintSettingsProtocol>)settings;
- (BRLMPrintError *)printPDFWithURL:(NSURL *)url pages:(NSArray<NSNumber *> *)pages settings:(id<BRLMPrintSettingsProtocol>)settings;

- (BRLMPrintError *)sendPRNFileWithURL:(NSURL *)url;
- (BRLMPrintError *)sendPRNFileWithURLs:(NSArray<NSURL *> *)urls;

- (void)cancelPrinting;

- (BRLMPrintError *)sendRawData:(NSData *)data;

- (BRLMGetPrinterStatusResult *)getPrinterStatus;


- (void)closeChannel;

@end


NS_ASSUME_NONNULL_END
