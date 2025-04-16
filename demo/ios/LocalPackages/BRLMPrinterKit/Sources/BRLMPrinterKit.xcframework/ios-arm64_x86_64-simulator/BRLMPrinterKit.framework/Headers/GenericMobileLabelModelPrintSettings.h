//
//  GenericMobileLabelModelPrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 9/12/12.
//  Copyright (c) 2012-13 Brother Mobile Solutions. All rights reserved.
//

#import <BRLMPrinterKit/PrintSettings.h>



@interface GenericMobileLabelModelPrintSettings : PrintSettings <PrintSettingsDelegate>
{
    // provide subclasses with access to readonly properties via ivars.
    // Each subclass should set these to their model-specific values, at least if the
    // generic defaults are not accurate for each model.

    // Because they are readonly, there is no "Setter" method available in the superclass.
    // The subclasses should be the only place where these properties are set.
    // But, they will be used in this superclass.
    // Users of this superclass can read the values if desired, but in most cases this
    // will not be necessary.
    int _resolutionHorz;
    int _resolutionVert;
    int _headSizeDots;
    int _paperWidthDotsMin;
    int _paperWidthDotsMax;
    int _paperLengthDotsMin;
    int _paperLengthDotsMax;
    int _marginLengthDotsMin;
    int _marginLengthDotsMax;
    int _extraFeedMax;

    unsigned char _energyRankMax; // min is always 0. Max depends on printer model.
    // TD-4D and TD2a is +/-5, all others are +/-2 for sensor threshold adjust.
    signed char _sensThrAdjMin;
    signed char _sensThrAdjMax;
    // NOTE: All models are +/-2 for sensor duty adjust, so not adding min/max for that one yet.
}

// the following are defined somewhat arbitrarily to limit these settings to a
// reasonable value during validation.
// NOTE: We do NOT need "read only" properties for these since they will be the same
// for each model and subclass.
#define LABELSPACINGDOTS_MAX 1000
#define MARKWIDTHDOTS_MAX 1000
#define MARKOFFSETDOTS_MIN -1000
#define MARKOFFSETDOTS_MAX 1000


//*** IMPORTANT (for SDK Users):
// *** DO NOT INSTANTIATE AN OBJECT OF THIS CLASS!!!!! ***
// The GenericMobileLabelModelPrintSettings class is essentially an ABSTRACT class.
// Instead, you should instantiate the model-specific class, e.g. RJ4040PrintSettings.

//*** The following properties should be set by your app code, either PROGRAMMATICALLY
// or by using one of the PrintSettingsViewControllers (which may not set all properties).

// The properties and methods in this super-class will be COMMON for all
// of the RJ and TD printer models.
// They will be implemented in here generically, but will use the genericModelDelegate
// property above to get model-specific information.

// SDK Users: All of the properties below (and those in the PrintSettings superclass)
// are settable by you to change the printer behavior.
// If you are not using the PrintSettingsViewController (either the SDK's or your own custom one),
// then you should set each of these properties programmatically to your desired setting.
@property (nonatomic, assign) PAPERTYPE paperType;
// formFeedMode = NOFEED mode is used to implement "trimTapeAfterData" from Windows driver.
// This allows clipping white space from the bottom of the job to save paper.
@property (nonatomic, assign) FORMFEEDMODE formFeedMode;
@property (nonatomic, assign) DENSITY density;
@property (nonatomic,assign) int extraFeed; // used with kFormFeedModeNoFeed only

//*** PAPER SIZE is defined by setting ALL of the following properties.
// They are used to generate the "Esc iUw[128 bytes]" command.

