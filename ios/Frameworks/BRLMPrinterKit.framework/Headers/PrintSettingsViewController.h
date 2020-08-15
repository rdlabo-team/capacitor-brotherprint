//
//  PrintSettingsViewController.h
//  BMSPrinterKit
//
//  Created by BMS on 8/8/12.
//  Copyright (c) 2012 Brother Mobile Solutions. All rights reserved.
//
//  The PrintSettingsViewController class is the SUPERCLASS of each of the
//  model-specific view controller classes.
//
//  You should NEVER instantiate an object of this superclass. Instead, instantiate
//  an object of the model-specific subclass, e.g. PJ673PrintSettingsViewController.

#import <UIKit/UIKit.h>
#import "PrintSettings.h"

// The following mask constants allow customizing the options available in the PrintSettingsViewController.
// Combine these using the OR (|) operator to select only the options you want to be available.
// Set these into the property named "showSettingsOptionMask".
typedef enum
{
    // NOTE: Not all options will be available for all printer models.
    // If an unsupported option is added to the mask, it will be ignored.

    // Printer Connection Settings
    kShowSettingsOption_ChannelType    = 0x00000001,
    kShowSettingsOption_IPAddress      = 0x00000002,
    kShowSettingsOption_Bluetooth      = 0x00000004,
    // Printer-specific settings
    kShowSettingsOption_PaperSize      = 0x00000008,
    kShowSettingsOption_PaperType      = 0x00000010,
    kShowSettingsOption_FormFeedMode   = 0x00000020,
    kShowSettingsOption_ExtraFeed      = 0x00000040,
    kShowSettingsOption_Density        = 0x00000080,
    // General Settings
    kShowSettingsOption_ScaleMode      = 0x00000100,
    kShowSettingsOption_Orientation    = 0x00000200,
    kShowSettingsOption_Margins        = 0x00000400,
    kShowSettingsOption_Halftone       = 0x00000800,
    kShowSettingsOption_Threshold      = 0x00001000,
    kShowSettingsOption_Invert         = 0x00002000,

    // PJ7 model-specific settings not handled by above options
    kShowSettingsOption_RollCase       = 0x00004000,
    kShowSettingsOption_PrintSpeed     = 0x00008000,

    // RJ42 model-specific settings not handled by above options
    // NOTE: This is now in genericMLM class, but only RJ42 seems to need it.
    kShowSettingsOption_CoolingDelayEnabled = 0x00010000,
    
    // TD-4550DNWB model-specific settings not handled by above options
    kShowSettingsOption_Cutter = 0x00020000,

    // ViewController Settings
    // Normally, the TableView uses 3 sections. However, depending on the combination
    // of settings added, it may look better with only a single section.
    kShowSettingsOption_SingleSection   = 0x80000000,
    
} SETTINGS_OPTIONS_MASK;

//*********************************************************************
// PrintSettingsResponder PROTOCOL
// NOTE: Perhaps a better way to implement this would be using NSNotificationCenter.
// But, for now we will use with this approach.
/*
 A class that uses a PrintSettingsViewController is a PrintSettingsResponder.

 You are required to do 3 things if you implement this protocol:
 --------------------------------------------------------------
 1. Set the "printSettingsDelegate" property in the PrintSettingsViewController class
 object in order to receive the printSettingsDidChange notification. Since
 each PrintSettingsViewController implements the PrintSettingsSetter protocol, each
 of the model-specific ViewControllers will have this property.
 
 2. Implement the printSettingsDidChange function.
 
 3. Implement the searchingForPrinterDidChange function.
 This is a CRITICAL new callback method for apps that display PrintSettingsVC in a POPOVER control for iPad.
 If user is searching for printers via the IPAddressViewController,
 
 SDK Users:
 ---------
 You should implement this protocol if you choose to display any of the
 model-specific PrintSettingsViewControllers built-in to the SDK, but only if you 
 care to be notified when any of the settings are changed by the controller, 
 -OR- (NEW) if you allow the Search feature to be available and you show the PrintSettingsVC
 in a POPOVER control, to prevent the popover from being dismissed when search is active.
 
 For example, you can allow the view controller to change the print settings,
 and you can pass the new settings to the BMSPrinterDriver when you need to
 use them. In this case, you do not need to implement this protocol.
 
 However, you may want to "savePreferences" when any of the settings change, 
 as the SDK will not do this automatically. In this case, you do need to
 implement this protocol to call the savePreferences API in your implementation
 of the printSettingsDidChange API.
 */
//*********************************************************************
@protocol PrintSettingsResponder
- (void)printSettingsDidChange;
- (void)searchingForPrinterDidChange:(BOOL)isSearching;
@end


