//
//  BMSPrinterDriver.h
//  BMSPrinterKit
//
//  Created by BMS on 10/2/12.
//  Copyright (c) 2012 Brother Mobile Solutions, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h> // for UIViewController definition

#ifndef REMOVE_BRPTOUCH_SDK
// NOTE: This is now preprocessor definition in the RemoveBRPtouchSDK version of the project.

// NOTE: We can NOT use the following "framework" syntax here (i.e. <BRPtouchPrinterKit/BRPtouchPrinterKit.h>)
// However, we CAN use the "BRPtouchPrinterKit.h" syntax, since we have added the BIL SDK .h
// files to our project.
//#import <BRPtouchPrinterKit/BRPtouchPrinterKit.h>
#import <BRLMPrinterKit/BRPtouchPrinterKit.h>

#endif

#import <BRLMPrinterKit/PrintSettings.h>

//*** NOTIFICATIONS
// Register for these if you wish to receive notifications for the following messages.
// Typically, you will only want to register for these if you are creating your own Custom Progress View.

// *************************************************************************************
// 1. Use this notification instead of sendData return value if you are making a Progress ViewController.
// With PJ7 printers, especially PJ-763MFi, "sendDataToChannel" may not return immediately on the final band,
// because it needs to check printer status to wait for the page to be printed before more data can be sent.
// This notification will be sent as soon as the data has been sent, allowing you to update your Progress GUI quicker.
// NOTE: KEY_BYTESWRITTEN is used when processing the SENDDATA notification.
// NOTE: If you are creating a ProgressView, this notification will work with ALL print APIs, even "simple" ones.
// So, it's recommended to use this instead of the return value of "sendDataToChannel". Refer to CustomProgress in SDK Sample.
#define NOTIFICATION_SENDDATA @"SendDataNotification"
#define NOTIFICATION_SENDDATA_KEY_BYTESWRITTEN  @"Bytes Written"
// *************************************************************************************

// *************************************************************************************
// 2. MFi Bluetooth models MUST process STATUS RESPONSES for each page BEFORE sending data for the next page.
//  This is being done to fix problems related to data handshake that cause Apple to disconnect the MFi device.
//  The status response will not come until the page has been printed, which may take some time.
//
//   This notification lets you know when this event occurs, in case you are creating a custom progress and want
// to inform the user. Even though you probably already know this will happen, the sendDataToChannel API will not return
// until the status response has been received. So, this notification allows you to update your Progress GUI
// at the moment this event occurs.
//
//  * NOTE: SDK 2.1 decided NOT to send this notification for RJ-4030Ai (and other new RJ models). So, it is only being sent for PJ-763MFi.
//
//    The main reason why is that RJ4 pages typically send AND print much faster than PJ pages because they are shorter,
//    particularly if you are printing labels. So, if you are printing a large batch of labels, then the progress GUI
//    (at least the one built-in to the SDK) looks strange when processing this message, because it spends most of its
//    time showing this message, instead of some potentially more valuable messages for the user.

//    Also, RJ models will always be using Roll Paper (continuous or labels), so it is not necessary (typically) to remind user to insert paper
//    for each page, like it is for PJ users who use Cutsheet paper.
//
//    So, a better approach for RJ4 may be to add other notifications instead of this one, as we read them.
//    This could allow sending notifications if various errors occur DURING printing, such as "Cover Open",
//    "No Paper", "Printer Overheat", "Low Battery", etc.
//
//    TODO: Add these new notifications. These could be applicable with PJ763Mfi too, but only if we enable the
//          unsolicited status responses.
//
#define NOTIFICATION_WAITING_FOR_STATUSRESPONSE @"WaitStatusResponseNotification"
// *************************************************************************************

// *************************************************************************************
// 3. Use NOTIFICATION_PRINTER_STATUS to receive notifications about printer status issues while printing,
// such as "cooling on/off", "coverOpen", "paperOut", etc.
// Refer to CustomProgressView in SDK Sample App for example of how to process this notification.
// The "userInfo" will contain Dictionary with 1 key/value pair.
// * Use KEY_STATUSNUMBER (NSNumber object) to process the notification.
//
// *** IMPORTANT NOTE: For now, this notification is ONLY supported for the BLUETOOTH channel, since it's the only channel
// that can read printer status for the "unsolicited" responses. In the future, this might be applicable for WIFI.
#define NOTIFICATION_PRINTER_STATUS @"PrinterStatusNotification"
#define NOTIFICATION_PRINTER_STATUS_KEY_STATUSNUMBER @"Status Number"

//*** Possible values returned for KEY_STATUSNUMBER
// These values are sent to Apps that register for NOTIFICATION_PRINTER_STATUS notifications.
// These values are defined arbitrarily and have no correspondence to the values returned in PTSTATUSINFO.
// NOTE: Not all status responses will generate a notification. And, not all are supported for each printer and channel.
// And, you are not required to handle all or any of them.