// Paper Size is defined by a Width and Length.
// Unprintable Margins impact printable area within this defined size.
// * Printable height will be calculated as:
//        "printableHeight = paperLengthDots - 2*marginLengthDots"
// * Printable width will be calculated as:
//        "printableWidth = paperWidthDots - 2*marginWidthDots"
// Also, since printer paper is "center justified", anything less than 4" width needs to be
// compensated for using "rightOffsetBytes" to shift the data relative to printhead origin.
// We calculate "rightOffsetBytes" in the DRIVER based on paperWidthDots and marginWidthDots.
//
// NOTE: RJ4040 requires the left/right margins (marginWidth) to be the same as each other,
// and it requires top/bottom margins (marginLength) to be the same as each other.
// Although TD2 firmware allows these to be different, we are NOT going to support that
// functionality. If any margin settings are needed that require these unprintable margins
// to be different, then use the "marginDots" property in the superclass to make these adjustments.
@property (nonatomic, assign) int paperWidthDots;
@property (nonatomic, assign) int paperLengthDots;
@property (nonatomic, assign) int16_t marginWidthDots; // left and right unprintable margin
@property (nonatomic, assign) int16_t marginLengthDots; // top and bottom unprintable margin
// this is for labels only:
@property (nonatomic, assign) int16_t labelSpacingDots; // distance between labels
// These 2 are for Marked Paper only:
@property (nonatomic, assign) int16_t markWidthDots; // width of mark
@property (nonatomic, assign) int16_t markOffsetDots; // distance from leading edge to marks. Can be negative.

//*** Advanced Paper Size Adjustments
// The following properties are also added to Esc iUw (additionalMediaInfo) command.
// In Windows Driver, these are on "Advanced Printing Adjustments..." when editing a PaperSize.
//
// Until now they have been hard-coded to match defaults in Windows driver.
// In SDK version 2.3.4, we made these available for you to modify if necessary.
// In SDK version 2.5.1 => v4.3.2, we added (most of) these to the ViewControllers.
// NOTE: Added new "min/max" read-only properties for these too, in order to allow sub-classes
// to set their min/max values, based on the specific printer model.
@property (nonatomic, assign) unsigned char energyRank;  // similar to density.
                                                // Range is 0-10
                                                // TD-4550 range is 0-30.
                                                // RJ-3230B, RJ-3250WB range is 0-20
                                                // Default depends on printer model and paper type.
                                                // Set to '\xFF' (-1) to allow SDK to set default, same as SDK versions < v2.3.4.

// NOTE: These are N/A for RJ-4000 series. But they are supported with all other RJ/TD models.
@property (nonatomic, assign) signed char sensThrAdj;  // range = 0xFE (-2) to 0x02 (2), default = 0
                                                       // TD-4550 range = 0xFB (-5) to 0x05 (5), default = 0
@property (nonatomic, assign) signed char sensDutyAdj; // range = 0xFE (-2) to 0x02 (2), default = 0

// TODO NOTE: These 2 horz/vert position adjustments do NOT work. So, IGNORE these for now.
// It's not clear why these are properties are included in the "Esc iUw[]" data structure,
// since setting them doesn't change the output. Based on Windows driver behavior, it appears
// that the DRIVER is expected to modify the print data. If that's the case, then these really
// do not belong inside the paper definition, in my opinion.
#ifdef WHAT_THE_SPEC_SAYS
// This range is small, especially at 300 dpi.
// And, it doesn't match the size of the field in the struct or the Windows Driver.
@property (nonatomic, assign) int16_t horzPosAdjDots; // range = 0x80 (-128) to 0x7F (127), default = 0
@property (nonatomic, assign) int16_t vertPosAdjDots; // range = 0x80 (-128) to 0x7F (127), default = 0
#else
// Windows driver allows +/- 5.91 inches at printer resolution.
// And, the value in Esc iUw structure is a 16-bit signed integer.
// So, these values in comments seem to be the right ones.
@property (nonatomic, assign) int16_t horzPosAdjDots; // range = +/- 5.91*resolution, default = 0
@property (nonatomic, assign) int16_t vertPosAdjDots; // range = +/- 5.91*resolution, default = 0
#endif


// By default, the "Esc iUw [128 bytes]" command  will be sent by computing the 128-byte
// structure fields from the paperSize and other property settings.
// However, some types of media may not be adequately handled. In this case, we provide the
// option to disable sending this command. However, it will then be necessary to use the
// Windows driver to send a job to the printer to cause the appropriate "Esc iUw" command to
// be sent to the printer.
// *** The printer will remember the settings from the last time this command was sent. ***
// It will be important to assure that this class' property settings closely match the
// settings sent by the Windows driver.
@property (nonatomic, assign) BOOL sendAdditionalMediaInfoCommand;


