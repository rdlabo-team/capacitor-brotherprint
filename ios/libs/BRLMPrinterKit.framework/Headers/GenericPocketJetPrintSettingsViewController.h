//
//  GenericPocketJetPrintSettingsViewController.h
//  BMSPrinterKit
//
//  Created by BMS on 8/8/12.
//  Copyright (c) 2012 Brother Mobile Solutions. All rights reserved.
//

#import "PrintSettingsViewController.h"
#import "GenericPocketJetPrintSettings.h"
#import "PrintSettingTableViewController.h" // for PrintSettingTableViewResponder protocol, internal use only




//*********************************************************************
// GenericPocketJetPrintSettingsViewController INTERFACE
//
//*** IMPORTANT: *** DO NOT INSTANTIATE AN OBJECT OF THIS CLASS!!!!! ***
// The GenericPocketJetPrintSettingsViewController class is essentially an
// ABSTRACT class.
// Instead, you should instantiate the model-specific class,
// e.g. PJ673PrintSettingsViewController.
//*********************************************************************

@interface GenericPocketJetPrintSettingsViewController : PrintSettingsViewController <UITableViewDelegate, UITableViewDataSource, PrintSettingTableViewResponder>
{
}

// "style" MUST be set to UITableViewStyleGrouped!!
-(id)initWithStyle:(UITableViewStyle)style;

// SDK Designers: The PrintSettingsViewController superclass defines several properties:
//   printSettings, popoverSize, rowHeight, fontSize, printSettingsDelegate,
//   showIPAddressHelpButton, IPAddressHelpFileName, and showSettingsOptionMask.
// So, they are not defined in here.
// However, this class and/or a model-specific subclass should "init" these properties
// and handle setter overrides (if necessary).
//
// SDK Users: You should be familiar with the superclass properties so you can set
// them accordingly, in particular the printSettingsDelegate (which handles
// the PrintSettingsResponder protocol).


@end