#define NOTIFICATION_PRINTER_STATUSNUMBER_COOLING_BEGIN          2001
#define NOTIFICATION_PRINTER_STATUSNUMBER_COOLING_END            2002
#define NOTIFICATION_PRINTER_STATUSNUMBER_MEDIAEMPTY_BEGIN       2003
#define NOTIFICATION_PRINTER_STATUSNUMBER_COVEROPEN_BEGIN        2004
#define NOTIFICATION_PRINTER_STATUSNUMBER_WAITINGFORPEELER_BEGIN 2005 // only sent for printers with peeler available and enabled.

// NOTE: MEDIAEMPTY, COVEROPEN, and WAITINGFORPEELER do NOT have a corresponding "_END" notification.
// The main reason is that the printers don't generate an "unsolicited" status response notification for these "end" conditions.
// These conditions can be detected internally, but it's not useful for an app because it requires sending another page(s) of data.
//
// --------------------------------
// With MEDIAEMPTY and COVEROPEN:
// --------------------------------
//    In a multi-page job, the RJ/TD printers continue to receive data and send status responses indicating that printing is successful.
// Only on the LAST page of a job does the printer WAIT to send "printing completed" status.
// So, as long as the job doesn't overflow the internal buffer, the printer can receive all the data for the job before
// you insert paper or close the cover. Then, if cover is closed or media inserted, printing should complete NORMALLY!!
// This is why they are NOT being treated as "fatal errors". And, for this reason, you may not even need to handle these notifications.
//
// The concerns are:
//  a) a statusResponseTimeout error could occur before media is changed after the final page of data has been sent.
//     So, the print API return code will indicate error.
//     But, everything should print OK anyway after you close the cover or insert more paper.
//  b) If too many pages are sent after the error occurs, the printer's internal buffer may fill up which could cause a "fatal" error.
//
// If you wish o prevent these concerns, you will have to use the "Manual APIs".
//
// Here is an algorithm that may be useful:
// 1. When you receive the notification, set a flag indicating mediaEmpty or coverOpen.
// 2. If flag is set, STOP sending data at the BEGINNING of a page
// 3."Poll" the printer status manually using "getPTStatus", and check the appropriate bits in the status response
// to determine when the condition goes away.
// 4. Allow user to cancel, in case there's a problem with the "polling".
//
// Refer to the Custom Progress View in SDK Sample App for an example.
//
// --------------------------------
// With WAITINGFORPEELER:
// --------------------------------
// There is no reason to check for the "end" notification anyway, because "sendDataToChannel" will not return
// until after the label is peeled (or a timeout occurs). But, you can use the "begin" notification to post
// a notice to users as a reminder they need to peel.
//
// *************************************************************************************


//*** Used to assure that the CGBitmap is 32-pixel aligned
// NOTE: Need to cast the division result to (int) in case a float type
// is passed in for x so we get the expected truncation.
#define ALIGN32(x) ((int)((x+31)/32) * 32)

// *************************************************************************************
//*** API Function Possible Return Codes ***

#ifndef REMOVE_BRPTOUCH_SDK
// NOTE: This is now a preprocessor definition in the RemoveBRPtouchSDK version of the project.

// NOTE: Some API functions will return values defined in BRPtouchPrinter.h,
// such as RET_TRUE, RET_FALSE, ERROR_TIMEOUT, etc.
// For clarity, the return codes defined in this module will begin at -1000.

#else
// When BRPtouch SDK is removed, we need to define these constants.

#define RET_FALSE       0
#define RET_TRUE        1
#define ERROR_TIMEOUT		-3

#endif

#define RET_PARAMETER_ERROR         -1000
#define RET_FILEPATHURL_ERROR       -1001
#define RET_PDFPAGE_ERROR           -1002
#define RET_PRINTSETTINGS_ERROR     -1003
#define RET_PARENTVIEW_ERROR        -1004
#define RET_DATABUFFER_ERROR        -1005
#define RET_PRINTERMODEL_ERROR      -1006

// Channel related errors
// These are generic for EACH channel (WIFI, BT, FILE)
#define RET_CHANNEL_NOTOPEN_ERROR   -1007 // if use Manual APIs on channel that has not been opened successfully
#define RET_CHANNEL_CREATE_ERROR     -1008 // if error creating WIFI/FILE streams or BluetoothSessionController. Memory related.
#define RET_CHANNEL_WRITE_ERROR      -1009 // error occurred while writing data, such as if channel disconnected
#define RET_CHANNEL_READ_ERROR       -1010 // error occurred while reading data, such as if channel disconnected

// In previous SDKs, RET_CHANNEL_CREATEERROR was used instead, but now we differentiate between these cases.
// NOTE: With WIFI channel especially, this can occur when user tries to start a job too quickly after a failure.
//       It may be reported only when trying to open the channel.
// NOTE: With BT channel, it can be reported in open, send, or read, if error event occurs on stream.
//       But I don't know how to cause it. Never seen it happen.
#define RET_CHANNEL_STREAMSTATUS_ERROR     -1011

