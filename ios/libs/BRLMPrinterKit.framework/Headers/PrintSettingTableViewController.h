//
//  PrintSettingTableViewController.h
//  BMSPrinterKit
//
//  Created by BMS on 2/11/13.
//  Copyright (c) 2013 Brother Mobile Solutions. All rights reserved.
//
// NOTE: This is a generic implementation for all print settings that use a
// TableView to display the settings. It handles only a single setting at a time.
//
// NOTE: This class is used INTERNALLY to the SDK only. But, we need to make this a PUBLIC header file
// because other public headers need to #import it due to the PrintSettingTableViewResponder protocol.

#import <UIKit/UIKit.h>

@protocol PrintSettingTableViewResponder
// The "tableViewItemChanged" function must be implemented by the class that
// uses this PrintSettingTableViewController.
// This function is responsible to:
// (1) set the new value to its copy of the printSettings,
// (2) handle any "mutually exclusive" settings (such as FFMode and PaperType),
// (3) call "printSettingsDidChange" function per the "PrintSettingsSetter"
// protocol to pass the settings change notification along
- (void)tableViewItemChanged: (int)whichSetting newItemValue:(int)newItemValue;
@end

@protocol PrintSettingTableViewSetter
@property (assign, nonatomic) id <PrintSettingTableViewResponder> printSettingDelegate;
@end


@interface PrintSettingTableViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource, PrintSettingTableViewSetter>
{
    UITableViewCell *_cellWithCheck;
}
// used internally only
@property (nonatomic, retain) UITableViewCell *cellWithCheck;

// Must be set by caller.
@property (nonatomic, assign) int whichPrintSetting;
@property (nonatomic, retain) NSString *viewTitle;
@property (nonatomic, retain) NSArray *tableItemArray;
@property (nonatomic, assign) int initialItemValueToCheck;
@property (nonatomic, assign) UITableViewCellStyle cellStyle;
@property (nonatomic, retain) NSString *section0Title;

// The class which implements the printSettingDidChange function.
// This MUST be set by the CALLER before we are loaded. It cannot be nil!!
@property (assign, nonatomic) id <PrintSettingTableViewResponder> printSettingDelegate;

@end
