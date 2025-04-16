//
//  TD4550DNWBPrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 5/16/19.
//  Copyright (c) 2019 Brother Mobile Solutions. All rights reserved.

//

#import <BRLMPrinterKit/GenericMobileLabelModelPrintSettings.h>


@interface TD4550DNWBPrintSettings : GenericMobileLabelModelPrintSettings <PrintSettingsDelegate>

// These definitions are provided for information only. DO NOT MODIFY!
// spec section 4.3.1 Resolution
#define TD4550DNWB_RESOLUTION_HORZ 300
#define TD4550DNWB_RESOLUTION_VERT 300
// spec section 4.3.5 Raster Line
#define TD4550DNWB_HEADSIZEDOTS 1280   // 4.2666" at 300 dpi
// spec section "Select compression mode" command
// NOTE: This should be same as HEADSIZEDOTS/8. And, it should be same as the # of decompressed bytes.
#define TD4550DNWB_MAX_COMPRESS_LENGTH 160

//////////////////////////////////////////////
// MIN/MAX PAPERWIDTH values are ** NOT ** in spec.
// Used Windows Printer Driver as reference (which allows 0.75" min and 4.65" max).
//
#define TD4550DNWB_MIN_PAPERWIDTHDOTS (0.75F * TD4550DNWB_RESOLUTION_HORZ)
#define TD4550DNWB_MAX_PAPERWIDTHDOTS (4.65F * TD4550DNWB_RESOLUTION_HORZ)
//////////////////////////////////////////////

//*** spec section 4.3.4 Maximum and Minimum Lengths
#define TD4550DNWB_MIN_PAPERLENGTHDOTS 142
#define TD4550DNWB_MAX_PAPERLENGTHDOTS 35433

//*** spec section 4.3.3 Feed Amount
//
// Instead of forcing MIN_PAGEFEEDDOTS value on EVERY page, which essentially PREVENTS a "no gap"
// solution if the FW actually supports it, now we only force it on the LAST page, allowing a 0-margin between pages
// in case the FW allows it. But, we will force this on the LAST page, to assure the FW will eject the page to tear off
// in case it doesn't automatically behave that way.
#define TD4550DNWB_MIN_MARGINLENGTHDOTS 0
#define TD4550DNWB_MIN_PAGEFEEDDOTS_LASTPAGE 35

#define TD4550DNWB_MAX_MARGINLENGTHDOTS 1500 // 5" at 300 dpi
// Arbitrarily restrict extraFeed to 11" at 203 dpi. There is no reason to be larger.
#define TD4550DNWB_MAX_EXTRAFEEDDOTS (11*TD4550DNWB_RESOLUTION_VERT)

#define TD4550DNWB_MAX_AUTOCUT_NUMLABELS 255 // NOTE: Windows driver uses 99 here, spec says 255.



//*** IMPORTANT ***
// SDK Users:
// You should NEVER instantiate an object of the GenericMobileLabelModelPrintSettings class.
// Instead, you should instantiate one of the model-specific subclasses (i.e. TD4550DNWBPrintSettings).
//
// For a list of important properties in the settings that your application needs to set,
// refer to the GenericMobileLabelModelPrintSettings superclass.

// SDK Designers:
// The GenericMobileLabelModelPrintSettings class handles all the important work.
// This subclass only needs to provide the model-specific data to the generic class.

//*** ADDITIONAL PROPERTIES not handled by GenericMobileLabelModelPrintSettings
@property (nonatomic, assign) BOOL peelerEnabled;
@property (nonatomic, assign) BOOL invert180degrees;

// The following are used with the cutter
@property (nonatomic, assign) BOOL cutAtEndOfJobEnabled; // true: cuts at end of last page of job
@property (nonatomic, assign) BOOL autoCutEnabled; // true: automatically cuts every N labels, as specified by autoCutEveryNLabels
@property (nonatomic, assign) unsigned int autoCutEveryNLabels; // default is 1 -> autocut each label. Set value to print N labels before cutting. Allowed Range is 1 to TD4550DNWB_MAX_AUTOCUT_NUMLABELS.


@end
