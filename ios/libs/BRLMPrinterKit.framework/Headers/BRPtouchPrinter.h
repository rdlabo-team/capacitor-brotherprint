//
//  BRPtouchPrinter.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2012-2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h>

#include "BRPtouchPrintInfo.h"
#include "BRPtouchPrinterData.h"
#include "BRPtouchPrinterStatus.h"
#include "BRPtouchLabelParam.h"
#include "BRPtouchLabelInfoStatus.h"
#import "BRPtouchBatteryInfo.h"
#import "BRCustomPaperInfoCommand.h"
#include "BRPtouchTemplateInfo.h"

#define ERROR_NONE_          0
#define ERROR_TIMEOUT		-3
#define ERROR_BADPAPERRES	-4
#define ERROR_IMAGELARGE	-6
#define ERROR_CREATESTREAM	-7
#define ERROR_OPENSTREAM	-8
#define ERROR_FILENOTEXIST  -9
#define ERROR_PAGERANGEERROR  -10
#define ERROR_NOT_SAME_MODEL_ -11
#define ERROR_BROTHER_PRINTER_NOT_FOUND_ -12
#define ERROR_PAPER_EMPTY_ -13
#define ERROR_BATTERY_EMPTY_ -14
#define ERROR_COMMUNICATION_ERROR_ -15
#define ERROR_OVERHEAT_ -16
#define ERROR_PAPER_JAM_ -17
#define ERROR_HIGH_VOLTAGE_ADAPTER_ -18
#define ERROR_CHANGE_CASSETTE_ -19
#define ERROR_FEED_OR_CASSETTE_EMPTY_ -20
#define ERROR_SYSTEM_ERROR_ -21
#define ERROR_NO_CASSETTE_ -22
#define ERROR_WRONG_CASSENDTE_DIRECT_ -23
#define ERROR_CREATE_SOCKET_FAILED_ -24
#define ERROR_CONNECT_SOCKET_FAILED_ -25
#define ERROR_GET_OUTPUT_STREAM_FAILED_ -26
#define ERROR_GET_INPUT_STREAM_FAILED_ -27
#define ERROR_CLOSE_SOCKET_FAILED_ -28
#define ERROR_OUT_OF_MEMORY_ -29
#define ERROR_SET_OVER_MARGIN_ -30
#define ERROR_NO_SD_CARD_ -31
#define ERROR_FILE_NOT_SUPPORTED_ -32
#define ERROR_EVALUATION_TIMEUP_ -33
#define ERROR_WRONG_CUSTOM_INFO_ -34
#define ERROR_NO_ADDRESS_ -35
#define ERROR_NOT_MATCH_ADDRESS_ -36
#define ERROR_FILE_NOT_FOUND_ -37
#define ERROR_TEMPLATE_FILE_NOT_MATCH_MODEL_ -38
#define ERROR_TEMPLATE_NOT_TRANS_MODEL_ -39
#define ERROR_COVER_OPEN_ -40
#define ERROR_WRONG_LABEL_ -41
#define ERROR_PORT_NOT_SUPPORTED_ -42
#define ERROR_WRONG_TEMPLATE_KEY_ -43
#define ERROR_BUSY_ -44
#define ERROR_TEMPLATE_NOT_PRINT_MODEL_ -45
#define ERROR_CANCEL_ -46
#define ERROR_PRINTER_SETTING_NOT_SUPPORTED_ -47
#define ERROR_INVALID_PARAMETER_ -48
#define ERROR_INTERNAL_ERROR_ -49
#define ERROR_TEMPLATE_NOT_CONTROL_MODEL_ -50
#define ERROR_TEMPLATE_NOT_EXIST_ -51
#define ERROR_BADENCRYPT_ -52 // This does not occur in iOS
#define ERROR_BUFFER_FULL_ -53
#define ERROR_TUBE_EMPTY_ -54
#define ERROR_TUBE_RIBON_EMPTY_ -55
#define ERROR_UPDATE_FRIM_NOT_SUPPORTED_ -56 // This does not occur in iOS
#define ERROR_OS_VERSION_NOT_SUPPORTED_ -57 // This does not occur in iOS
#define ERROR_MINIMUM_LENGTH_LIMIT_ -58
#define ERROR_FAIL_TO_CONVERT_CSV_TO_BLF_ -59


