//
//  TD2130NPrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 9/18/13.
//  Copyright (c) 2013 Brother Mobile Solutions. All rights reserved.
//

#import "GenericMobileLabelModelPrintSettings.h"


@interface TD2130NPrintSettings : GenericMobileLabelModelPrintSettings <PrintSettingsDelegate>

// These definitions are provided for information only. DO NOT MODIFY!
// spec section 2.3.1 Resolution
#define TD2130N_RESOLUTION_HORZ 300
#define TD2130N_RESOLUTION_VERT 300
// spec section 2.3.5 Raster Line
#define TD2130N_HEADSIZEDOTS 672
// spec section "Select compression mode" command
// NOTE: This should be same as HEADSIZEDOTS/8. And, it should be same as the # of decompressed bytes.
#define TD2130N_MAX_COMPRESS_LENGTH 84

// not in spec. Used Windows Printer Driver as reference.
// NOTE: MAXPAPERWIDTH can exceed HEADSIZEDOTS!!
#define TD2130N_MIN_PAPERWIDTHDOTS (0.47F * TD2130N_RESOLUTION_HORZ) // 141
#define TD2130N_MAX_PAPERWIDTHDOTS (2.48F * TD2130N_RESOLUTION_VERT) // 744
// spec section 2.3.4 Maximum and Minimum Lengths
#define TD2130N_MIN_PAPERLENGTHDOTS 142
#define TD2130N_MAX_PAPERLENGTHDOTS 11811
// spec section 2.3.3 Feed Amount
#ifdef OLD
// IMPORTANT NOTE: For LABEL papertype ONLY, we allow the MIN marginlengthdots to be set to 0.
// That is, this MIN value here is ignored when papertype = label is specified.
// Consider the case of a 1.0x0.5" label. A min margin of 0.12" will take up half of the printable length!!
// This is why we will allow the margin to be set to 0 in this case.
// Beware that some labels might clip the top if this is set to 0. But, other settings can be tweaked to
// make it work (such as by setting a larger paperlength and setting the top margin to non-zero.)
#define TD2130N_MIN_MARGINLENGTHDOTS 35

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

#define TD2130N_MIN_MARGINLENGTHDOTS 0
// v2.3.0 UPDATE: Instead of forcing this MIN_PAGEFEEDDOTS value on EVERY page, which essentially PREVENTS a "no gap"
// solution if the FW actually supports it, now we only force it on the LAST page, allowing a 0-margin between pages
// in case the FW allows it. But, we will force this on the LAST page, to assure the FW will eject the page to tear off
// in case it doesn't automatically behave that way.
#define TD2130N_MIN_PAGEFEEDDOTS_LASTPAGE 35

#endif

#define TD2130N_MAX_MARGINLENGTHDOTS 1500
// Arbitrarily restrict extraFeed to 11" at 203 dpi. There is no reason to be larger.
#define TD2130N_MAX_EXTRAFEEDDOTS (11*TD2130N_RESOLUTION_VERT)

//*** IMPORTANT ***
// SDK Users:
// You should NEVER instantiate an object of the GenericMobileLabelModelPrintSettings superclass.
// Instead, you should instantiate one of the model-specific subclasses (i.e. TD2130NPrintSettings).
//
// For a list of important properties in the settings that your application needs to set,
// refer to the GenericMobileLabelModelPrintSettings superclass.

// SDK Designers:
// The GenericMobileLabelModelPrintSettings superclass handles all the important work.
// This subclass only needs to provide the model-specific data to the generic class(es).

//*** ADDITIONAL PROPERTIES not handled by GenericMobileLabelModelPrintSettings
@property (nonatomic, assign) BOOL peelerEnabled;
@property (nonatomic, assign) BOOL invert180degrees;

@end