//*********************************************************************
// PrintSettingsSetter PROTOCOL
/*
 A ViewController that modifies print settings is a PrintSettingsSetter.
 It will call the printSettingsDidChange function that will be implemented by the
 object class specified in the printSettingsDelegate property, which is a 
 PrintSettingsResponder. 
 NOTE: It is OK to set this to "nil" if your class does not want to
 be notified when the settings change.
 
 This protocol is for SDK Designers only. 
 SDK Users do NOT need to implement this protocol.
 */
//*********************************************************************
@protocol PrintSettingsSetter
@property (assign, nonatomic) id <PrintSettingsResponder> printSettingsDelegate;
@end


//*********************************************************************
// PrintSettingsViewController INTERFACE
//
// DO NOT INSTANTIATE an object of this class.
// ALWAYS INSTANTIATE a model-specific object, e.g. PJ673PrintSettingsViewController.
//
// This is essentially an ABSTRACT class that does nothing on its own.
//
// NOTE: You can use a generic variable of the PrintSettingsViewController class to contain
// the model-specific instantiation, and this reference which will have access to the
// following properties/methods.
//*********************************************************************
@interface PrintSettingsViewController : UITableViewController <PrintSettingsSetter, PrintSettingsResponder>
{
    // ivars needed during subclass init, defining here allows subclasses to access ivar directly without setter.
    BOOL _showPrinterSearchButton;
    
    // these two will combine constants defined by enum SETTINGS_OPTIONS_MASK
    // They allow ISV to customize the setting options available in the ViewController
    unsigned int _showSettingsOptionMask;
    unsigned int _supportedOptions; // read only, specific for each printer model
}

//*** PrintSettingsSetter protocol
@property (assign, nonatomic) id <PrintSettingsResponder> printSettingsDelegate;

//*** PrintSettingsResponder protocol
- (void)printSettingsDidChange;

//*** The following properties are defined in this super-class so they are available
// to an APP that stores the printer-specific object into a generic super-class object
// and then wishes to modify these properties.
@property (nonatomic, retain) PrintSettings *printSettings;

// Allows SDK User to modify the default tableview properties
@property (nonatomic, assign) CGSize popoverSize;
@property (nonatomic, assign) int rowHeight;
@property (nonatomic, assign) int fontSize;
@property (nonatomic, assign) CGFloat section0HeaderSize;
@property (nonatomic, assign) CGFloat otherSectionHeaderSize;
@property (nonatomic, assign) CGFloat sectionFooterSize;



//*** These 2 properties allow customizing the IPAddressHelpViewController.
// * showIPAddressHelpButton is passed through to the IPAddressViewController to decide
// whether to show the Help Button at all.
// * IPAddressHelpFileName is passed thru to the IPAddressHelpViewController.
// This allows SDK User to change the Help File displayed, if desired.
// The file MUST be a "pdf" file (with lower-case file extension in the actual file),
// and you MUST omit the ".pdf" from the filename specified in this property.
@property (nonatomic, assign) BOOL showIPAddressHelpButton;
@property (nonatomic, retain) NSString *IPAddressHelpFileName;

// Allows SDK User to modify the settings options displayed in the tableview.
// c.f. SETTINGS_OPTIONS_MASK enum for these two properties!
@property (nonatomic, assign) unsigned int showSettingsOptionMask;
// READONLY property. Value will be set by the subclass. SDK User can NOT change it.
@property (readonly, nonatomic, assign) unsigned int supportedOptions;

// By default, the Search button will be available on the IPAddressViewController for all models EXCEPT PJ673,
// which does not support the search feature. This property allows SDK user to hide the search button if desired.
// NOTE: This same property is used for the Search button on the BluetoothViewController too!!
@property (nonatomic, assign) BOOL showPrinterSearchButton;
// New in SDK 2.1.1. Default is YES, which means that an IPv6 address format will be returned by the
// "IPAddress Search" instead of IPv4 format, but only if IPv6 address is available.  It will only be
// available when connected by Infrastructure Mode to an IPv6-only network, which is not a typical use mode.
// If for some reason you don't want this behavior, i.e. you only want to see IPv4 formatted addresses,
// this property is provided to allow you to override the default behavior. However, beware that Apple
// **MAY** require the IPv6 format address, based on their new (June 1, 2016) IPv6 policy.
@property (nonatomic, assign) BOOL showIPv6AddressIfAvailable;



// NOTE: subclasses MUST set these in their init method to model-specific values
@property (nonatomic, retain) NSString *mainTitle; // ViewController title
@property (nonatomic, retain) NSString *section1Title; // main TableView section1 title

@end