// These are used only for BT channel
#define RET_BLUETOOTHCHANNEL_NOCONNECTEDPRINTERS_ERROR -1012 // No BT printers are connected
#define RET_BLUETOOTHCHANNEL_CHANNELDISCONNECTED_ERROR -1013 // BT Session disconnected while sending data
#define RET_BLUETOOTHCHANNEL_OPENSESSION_ERROR         -1014 // unable to open EASession
// Added this in SDK 2.1.0 to allow distinguishing this from ERROR_TIMEOUT. Only applicable for MFi models.
// This error may occur if all data was sent successfully for a page, but the SDK timed out waiting for
// a status response from the printer, which may occur if paper is not inserted on time to print the whole page
// before the "statusReponseTimeout" has been reached.
#define RET_BLUETOOTHCHANNEL_STATUSTIMEOUT_ERROR       -1015

// These are also only used with BT channel, since it's currently the only Bi-Di channel.
// However, these are generally related to errors received when processing printer status.
// So, in case WIFI is ever able to be Bi-Di, let's use a more generic name for these #define constants
#define RET_PRINTERSTATUS_ERROR_PRINTERTURNEDOFF       -1016
#define RET_PRINTERSTATUS_ERROR_BATTERYWEAK            -1017
#define RET_PRINTERSTATUS_ERROR_EXPANSIONBUFFERFULL    -1018
#define RET_PRINTERSTATUS_ERROR_COMMUNICATIONERROR     -1019
#define RET_PRINTERSTATUS_ERROR_OVERHEATINGERROR       -1020
#define RET_PRINTERSTATUS_ERROR_MEDIACANNOTBEFED       -1021 // This is different from media empty. Treated as an error.
#define RET_PRINTERSTATUS_ERROR_UNKNOWNERROR           -1022 // If status response processor gets unhandled error

//** 6/8/23: Added NEW error codes. Not changing old constants above, to be safe.
// This occurs if "app switch" in middle of BT transfer (if no app support for background processing).
// NOTE: It does not seem to happen on WIFI.
#define RET_CHANNEL_STREAM_END_OCCURRED                -1023

// *************************************************************************************


// *************************************************************************************
// SENDDATAMODE - new parameter added to "sendDataToChannel" and "sendNSDataToChannel".
//
// With Bluetooth Printers, it is REQUIRED to know EXACTLY when the FORM FEED command occurs in the data stream.
//
// NOTE: By "Form Feed", this means "the last byte(s) of data sent for a PAGE of data", which cause it to
// print the page.
//   * for PJ models, this typically means "Esc ~ \0C" or "Esc ^ \0C"
//   * for RJ/TD models, this typically means "\1A" or "\0C".
//
// The reason is that BT printers typically send several "status" responses while printing a page.
// In some cases, the printer will NOT ACCEPT DATA until the page completes printing.
// And, with Bluetooth MFi, Apple will SHUT DOWN THE CONNECTION if it cannot send data to the printer within
// about 5 seconds. Yet, Apple seems to take the data in some cases before it reports FALSE for "hasSpaceAvailable".
// As such, there seems to be a TIMING PROBLEM with either the Printer Firmware or Apple's EASession, or both!!
// So, we need to AVOID sending data to Apple when the printer will stop accepting it.
//
// So the "sendDataToChannel"  and "sendNSDataToChannel" APIs need to know when the printer will begin printing a
// page, so we can stop sending data until all the status responses have been processed.
//
// NOTE: WIFI models do NOT send status responses over Port9100 connections.
// So, it is not possible to handle them in the same way as required with Bluetooth models.
// In this SDK, status is being IGNORED with WIFI connections. But, in the future, it may be necessary to handle it
// a different way. So, even WIFI models MAY benefit from this new option eventually.
//
// Options:
// 1. kSendDataMode_NoFormFeed:
//   * This buffer of data does NOT contain a form feed.
//   * So, all bytes will be sent and the method will return immediately (as soon as possible)
//
// 2. kSendDataMode_FormFeedAtEnd:
//   * This buffer DOES contain a form feed, and this is at the END of the buffer.
//   * NOTE: This can normally be determined easily, since this will be the last buffer of data for the page,
//     where the data was generated by one of the "generatePrintData" APIs below.
//   * So, the sendDataXX methods will process the printer status before returning.
//   * WARNING: Do not use this mode if you generate/send your own ZPL/CPCL/ESCP data!! Some printers (RJ4030Ai at least)
//     do not send status responses except for Raster Mode data.
//
// 3. kSendDataMode_FormFeedIndeterminate:
//   * This buffer MAY OR MAY NOT contain a form feed, and it could be anywhere in the buffer.
//   * So, the code may "search" the data buffer to see if it contains the Form Feed, if applicable.
//   * NOTE: Since PJ models contain a 3-byte FormFeed command, it is possible to find it. However, since
//     other models only have a 1-byte command, it is not practical to try to find the byte in the data,
//     because it could be a valid byte of actual PRINT data, NOT a command byte. It is impossible to
//     distinguish without writing a full parser.
//     So, only PJ763 will treat this differently than the "NoFormFeed" option.
//   * NOTE: However, even with PJ, it is possible for the print data to RANDOMLY contain the 3-bytes as PRINT DATA.
//     If this occurs, it WILL cause a PROBLEM.
//   * NOTE: This case is mostly applicable when sending multiple page PRN data, such as with one of the
//     "sendFile" APIs.
//
//   * We recommend NOT using this option, if it is at all possible to avoid it.
//
// Generally, the SDK will handle this internally for the "Simple APIs", e.g. "printPDFFilePathURL".
// So, you the ISV will only need to care about this when you are using the "Manual APIs".
//
// We recommend using options 1 and 2 only if you are using Manual APIs to send PDF or IMAGE data.
// However, for PRN data, particularly in a FILE, it may be necessary to use option 3 ("Indeterminate").
//
// WARNING: If your app generates its own PRN data (e.g. ESC/P, ZPL, CPCL, etc) which you use with "sendDataToChannel",
// you might think you can use option 1,2 instead of option 3. However, **DO NOT USE** Option 2 ("FormFeedAtEnd")
// with RJ4030Ai, unless data is "Raster Mode". Unfortunately, the printer does NOT send "unsolicited status" responses
// for ESC/P, ZPL, CPCL, etc. So, the SDK will lock up waiting for status responses that will never come, and this will
// result in a STATUSTIMEOUT_ERROR.
//
// Refer to SDK Sample App code for more details of how to use this.
//
typedef enum
{
    kSendDataMode_NoFormFeed=1,
    kSendDataMode_FormFeedAtEnd,
    kSendDataMode_FormFeedIndeterminate,
} SENDDATAMODE;
// *************************************************************************************