//  Message value
#define MESSAGE_START_COMMUNICATION_ 1
#define MESSAGE_START_CONNECT_ 2
#define MESSAGE_END_CONNECTED_ 3
//#define MESSAGE_START_GET_OUTPUT_STREAM 4 // Not Available
//#define MESSAGE_END_GET_OUTPUT_STREAM 5 // Not Available
//#define MESSAGE_START_GET_INPUT_STREAM 6 // Not Available
//#define MESSAGE_END_GET_INPUT_STREAM 7 // Not Available
#define MESSAGE_START_SEND_STATUS_REQUEST_ 8
#define MESSAGE_END_SEND_STATUS_REQUEST_ 9
#define MESSAGE_START_READ_PRINTER_STATUS_ 10
#define MESSAGE_END_READ_PRINTER_STATUS_ 11
#define MESSAGE_START_CREATE_DATA_ 12
#define MESSAGE_END_CREATE_DATA_ 13
#define MESSAGE_START_SEND_DATA_ 14
#define MESSAGE_END_SEND_DATA_ 15
#define MESSAGE_START_SEND_TEMPLATE_ 16
#define MESSAGE_END_SEND_TEMPLATE_ 17
#define MESSAGE_START_SOCKET_CLOSE_ 18
#define MESSAGE_END_SOCKET_CLOSE_ 19
//#define MESSAGE_END_COMMUNICATION 20 // Not Available
#define MESSAGE_PRINT_COMPLETE_ 21
#define MESSAGE_PRINT_ERROR_ 22
#define MESSAGE_PAPER_EMPTY_ 23
#define MESSAGE_START_COOLING_ 24
#define MESSAGE_END_COOLING_ 25
//#define MESSAGE_PREPARATION 26 // Not Available
#define MESSAGE_WAIT_PEEL_ 27
#define MESSAGE_START_UPDATE_BLUETOOTH_SETTING_ 28
#define MESSAGE_END_UPDATE_BLUETOOTH_SETTING_ 29
#define MESSAGE_START_GET_BLUETOOTH_SETTING_ 30
#define MESSAGE_END_GET_BLUETOOTH_SETTING_ 31
#define MESSAGE_START_GET_TEMPLATE_LIST_ 32
#define MESSAGE_END_GET_TEMPLATE_LIST_ 33
#define MESSAGE_START_REMOVE_TEMPLATE_LIST_ 34
#define MESSAGE_END_REMOVE_TEMPLATE_LIST_ 35
#define MESSAGE_CANCEL_ 36
//#define MESSAGE_START_TRANSFER_FIRM_ 39, //Not Available
//#define MESSAGE_END_TRANSFER_FIRM_ 38, //Not Available
#define MESSAGE_CHECK_PRINTER_STATUS_WHILE_CHANGE_STATUS_MODE_ON_ 39

typedef NS_ENUM(NSUInteger, PrinterSettingItem) {
    PSI_NET_BOOTMODE = 1,
    PSI_NET_INTERFACE = 2,
    PSI_NET_USED_IPV6 = 3,
    PSI_NET_PRIORITY_IPV6 = 4,
    PSI_NET_IPV4_BOOTMETHOD = 5,
    PSI_NET_STATIC_IPV4ADDRESS = 6,
    PSI_NET_SUBNETMASK = 7,
    PSI_NET_GATEWAY = 8,
    PSI_NET_DNS_IPV4_BOOTMETHOD = 9,
    PSI_NET_PRIMARY_DNS_IPV4ADDRESS = 10,
    PSI_NET_SECOND_DNS_IPV4ADDRESS = 11,
    PSI_NET_IPV6_BOOTMETHOD = 12,
    PSI_NET_STATIC_IPV6ADDRESS = 13,
    PSI_NET_PRIMARY_DNS_IPV6ADDRESS = 14,
    PSI_NET_SECOND_DNS_IPV6ADDRESS = 15,
    PSI_NET_IPV6ADDRESS_LIST = 16,
    PSI_NET_COMMUNICATION_MODE = 17,
    PSI_NET_SSID = 18,
    PSI_NET_CHANNEL = 19,
    PSI_NET_AUTHENTICATION_METHOD = 20,
    PSI_NET_ENCRYPTIONMODE = 21,
    PSI_NET_WEPKEY = 22,
    PSI_NET_PASSPHRASE = 23,
    PSI_NET_USER_ID = 24,
    PSI_NET_PASSWORD = 25,
    PSI_NET_NODENAME = 26,
    PSI_WIRELESSDIRECT_KEY_CREATE_MODE = 27,
    PSI_WIRELESSDIRECT_SSID = 28,
    PSI_WIRELESSDIRECT_NETWORK_KEY = 29,
    PSI_BT_ISDISCOVERABLE = 30,
    PSI_BT_DEVICENAME = 31,
    PSI_BT_BOOTMODE = 34,
    PSI_PRINTER_POWEROFFTIME = 35,
    PSI_PRINTER_POWEROFFTIME_BATTERY = 36,
    PSI_PRINT_JPEG_HALFTONE = 37,
    PSI_PRINT_JPEG_SCALE = 38,
    PSI_PRINT_DENSITY = 39,
    PSI_PRINT_SPEED = 40,
    
    PSI_BT_POWERSAVEMODE = 41,
    PSI_BT_SSP = 42,
    PSI_BT_AUTHMODE = 43,
    PSI_BT_AUTO_CONNECTION = 44,
} ;

//  Return value
#define RET_FALSE       0
#define RET_TRUE        1

//