// Enable/Disable the FIXED 2 Sec Delay at end of **EACH** page when printing by MFi Bluetooth.
// SDK v2.3.2 forced this delay for RJ-42 models ONLY, without this configurable property.
// SDK v2.3.3 added this property to allow you to turn it ON only if needed, for RJ-42 series only.
// SDK v2.3.4 moved this to Generic class, so it can be used with other models in case it is needed.
//   * However, we haven't seen the problem occur with any printers other than RJ-4200 series.
//   * We provide this capability with other models just in case it is helpful.
//
// When Enabled:
//  * A fixed 2-second delay is added at the END OF EACH PAGE, for MFi Bluetooth Channel ONLY.
//  * This delay seems to fix a problem that occurs with RJ-42 models which can cause MFi to disconnect
//    when the printer goes into a cooling state while printing multiple page jobs.
//  * However, the delay is NOT needed in most "normal" conditions, so enabling it can slow you down.
//    Therefore, the default value is NO (Disabled).
//
// When Disabled (default):
//  * NO 2-sec delay at end of each page, which is normal for all printers released prior to RJ-4200 series.
//  * However, when printing multiple page jobs on RJ-4200 series, there is a possibility that MFi can be disconnected
//    by Apple when printer begins "Cooling". This seems to be a FW bug with RJ-42 models only. However, the possibility
//    exists that other models can have this problem too.
//
// Generally, we recommend setting NO unless your "worst case" scenario results in the MFi being disconnected
// for your printer model. If so, then we suggest giving user a CHOICE to enable/disable this so the delay doesn't
// occur on every page, unless you are OK with this delay happening every page.
//
@property (nonatomic, assign) BOOL coolingDelayEnabled;

//*** The properties below are READ ONLY. SDK Users can NOT set them!!
// Each subclass MUST set them to their model-specific values in their init method.
// The purpose of this GenericMobileLabelModelPrintSettings class is to implement COMMON
// code so it does not have to be duplicated for each similar printer model. However, it
// requires these model-dependent properties to be set by the SUBCLASS.
@property (readonly, nonatomic, assign) int resolutionHorz;
@property (readonly, nonatomic, assign) int resolutionVert;
@property (readonly, nonatomic, assign) int headSizeDots;
@property (readonly, nonatomic, assign) int paperWidthDotsMin;
@property (readonly, nonatomic, assign) int paperWidthDotsMax;
@property (readonly, nonatomic, assign) int paperLengthDotsMin;
@property (readonly, nonatomic, assign) int paperLengthDotsMax;
@property (readonly, nonatomic, assign) int marginLengthDotsMin;
@property (readonly, nonatomic, assign) int marginLengthDotsMax;
@property (readonly, nonatomic, assign) int extraFeedMax;
@property (readonly, nonatomic, assign) unsigned char energyRankMax;
@property (readonly, nonatomic, assign) signed char sensThrAdjMin;
@property (readonly, nonatomic, assign) signed char sensThrAdjMax;



// *** PrintSettingsDelegate protocol
// All subclasses of PrintSettings are required to implement these methods.
// validatePrintSettings - This function assures that all properties are in range
// and that there are no conflicting settings. 
- (BOOL)validatePrintSettings;
- (void) loadPreferences;
- (void) savePreferences;
//generate strings from CURRENT settings, for convenience.
// This is part of the PrintSettingsDelegateProtocol too.
- (NSString *)stringFromCurrentPaperType;
// NOTE: PaperSize uses combination of internal settings to generate a string, formatted in inches.
- (NSString *)stringFromCurrentPaperSize;
- (NSString *)stringFromCurrentFormFeedMode;
- (NSString *)stringFromCurrentExtraFeed;
- (NSString *)stringFromCurrentDensity;


//*** generate strings from constants, for convenience
- (NSString *)stringFromPaperType: (PAPERTYPE)paperType;
// NOTE: paperSize for these models does not have a constant, so you can only use the CURRENT version below.
- (NSString *)stringFromFormFeedMode: (FORMFEEDMODE)ffmode;
- (NSString *)stringFromExtraFeed:(int)extraFeed;
- (NSString *)stringFromDensity: (DENSITY)density;


//*** TD23xx function only. Adding here so we don't have to implement in each subclass,
// and adding to this Generic interface avoids a warning.
-(NSString *)stringFromResolution:(RESOLUTION)resolution;

-(void)updatePaperInfoFromOldResolution:(RESOLUTION)old
                        toNewResolution:(RESOLUTION)new;

@end
