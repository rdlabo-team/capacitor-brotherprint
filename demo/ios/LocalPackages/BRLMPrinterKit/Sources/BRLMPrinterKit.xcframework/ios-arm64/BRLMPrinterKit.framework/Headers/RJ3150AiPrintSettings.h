//
//  RJ3150AiPrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 9/12/12.
//  Copyright (c) 2012 Brother Mobile Solutions. All rights reserved.
//

#import <BRLMPrinterKit/GenericMobileLabelModelPrintSettings.h>


@interface RJ3150AiPrintSettings : GenericMobileLabelModelPrintSettings <PrintSettingsDelegate>

// These definitions are provided for information only. DO NOT MODIFY!
// spec section 4.3.1 Resolution
#define RJ3150Ai_RESOLUTION_HORZ 203
#define RJ3150Ai_RESOLUTION_VERT 203
// spec section 4.3.6 Raster Line
#define RJ3150Ai_HEADSIZEDOTS 576
// spec section "Select compression mode" command
// NOTE: This should be same as HEADSIZEDOTS/8. And, it should be same as the # of decompressed bytes.
#define RJ3150Ai_MAX_COMPRESS_LENGTH 72

// NOT in spec. *** Used Windows Printer Driver as reference ***
// NOTE: MAXPAPERWIDTH DOES exceed HEADSIZEDOTS!!
#define RJ3150Ai_MIN_PAPERWIDTHDOTS (0.47F * RJ3150Ai_RESOLUTION_HORZ) // 95
#define RJ3150Ai_MAX_PAPERWIDTHDOTS (3.15F * RJ3150Ai_RESOLUTION_HORZ) // 639.
// spec section 4.3.4 Maximum and Minimum Lengths
#define RJ3150Ai_MIN_PAPERLENGTHDOTS 96
#define RJ3150Ai_MAX_PAPERLENGTHDOTS 7992
// spec section 4.3.3 Feed Amount
#ifdef OLD
// IMPORTANT NOTE: For LABEL papertype ONLY, we allow the MIN marginlengthdots to be set to 0.
// That is, this MIN value here is ignored when papertype = label is specified.
// Consider the case of a 1.0x0.5" label. A min margin of 0.12" will take up half of the printable length!!
// This is why we will allow the margin to be set to 0 in this case.
// Beware that some labels might clip the top if this is set to 0. But, other settings can be tweaked to
// make it work (such as by setting a larger paperlength and setting the top margin to non-zero.)
#define RJ3150Ai_MIN_MARGINLENGTHDOTS 24

#else
// Allow 0 now for continuous roll too, as the Esc iUw command generated from this can affect AirPrint.
// However, if this is 0, then the Esc id command will send 0 too. And, the printer may not eject the page
// enough at the end. So, I am introducing the MIN_PAGEFEEDDOTS definition now, so that if current marginLengthDots
// is LESS than MIN_PAGEFEEDDOTS, we will send Esc id command using the MIN_PAGEFEEDDOTS value instead.
// Otherwise, the Esc id command will use the marginLengthDots value as it has in the past.
//
// NOTE: If this occurs, the final paper length after tear off will be up to 1/4" longer than the
// specified paperLengthDots, because the print data will be (up to) the entire paper length, and then the
// PAGEFEEDDOTS will be added to the final paper length. If you need a specific paperlength, then make sure
// that marginLengthDots is >= MIN_PAGEFEEDDOTS.

#define RJ3150Ai_MIN_MARGINLENGTHDOTS 0
// v2.3.0 UPDATE: Instead of forcing this MIN_PAGEFEEDDOTS value on EVERY page, which essentially PREVENTS a "no gap"
// solution if the FW actually supports it, now we only force it on the LAST page, allowing a 0-margin between pages
// in case the FW allows it. But, we will force this on the LAST page, to assure the FW will eject the page to tear off
// in case it doesn't automatically behave that way.
#define RJ3150Ai_MIN_PAGEFEEDDOTS_LASTPAGE 24

#endif

#define RJ3150Ai_MAX_MARGINLENGTHDOTS 1015
// Arbitrarily restrict extraFeed to 11" at 203 dpi. There is no reason to be larger.
#define RJ3150Ai_MAX_EXTRAFEEDDOTS (11*RJ3150Ai_RESOLUTION_VERT)

//*** IMPORTANT ***
// SDK Users:
// You should NEVER instantiate an object of the GenericMobileLabelModelPrintSettings class.
// Instead, you should instantiate one of the model-specific subclasses (i.e. RJ3150AiPrintSettings).
//
// For a list of important properties in the settings that your application needs to set,
// refer to the GenericMobileLabelModelPrintSettings superclass.

// SDK Designers:
// The GenericMobileLabelModelPrintSettings class handles all the important work.
// This subclass only needs to provide the model-specific data to the generic class.

//*** ADDITIONAL PROPERTIES not handled by GenericMobileLabelModelPrintSettings
@property (nonatomic, assign) BOOL peelerEnabled;
@property (nonatomic, assign) BOOL invert180degrees;

@end