@interface BMSPrinterDriver : NSObject
{

}

//******** Designated Initializers ********
- (id) initWithPrintSettings:(PrintSettings *)printSettings; // use this for PDF or IMAGE
- (id) initWithModel:(PRINTERMODEL)theModel; // use this for PRN file/data

//******** Class Properties ********
@property (nonatomic, assign) PRINTERMODEL printerModel;
@property (nonatomic, retain) PrintSettings *printSettings;

// Caller MUST set these as appropriate when printing multiple pages (when using the "Manual APIs").
@property (nonatomic, assign) BOOL isFirstPage;
@property (nonatomic, assign) BOOL isLastPage;

//*** Optional Properties:

// *************************************************************************************
// antiAliasMode:
//
// This property is now provided to override the (new) default handling of "anti-aliasing".
// When Anti-Alias is ENABLED, font characters will be "smoothed" by Apple by adding a "light-gray outline".
// This is Apple's default behavior, and it was our SDK's default behavior in v1.1.1 and earlier.
//
// DEFAULT driver behavior (new in SDK 2.0):
// * If Halftone is kHalftoneDiffusion, anti-alias is DISABLED, because halftoning may produce "stray dots" on
//   characters that can decrease clarity.
// * If Halftone is kHalftoneThreshold, anti-alias is ENABLED, because Threshold Value can be used to "thicken"
//   the font characters by increasing the Threshold Value, so that the "light-gray outline" is printed BLACK
//   instead of WHITE as it normally does at the default threshold value of 128.
//
// In some cases, you might want to force the anti-alias on/off. This property is provided for that purpose.
//
// For example, when printing barcodes, generally you will want to use Threshold halftone because Diffusion
// could make them unreadable. However, with AntiAlias ENABLED, it could result in thicker bars which MAY
// confuse a barcode reader. If that happens, then you should force AntiAliasing to be disabled.
//
// Another example: some pixels in the anti-alias "gray outline" might be darker gray than the Threshold Value,
// which would result in "stray dots". So, you could DISABLE anti-alias in this case to remove these
// stray dots, but then modifying Threshold Value would have NO effect in "thickening" the text characters.
//
// In most cases, it is a very subtle change, and most users will not notice the difference either way.
// But, we are providing this property as an option to an ISV just in case it will be required.
typedef enum
{
    kAntiAliasMode_UseDriverDefault=1,
    kAntiAliasMode_ForceEnabled,
    kAntiAliasMode_ForceDisabled,
} ANTIALIASMODE;
@property (nonatomic, assign) ANTIALIASMODE antiAliasMode;
// *************************************************************************************


// *************************************************************************************
// interpolationQuality - Added at SDK 2.3.0
// Previous SDKs did not set a value when drawing the PDF/Image on the CGContext.
// So, presumably it was using "kCGInterpolationDefault".
// However, recent tests indicate that using "kCGInterpolationNone" can produce better quality
// output, especially when using ErrorDiffusion. So, "none" will be the new default.
// If ISV notices any problems in output quality with the new SDK, this property is provided to
// allow you to change this setting to one of the other values.
// NOTE: Not adding a new enum as we did with ANTIALIASMODE, since Apple already provides one.
@property (nonatomic, assign) CGInterpolationQuality interpolationQuality;
// *************************************************************************************


