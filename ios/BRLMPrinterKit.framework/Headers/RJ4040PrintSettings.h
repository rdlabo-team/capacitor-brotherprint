//
//  RJ4040PrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 9/12/12.
//  Copyright (c) 2012 Brother Mobile Solutions. All rights reserved.
//

#import "GenericMobileLabelModelPrintSettings.h"


@interface RJ4040PrintSettings : GenericMobileLabelModelPrintSettings <PrintSettingsDelegate>

// These definitions are provided for information only. DO NOT MODIFY!
// spec section 2.3.1 Resolution
#define RJ4040_RESOLUTION_HORZ 203
#define RJ4040_RESOLUTION_VERT 203
// spec section 2.3.5 Raster Line
#define RJ4040_HEADSIZEDOTS 832
// spec section "Select compression mode" command
// NOTE: This should be same as HEADSIZEDOTS/8. And, it should be same as the # of decompressed bytes.
// NOTE: Previously, this was 162. But, it appears to be an error in the English Command Ref Guide.
#define RJ4040_MAX_COMPRESS_LENGTH 104

//////////////////////////////////////////////
// MIN/MAX PAPERWIDTH values are not in spec.
// Used Windows Printer Driver as reference (which allows 0.47" min and 4.09" max).
//
// v1.1.1 UPDATE: According to Ann, MAX_PAPERWIDTH can actually be 4.64" instead of 4.09".
// However, the printable width can not exceed 832 dots, so setting the paperWidthDots larger than 4.09"
// will result in the marginWidthDots being increased automatically, but subsequently reducing paperWidthDots
// will not automatically result in marginWidthDots being decreased back to its previous value.
// This can be demonstrated using the PrintSettingsViewController.
//
// If someone actually has paper that is 4.64" wide, the printed result will NOT BE ANY DIFFERENT whether
// we allow 4.09" or 4.64" here, because in either case we can't use more than 832 dots, and this will be
// centered automatically on the wider paper. So, there is no reason to allow 4.64" wide here, since doing so
// potentially introduces the issue related to marginWidthDots mentioned above.
// So, let's leave MAX_PAPERWIDTHDOTS at 832 rather than change to (4.64F * RJ4040_RESOLUTION_HORZ)
//
// UPDATE2: According to Ann, MIN_PAPERWIDTH is actually 2.00" rather than 0.47".
// However, in case for some reason someone actually has narrower paper, let's not force this 2" restriction.
// So, we will keep this value the same as before as well.
//
// NOTE: MAXPAPERWIDTH does NOT exceed HEADSIZEDOTS!! (refer to v1.1.1 UPDATE discussion above. This comment
// here is only relevant in comparison to RJ3 and TD2 models, which are still allowing paper width to EXCEED
// the headsize, though perhaps we shouldn't allow this for the other models either.)
#define RJ4040_MIN_PAPERWIDTHDOTS (0.47F * RJ4040_RESOLUTION_HORZ) // 95, was using 203 in 0.9 SDK
#define RJ4040_MAX_PAPERWIDTHDOTS 832 // NOTE: WinDriver allows 4.09", which is ~832.
//////////////////////////////////////////////

// spec section 2.3.4 Maximum and Minimum Lengths
#define RJ4040_MIN_PAPERLENGTHDOTS 203 // NOTE: spec says 204 dots, but 203 seems OK.
#define RJ4040_MAX_PAPERLENGTHDOTS 24094
// spec section 2.3.3 Feed Amount
#ifdef OLD
// IMPORTANT NOTE: For LABEL papertype ONLY, we allow the MIN marginlengthdots to be set to 0.
// That is, this MIN value here is ignored when papertype = label is specified.
// Consider the case of a 1.0x0.5" label. A min margin of 0.12" will take up half of the printable length!!
// This is why we will allow the margin to be set to 0 in this case.
// Beware that some labels might clip the top if this is set to 0. But, other settings can be tweaked to
// make it work (such as by setting a larger paperlength and setting the top margin to non-zero.)
#define RJ4040_MIN_MARGINLENGTHDOTS 24

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

#define RJ4040_MIN_MARGINLENGTHDOTS 0
// v2.3.0 UPDATE: Instead of forcing this MIN_PAGEFEEDDOTS value on EVERY page, which essentially PREVENTS a "no gap"
// solution if the FW actually supports it, now we only force it on the LAST page, allowing a 0-margin between pages
// in case the FW allows it. But, we will force this on the LAST page, to assure the FW will eject the page to tear off
// in case it doesn't automatically behave that way.
#define RJ4040_MIN_PAGEFEEDDOTS_LASTPAGE 24

#endif

#define RJ4040_MAX_MARGINLENGTHDOTS 1020 // NOTE: was using 1015 in 0.9 SDK
// Arbitrarily restrict extraFeed to 11" at 203 dpi. There is no reason to be larger.
#define RJ4040_MAX_EXTRAFEEDDOTS (11*RJ4040_RESOLUTION_VERT)

//*** IMPORTANT ***
// SDK Users:
// You should NEVER instantiate an object of the GenericMobileLabelModelPrintSettings class.
// Instead, you should instantiate one of the model-specific subclasses (i.e. RJ4040PrintSettings).
//
// For a list of important properties in the settings that your application needs to set,
// refer to the GenericMobileLabelModelPrintSettings superclass.

// SDK Designers:
// The GenericMobileLabelModelPrintSettings class handles all the important work.
// This subclass only needs to provide the model-specific data to the generic class.

@end