typedef NS_ENUM(NSUInteger, CONNECTION_TYPE) {
    CONNECTION_TYPE_WLAN,
    CONNECTION_TYPE_BLUETOOTH, // Classic Bluetooth
    CONNECTION_TYPE_BLE, // Bluetooth Low Energy
    CONNECTION_TYPE_ERROR
};

extern NSString *BRWLanConnectBytesWrittenNotification;
extern NSString *BRBluetoothSessionBytesWrittenNotification;
extern NSString *BRBLEBytesWrittenNotification;
extern NSString *BRPtouchPrinterKitMessageNotification;

extern NSString *const BRBytesWrittenKey;
extern NSString *const BRBytesToWriteKey;
extern NSString *const BRMessageKey;


@interface BRPtouchPrinter : NSObject <NSNetServiceBrowserDelegate,NSNetServiceDelegate>

- (id)initWithPrinterIPAddress:(NSString *)IPAddress;
- (id)initWithPrinterName:(NSString*)strPrinterName;
- (id)initWithPrinterName:(NSString*)strPrinterName interface:(CONNECTION_TYPE)type;
- (NSString *)printerName;
- (BOOL)setPrinterName:(NSString*)strPrinterName;
- (void)setPrintInfo:(BRPtouchPrintInfo*)printInfo;
- (BOOL)setCustomPaperFile:(NSString*)strFilePath;
- (NSArray *)setCustomPaperInfoCommand:(BRCustomPaperInfoCommand *)customPaperInfoCommand;
//- (BOOL)setEncryptKey:(NSString*)strKey keyEx:(NSString*)strKeyEx; // Not Available

- (BOOL)isPrinterReady;

- (NSArray *)getSupportPaperArray;
- (BRPtouchLabelParam *)getCurrentLabelParam;

- (BRPtouchLabelInfoStatus *)getLabelInfoStatus;
- (int)getPTStatus:(PTSTATUSINFO*)status;
- (int)getStatus:(BRPtouchPrinterStatus**)status;
- (int)getStatus:(BRPtouchPrinterStatus**)status errorCode:(int *)errorCode;
- (NSString *)getModelName;
- (NSString *)getFirmVersion;
- (NSString *)getMediaVersion;
- (NSString *)getMediaFileVersion:(NSString *)filepath;
- (NSString *)getDeviceSerialNumber;
- (int)getSystemReport:(NSString* *)report;

- (BOOL)sendTemplateFile:(NSArray*)sendFileArray;
- (BOOL)sendFirmwareFile:(NSArray*)sendFileArray;

- (int)sendTemplate:(NSString *)sendtemplateFilePath connectionType:(CONNECTION_TYPE) type;
- (int)sendDatabase:(NSString *)databaseFilePath;

- (BOOL)startPTTPrint:(int)key encoding:(NSStringEncoding)encoding;
- (BOOL)replaceText:(NSString *)data;
- (BOOL)replaceTextIndex:(NSString *)data objectIndex:(int)objIndex;
- (BOOL)replaceTextName:(NSString *)data objectName:(NSString *)objName;
- (int)flushPTTPrintWithCopies:(int)nCopy;
- (int)removeTemplate:(NSArray<NSNumber*> *)keyList;
- (int)getTemplateList:(NSArray<BRPtouchTemplateInfo*>*__autoreleasing *)templateList;

- (void)setIPAddress:(NSString*)strIP;
- (void)setupForBluetoothDeviceWithSerialNumber:(NSString*)serialNumber;
- (void)setBLEAdvertiseLocalName:(NSString*)advertiseLocalName;

/**
 * Deprecated.
 * Use startCommunication.
 **/
- (int)startPrint __attribute__((deprecated));
- (BOOL)startCommunication;

/**
 * Deprecated.
 * Use endCommunication.
 **/
- (void)endPrint __attribute__((deprecated));
- (void)endCommunication;

- (int)sendFile:(NSString*)filePath;
- (int)sendData:(NSData*)data;
- (int)sendFileEx:(NSString*)filePath;
- (int)sendDataEx:(NSData*)data;

- (int)printPDFAtPath:(NSString *)pdfPath pages:(NSUInteger [])indexes length:(NSUInteger)length copy:(int)nCopy;
- (int)printImage:(CGImageRef)imageRef copy:(int)nCopy;
- (int)printFiles:(NSArray *)filePaths copy:(int)nCopy;

- (int)cancelPrinting;
- (void)setInterface:(CONNECTION_TYPE)strInterface;

- (int)getBatteryStatus;
- (int)getBatteryInfo:(BRPtouchBatteryInfo* *)batteryInfo;
- (int)getPrinterBootMode;
- (int) setAutoConnectBluetooth:(BOOL)flag;
- (int) isAutoConnectBluetooth;

- (int)setPrinterSettings:(NSDictionary*)printerSettings;
- (int)getPrinterSettings:(NSDictionary**)printerSettings require:(NSArray*)require;

@end