// *************************************************************************************
// statusResponseTimeout:
//
// Timeout for Status Response - applicable for MFi (Bluetooth) printer models only.
// In case STATUS_RESPONSE_TIMEOUT_DEFAULT is not sufficient, we provide this property to allow ISV to change it.
//
// Quick Summary:
// * The value of statusResponseTimeout should be large enough to handle the AMOUNT OF TIME it takes to print a WORST CASE
//   page. The response(s) do not come until the page completes printing.
// * So, you should consider "WORST CASE" as follows:
//   - page has a lot of graphics, paper may not be inserted, print head overheats while printing the page, etc.
//
//
// IMPORTANT: If you do wish to set this, set it BEFORE calling "openChannel" (if using "Manual APIs") or BEFORE any of the
// "simple APIs" (e.g. "printPDFFilePathURL").
// Also, this ONLY applies DURING printing. It does not affect the "getPTStatus" API.
//
// NOTES:
// * This timeout is applicable only when "sendDataToChannel" is called with the kSendDataMode_FormFeedAtEnd, or if
//   kSendDataMode_FormFeedIndeterminate (and SDK code detects that a form feed exists in the data).
//   Since all APIs use this function, it is important to understand this.
// * Well, it is also applicable if kCloseBluetoothChannelFlushMode_UseStatusRequest is enabled.
//
// * PJ763Mfi handles this by sending a status request command and then waiting for the printer response, which does not
//   arrive until the page has completed printing.
// * RJ4030Ai handles this by reading responses that the printer sends automatically, without sending the request.
//   The final response does not arrive until the page has nearly completed printing.
//   Because these responses come only in Raster Mode, do NOT set "FormFeedAtEnd" unless you are sending Raster mode data.
//
// * Bluetooth models seems to require this to avoid what seems to be an Apple Bug with "handshaking" when printing
//   MULTIPLE PAGE jobs, especially with PJ763MFi. This printer will stop taking data for next page while the previous
//   page is being printed. However, the outputStream in EASession does not stop taking data on time.
//   So, if you write to EASession outputStream, but EASession cannot write to the printer within about 5 seconds,
//   then Apple shuts down the MFi connection. So, to get around this problem, the SDK performs this status check
//   between each page.

#define STATUS_RESPONSE_TIMEOUT_DEFAULT 60.0 // max # seconds we wait for a response from printer, by default
@property (nonatomic, assign) CFTimeInterval statusResponseTimeout;
// *************************************************************************************


// *************************************************************************************
// closeBluetoothChannelFlushMode:
//
// This property will affect only the "closeChannel" API (used by all printing APIs) for the MFi printers only.
// It is provided JUST IN CASE you run into a problem where Apple disconnects the Bluetooth device, or if all the data
// seems to have been sent correctly, but the printer doesn't print anything and needs to be power-cycled.
// If the Bluetooth channel (EASession) is closed before Apple sends all of the data to the printer, one of the two
// problems mentioned above might occur.
//
// 1. kCloseBluetoothChannelFlushMode_UsePrinterDefault (DEFAULT)
//    Each printer model will initialize this to the setting that works best for that model.
//    Choose one of the other settings to override the SDK's default value.
//
// 2. kCloseBluetoothChannelFlushMode_None
//    The channel is closed immediately.
//    Because we added the "Status Check" at the END of each page, as determined by kSendDataMode_FormFeedAtEnd,
//    this now seems to work correctly in most cases for RASTER MODE data (PDF, IMAGE, and Raster Mode PRN).
//    This is the default setting for PJ763MFi and RJ/TD Series models.
//
// 3. kCloseBluetoothChannelFlushMode_UseStatusRequest
//    Send a status request and wait for the response before closing the channel.
//    This was the best solution, until we added the status check at the end of each page, for each MFi model.
//    This assured that all data was sent before closing the channel, otherwise we would not receive the status response.
//    But, now it is redundant (with Image and PDF printing).
//
//    IMPORTANT: When sending PRN data (ESC/P, ZPL, CPCL, etc) over MFi Bluetooth, you MUST set this UseStatusRequest option!
//    Otherwise, it is highly likely the channel stream will be shut down before all the data has been sent to the printer.
//    This will result in failure to print the final page, and likely Apple will forcefully disconnect the MFi device.
//
// 4. kCloseBluetoothChannelFlushMode_UseFixedDelay
//    This was the first solution before we used the status option. But, the delay needed to be quite large (10 sec) to
//    be reliable. However, this adds this much delay every time the channel is closed.
//
// While testing this with a 10 page ESC/P PRN file, because we cannot detect each page break in this case to send the status
// check during printing, we found that Apple will disconnect the printer when we use the "None" option. So, we provide
// these other options just in case you run into a problem. In most cases, changing the default will NOT be necessary.

