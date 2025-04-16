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
#import <BRLMPrinterKit/BRLMTransferResult.h>
#import <BRLMPrinterKit/BRLMRequestPrinterInfoResult.h>
#import <BRLMPrinterKit/BRLMPtouchDeviceDependedDataHeader.h>
#import <BRLMPrinterKit/BRLMBatteryInfo.h>
#import <BRLMPrinterKit/BRLMPtouchTemplateInfo.h>
#import <BRLMPrinterKit/BRLMRemoveTemplateResult.h>

#import <BRLMPrinterKit/BRLMPrinterConfigRequestOrder.h>
#import <BRLMPrinterKit/BRLMPrinterConfigRequestResult.h>
#import <BRLMPrinterKit/BRLMPrinterConfigUpdateOrder.h>
#import <BRLMPrinterKit/BRLMPrinterConfigUpdateResult.h>

NS_ASSUME_NONNULL_BEGIN

@interface BRLMGetPrinterStatusResult : NSObject
@property (nonatomic, readonly) BRLMGetStatusError *error;
@property (nonatomic, readonly, nullable) BRLMPrinterStatus *status;
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@end

typedef _Nullable CGImageRef (^BRLMPrinterDriverPrintImageClosure)(void) NS_REFINED_FOR_SWIFT;
@class BRLMPrinterDriverPrintImageClosuresWrapper;

@interface BRLMPrinterDriver : NSObject

- (instancetype)init __attribute__((unavailable("Use [BRLMPrinterDriverGenerator openChannel:] instead")));
- (instancetype)copy __attribute__((unavailable("Use [BRLMPrinterDriverGenerator openChannel:] instead")));

- (BRLMPrintError *)printImageWithURL:(NSURL *)url settings:(id<BRLMPrintSettingsProtocol>)settings;
- (BRLMPrintError *)printImageWithURLs:(NSArray<NSURL *> *)urls settings:(id<BRLMPrintSettingsProtocol>)settings;
- (BRLMPrintError *)printImageWithImage:(CGImageRef)imageRef settings:(id<BRLMPrintSettingsProtocol>)settings;
- (BRLMPrintError *)printImageWithClosures:(NSArray<BRLMPrinterDriverPrintImageClosure>*)closures settings:(id<BRLMPrintSettingsProtocol>)settings NS_REFINED_FOR_SWIFT;

- (BRLMPrintError *)printPDFWithURL:(NSURL *)url settings:(id<BRLMPrintSettingsProtocol>)settings;
- (BRLMPrintError *)printPDFWithURLs:(NSArray<NSURL *> *)urls settings:(id<BRLMPrintSettingsProtocol>)settings;
- (BRLMPrintError *)printPDFWithURL:(NSURL *)url pages:(NSArray<NSNumber *> *)pages settings:(id<BRLMPrintSettingsProtocol>)settings;

- (BRLMPrintError *)sendPRNFileWithURL:(NSURL *)url __attribute__ ((deprecated));
- (BRLMPrintError *)sendPRNFileWithURLs:(NSArray<NSURL *> *)urls __attribute__((deprecated));

- (BRLMPrintError *)printTemplateWithKey:(NSUInteger)key settings:(nonnull id<BRLMTemplatePrintSettingsProtocol>)settings replacers:(nullable NSArray<BRLMTemplateObjectReplacer *>*)replacers;

- (BRLMTransferResult<NSURL*> *)transferFirmwareFiles:(nonnull NSArray<NSURL *> *)urls progress:(nullable void (^)(NSURL* current, int progress))progress;
- (BRLMTransferResult<NSURL*> *)transferTemplateFiles:(nonnull NSArray<NSURL *> *)urls progress:(nullable void (^)(NSURL* current, int progress))progress;
- (BRLMTransferResult<NSURL*> *)transferDatabaseFiles:(nonnull NSArray<NSURL *> *)urls progress:(nullable void (^)(NSURL* current, int progress))progress;
- (BRLMTransferResult<NSURL*> *)transferBinaryFiles:(nonnull NSArray<NSURL *> *)urls progress:(nullable void (^)(NSURL* current, int progress))progress;
- (BRLMTransferResult<NSNumber*> *)transferBinaryData:(nonnull NSArray<NSData *> *)data progress:(nullable void (^)(NSNumber* index, int progress))progress;

- (void)cancelPrinting;

- (BRLMPrintError *)sendRawData:(NSData *)data __attribute__ ((deprecated));

- (BRLMGetPrinterStatusResult *)getPrinterStatus;
- (BRLMRequestPrinterInfoResult<NSString *> *)requestSerialNumber;
- (BRLMRequestPrinterInfoResult<NSString *> *)requestSystemReport;
- (BRLMRequestPrinterInfoResult<NSString *> *)requestMainFirmVersion;
- (BRLMRequestPrinterInfoResult<NSString *> *)requestMediaVersion;
- (BRLMRequestPrinterInfoResult<BRLMBatteryInfo *> *)requestBatteryInfo;
- (BRLMRequestPrinterInfoResult<NSMutableArray<BRLMPtouchTemplateInfo*> *> *) requestTemplateInfoList;

- (BRLMRemoveTemplateResult*)removeTemplateWithKeys:(NSArray<NSNumber *> *)keys;

- (void)closeChannel;

///This api is for Brother applications. We do not recommend you to use it.
- (BRLMPrinterConfigRequestResult*)requestPrinterConfig:(BRLMPrinterConfigRequestOrder*)order;
///This api is for Brother applications. We do not recommend you to use it.
- (BRLMPrinterConfigUpdateResult*)updatePrinterConfig:(BRLMPrinterConfigUpdateOrder*)order;
///This api is for Brother applications. We do not recommend you to use it.
- (BRLMRequestPrinterInfoResult<NSMutableArray<BRLMPtouchDeviceDependedDataHeader*> *> *) requestPtouchDeviceDependedDataHeaderList;
///This api is for Brother applications. We do not recommend you to use it.
- (BRLMRequestPrinterInfoResult<NSString *> *)requestBluetoothFirmVersion;
///This api is for Brother applications. We do not recommend you to use it.
- (BRLMRequestPrinterInfoResult<NSNumber *>*) requestPrinterInternalModelFlag;
///This api is for Brother applications. We do not recommend you to use it.
- (BRLMRequestPrinterInfoResult<NSNumber *> *) requestIsBootMode;
///This api is for swift compatibility. We do not recommend you to use it.
- (BRLMPrintError *)printImageWithClosuresSwift:(NSArray<BRLMPrinterDriverPrintImageClosuresWrapper*>*)closures settings:(id<BRLMPrintSettingsProtocol>)settings;
@end

///This class is for swift compatibility. We do not recommend you to use it.
@interface BRLMPrinterDriverPrintImageClosuresWrapper : NSObject
@property(nonatomic, readonly) BRLMPrinterDriverPrintImageClosure closure;
- (instancetype)initWithClosure:(BRLMPrinterDriverPrintImageClosure)closure;
@end

NS_ASSUME_NONNULL_END
