//
//  BRLMPrinterDriver.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <BRLMPrinterKit/BRLMPrinterDefine.h>
#import <BRLMPrinterKit/BRLMPrintError.h>
#import <BRLMPrinterKit/BRLMPrintSettingsProtocol.h>
#import <BRLMPrinterKit/BRLMPrinterStatus.h>
#import <BRLMPrinterKit/BRLMGetStatusError.h>

#import <BRLMPrinterKit/BRLMTemplatePrintSettingsProtocol.h>
#import <BRLMPrinterKit/BRLMTemplateObjectReplacer.h>

#import <BRLMPrinterKit/BRLMRequestPrinterInfoResult.h>
#import <BRLMPrinterKit/BRLMPtouchDeviceDependedDataHeader.h>
#import <BRLMPrinterKit/BRLMBatteryInfo.h>
#import <BRLMPrinterKit/BRLMPtouchTemplateInfo.h>

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

- (BRLMPrintError *)printTemplateWithKey:(NSUInteger)key settings:(nonnull id<BRLMTemplatePrintSettingsProtocol>)settings replacers:(nullable NSArray<BRLMTemplateObjectReplacer *>*)replacers;

- (void)cancelPrinting;

- (BRLMPrintError *)sendRawData:(NSData *)data;

- (BRLMGetPrinterStatusResult *)getPrinterStatus;
- (BRLMRequestPrinterInfoResult<NSString *> *)requestSerialNumber;
- (BRLMRequestPrinterInfoResult<NSString *> *)requestSystemReport;
- (BRLMRequestPrinterInfoResult<NSString *> *)requestMainFirmVersion;
- (BRLMRequestPrinterInfoResult<NSString *> *)requestMediaVersion;
- (BRLMRequestPrinterInfoResult<BRLMBatteryInfo *> *)requestBatteryInfo;
- (BRLMRequestPrinterInfoResult<NSMutableArray<BRLMPtouchTemplateInfo*> *> *) requestTemplateInfoList;

- (void)closeChannel;

///This api is for Brother applications. We do not recommend you to use it.
- (BRLMRequestPrinterInfoResult<NSMutableArray<BRLMPtouchDeviceDependedDataHeader*> *> *) requestPtouchDeviceDependedDataHeaderList;
///This api is for Brother applications. We do not recommend you to use it.
- (BRLMRequestPrinterInfoResult<NSString *> *)requestBluetoothFirmVersion;
///This api is for Brother applications. We do not recommend you to use it.
- (BRLMRequestPrinterInfoResult<NSNumber *>*) requestPrinterInternalModelFlag;


@end


NS_ASSUME_NONNULL_END