typedef enum
{
    kCloseBluetoothChannelFlushMode_UsePrinterDefault=0,
    kCloseBluetoothChannelFlushMode_None,
    kCloseBluetoothChannelFlushMode_UseStatusRequest,
    kCloseBluetoothChannelFlushMode_UseFixedDelay,
} CLOSE_BLUETOOTHCHANNEL_FLUSH_MODE;
@property (nonatomic, assign) CLOSE_BLUETOOTHCHANNEL_FLUSH_MODE closeBluetoothChannelFlushMode;
// *************************************************************************************


//******** MAIN API FUNCTIONS ********

// *** BRPtouchSDK function WRAPPERS ***
// NOTE: These methods are now fully implemented in the BMS SDK. They are no longer wrappers.

// wrappers for sendFile and sendFileEx
- (int) sendFile: (NSString *)filePath
     channelType: (CHANNELTYPE)channelType
   channelString:(NSString *)channelString // IPAddress or BTDeviceName
         timeout:(int)nTimeout;

- (int) sendFileEx: (NSString *)filePath
       channelType: (CHANNELTYPE)channelType
     channelString:(NSString *)channelString // IPAddress or BTDeviceName
           timeout:(int)nTimeout;

// wrappers for sendData and sendDataEx.
- (int) sendData:(NSData *)printData
     channelType:(CHANNELTYPE)channelType
   channelString:(NSString *)channelString // IPAddress or BTDeviceName
         timeout:(int)nTimeout;

- (int) sendDataEx:(NSData *)printData
       channelType:(CHANNELTYPE)channelType
     channelString:(NSString *)channelString // IPAddress or BTDeviceName
           timeout:(int)nTimeout;


// "wrapper" for getPTStatus.
// * getPTStatus allows to you determine things like the actual connected model, paper inserted, cover open, etc.
// Refer to the BRPtouchSDK documentation for more details about the format and meaning of the status response.

// TAG: REMOVE_BRPTOUCH_SDK
// When using the "RemoveBRPtouchSDK" framework, getPTStatus is NOT supported
// for the WIFI channel, because WIFI channel requires using the BRPtouchSDK API.

#ifdef REMOVE_BRPTOUCH_SDK
// Need this struct from other SDK when using our own getPTStatus implementation for Bluetooth.
typedef struct _PTSTATUSINFO {
    Byte	byHead;						// Head mark
    Byte	bySize;						// Size
    Byte	byBrotherCode;				// Brother code
    Byte	bySeriesCode;				// Serial code
    Byte	byModelCode;				// Model code
    Byte	byNationCode;				// Nation code
    Byte	byFiller;					// information about cover
    Byte	byFiller2;					// Not used
    Byte	byErrorInf;					// Error information 1
    Byte	byErrorInf2;				// Error information 2
    Byte	byMediaWidth;				// Media width
    Byte	byMediaType;				// Media type
    Byte	byColorNum;					// The number of colors
    Byte	byFont;						// Font
    Byte	byJapanesFont;				// Japanese font
    Byte	byMode;						// Mode
    Byte	byDensity;					// Density
    Byte	byMediaLength;				// Media Length
    Byte	byStatusType;				// Status Type
    Byte	byPhaseType;				// Phase type
    Byte	byPhaseNoHi;				// Upper bytes of phase number
    Byte	byPhaseNoLow;				// Lower bytes of phase number
    Byte	byNoticeNo;					// Notice number
    Byte	byExtByteNum;				// Total bytes of extended part
    Byte	byLabelColor;				// Color of label
    Byte	byFontColor;				// Color of font
    Byte	byHardWareSetting[4];		// Settings of hardware
    Byte	byNoUse[2];                 // Not Use
} PTSTATUSINFO, *LPPTSTATUSINFO;
#endif

- (int) getPTStatus:(PTSTATUSINFO *)status
              model:(PRINTERMODEL)model
        channelType:(CHANNELTYPE)channelType
      channelString:(NSString *)channelString; // IPAddress or BTDeviceName

// note: wrappers for printImage and printPDFAtPath are NOT provided



//******* BMS new functionality functions **********

//***** Misc Utilities
- (CGRect) getPrintableRectFromCurrentSettings;
- (CGRect) getPaperRectFromCurrentSettings;

//********************************************************************************
// detectPrinterResolutionForModel
// Used to determine resolution of print head installed to TD23xx models ONLY
// 203, 300 models have different byModelCode in Status Response
// Each of these models supports only 1 resolution at a time.
//
// RETURNS:
// * RET_TRUE on success, detectedResolution is VALID
// * Other error code if FAILURE. detectedResolution is INVALID
- (int) detectPrinterResolutionForModel:(PRINTERMODEL)model
                            channelType:(CHANNELTYPE)channelType
                          channelString:(NSString *)channelString // IPAddress or BTDeviceName
                     detectedResolution:(RESOLUTION *)pResolution;
//********************************************************************************


//********************************************************************************
// SIMPLE APIs
//
// These printing APIs will handle the whole job for you (including openChannel, sendDataToChannel, closeChannel).
// See also the "Manual APIs", which give you more control of the printing process.

//***** FILE Printing Functions (see also sendFile and sendFileEx above)
// NOTE: new prototype in SDK 2.0
- (int) sendFileWithProgressView:(NSURL *)filePathURL
                     channelType:(CHANNELTYPE)channelType
                   channelString:(NSString *)channelString // IPAddress or BTDeviceName
                          copies:(int)numCopies
                    writeTimeout:(NSTimeInterval)timeout
                 usingParentView:(UIViewController *)parentViewController;

//***** PDF Printing Functions (see also Manual APIs below)

// send PDF file without a ProgresView
// This is a BLOCKING function only
// NOTE: SDK 1.1.0 changed this prototype to add "copies" and "collate" parameters.
// NOTE: The "collate" parameter value only matters when printing multiple copies of a multi-page PDF file.
- (int) printPDFFilePathURL:(NSURL *)filePathURL
                  firstPage:(int)firstPage 
                   lastPage:(int)lastPage
                    timeout:(CFTimeInterval)nTimeout
                     copies:(int)copies
                    collate:(BOOL)bCollate;

// send PDF file with a MODAL ProgressView
// NOTE: new prototype in SDK 2.0
- (int) printPDFFileWithProgressView:(NSURL *)filePathURL
                           firstPage:(int)firstPage
                            lastPage:(int)lastPage
                              copies:(int)numCopies
                             collate:(BOOL)bCollate
                        writeTimeout:(NSTimeInterval)timeout
                     usingParentView:(UIViewController *)parentViewController;

// Print an ARRAY of NSURLs that reference a PDF file.
// All pages from EACH PDF file will be printed.
// NOTE: The same printSettings will apply to EACH PDF file in the array.
-(int) printPDFURLArray:(NSArray *)array
                timeout:(CFTimeInterval)nTimeout
                 copies:(int)copies
                collate:(BOOL)bCollate;


//***** Image Printing Functions (see also Manual APIs below)
// print Image with a MODAL ProgressView
// NOTE: new prototype in SDK 2.0
- (int) printCGImageRefWithProgressView:(CGImageRef)imageRef
                                 copies:(int)numCopies
                           writeTimeout:(NSTimeInterval)timeout
                        usingParentView:(UIViewController *)parentViewController;



// Print 1 or more copies of a single image, without a ProgressView.
// NOTE: The printCGImageRef:blocking:timeout API has been REMOVED in SDK 2.0.0
// The printCGImageRef:timeout:copies is now used instead.
// WARNING: Do NOT use this API to send more than 1 image back-to-back.
// Instead, see printImageURLArray or sendCGImageRefToChannel below.
- (int) printCGImageRef:(CGImageRef)imageRef
                timeout:(CFTimeInterval)nTimeout
                 copies:(int)copies;

// Print an ARRAY of NSURLs that reference an IMAGE file, e.g. JPG, PNG, etc
// NOTE: The same printSettings will apply to EACH image in the array.
-(int) printImageURLArray:(NSArray *)array
                  timeout:(CFTimeInterval)nTimeout
                   copies:(int)copies;

//********************************************************************************
// Manual API Control of the Data Channel. These all work together!!
//
// NOTE: ALL "Manual API" prototypes have CHANGED in SDK 2.0!!
// * Previous SDKs only used WIFI channel, so Manual APIs had "WIFI" in the name.
// * Since SDK 2.0 now supports WIFI and Bluetooth (and also File), the APIs have all been RENAMED!!
// * But, they are all very similar to the API names in older SDKs.
//
// How to use the Manual APIs:
// 1. You MUST call openChannelWithTimeout (PDF or Image) or openChannelOfType (PRN) first
//  * For PDF/Image, the printSettings property of BMSPrinterDriver class is used to determine channelType/String
//  * For PRN, printSettings are N/A, so channelType/String need to be specified in the openChannelOfType parameters.
// 2. Call one (or more) of the other APIs for sending print data to the current channel
// 3. Call closeChannel
//
// Advantages to using the Manual APIs compared to the simpler APIs:
// 1. Sending data this way allows you to send data in bands and display your own progress indicator.
// 2. You can use different timeouts for openChannel and sending the data. This allows timing out more quickly if
// (for example) there is a WIFI connection issue (i.e. not connected to the printer, wrong IPAddress, etc.)
//
// *** WARNING: PJ7 models will very likely require a LARGER TIMEOUT for SENDING the print data compared to other models,
// especially when printing MULTIPLE PAGE jobs. PJ7 will STOP accepting data during printing (i.e. when paper is moving).
// If the print head overheats during printing, the printer will stop printing until the head cools down, which lengthens
// the amount of time needed for the timeout.
// * We recommend setting AT LEAST 60 seconds for the SEND timeout when using PJ7 models, unless you know you will only
// be printing single page jobs or jobs without a significant page coverage.
// * We strongly recommend that you test a WORST CASE print job to assure that your SEND timeout setting is large enough.

// Use this with PDF/Image. The printSettings property is used to get the channel info.
- (int) openChannelWithTimeout:(CFTimeInterval)nTimeout;

// Use this with PRN, since printSettings are N/A.
- (int) openChannelOfType:(CHANNELTYPE)channelType
            channelString:(NSString *)channelString // IPAddress or BTDeviceName or FilePath (Print to File)
                  timeout:(CFTimeInterval)nTimeout;

// Data Generation APIs for Image and PDF, so you can retrieve the print data to send in any way that you prefer.
- (int) generatePrintData:(NSMutableData *)printData
           fromCGImageRef:(CGImageRef)imageRef;

- (int) generatePrintData:(NSMutableData *)printData
           fromPDFPageRef:(CGPDFPageRef) pdfPageRef;

// NOTE: ALL other "Manual APIs" below use the CHANNELTYPE specified in printSettings or openChannelOfType.


// This must be called when finished sending data to the channel opened by openChannelWithTimeout or
// openChannelOfType
// TODO: Consider returning "int" code instead of "void". May need to propagate errors that may occur when closing
// bluetooth channel back to ISV.
// But, these errors typically will occur only when kCloseBluetoothChannelFlushMode_UseStatusRequest is specified,
// and this is not currently the default setting for any of the MFi models. So, "void" is OK for now.
- (void) closeChannel;

// This is the best API to use if you want to make your own progress indicator.
- (int) sendDataToChannel:(const uint8_t *)data
                   length:(int)length
             sendDataMode:(SENDDATAMODE)sendDataMode
                  timeout:(CFTimeInterval)nTimeout
                errorCode:(int *)pErrorCode;

// similar to sendDataToChannel, but easier to use since you don't have to manage the buffer.
// However, it is not as useful if you wish to make your own progress indicator.
// NOTE: The data will actually be sent in 16K bands, and the timeout parameter will apply to each band separately.
- (int) sendNSDataToChannel:(NSData *)data
               sendDataMode:(SENDDATAMODE)sendDataMode
                    timeout:(CFTimeInterval)nTimeout;


// simple manual API to send a CGImageRef to already open channel. Not useful for progress indicator.
// But, you can use this to print more than 1 image back-to-back.
- (int) sendCGImageRefToChannel:(CGImageRef)imageRef
                        timeout:(CFTimeInterval)nTimeout
                         copies:(int)copies;

// simple manual API to send a PDF file URL to already open channel. Not useful for progress indicator.
// But, you can use this to print more than 1 PDF file back-to-back.
- (int) sendPDFFilePathURLToChannel:(NSURL *)filePathURL
                          firstPage:(int)firstPage
                           lastPage:(int)lastPage
                            timeout:(CFTimeInterval)nTimeout
                             copies:(int)copies
                            collate:(BOOL)bCollate;

// simple manual API to send a PRN file URL to already open channel. Not useful for progress indicator.
// But, you can use this to print more than 1 PRN file back-to-back.
- (int) sendPRNFilePathURLToChannel:(NSURL *)filePathURL
                            timeout:(CFTimeInterval)nTimeout;

// Used with Manual APIs to handle possible "Cancel" button on a Custom Progress ViewController, if desired.
// Sends data to printer to clear out a partial page and eject the paper.
// NOTE: Only use this with PDF and IMAGE printing. It is N/A when sending PRN data.
- (int) sendCancelPrintingDataToChannel;


// Primarily used for RJ4 MCR reader applications!!
- (int) readDataFromChannel:(uint8_t *)buffer
                  maxLength:(int)maxLength
                  errorCode:(int *)pErrorCode;


// End of the Manual APIs
//********************************************************************************

@end

/////////////////////////////////////////////////////////////////////////////
// This defines the DataControllerProtocol that MUST be used for developing 
// new printer drivers for a printer model. The DataControllers are used to 
// generate model-specific print data based on the current printSettings.
@protocol DataControllerProtocol <NSObject>
// everything is required!
@required

// Properties
@property (nonatomic, assign) BOOL isFirstPage;
@property (nonatomic, assign) BOOL isLastPage;
@property (nonatomic, retain) PrintSettings *printSettings;

// Methods
- (CGRect) getPaperRectFromCurrentSettings;
- (CGRect) getPrintableRectFromCurrentSettings;
- (BOOL) generatePrintData: (NSMutableData *)printData fromCGContextRef: (CGContextRef) context;
- (BOOL) generateCancelPrintingData:(NSMutableData *)printData;
@end
/////////////////////////////////////////////////////////////////////////////
