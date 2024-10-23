# @rdlabo/capacitor-brotherprint

Capacitor Brother Print is a native Brother Print SDK implementation for iOS & Android. This plugin can use in `QL-820NW` and `QL-800`.

## How to install

```
% npm install @rdlabo/capacitor-brotherprint@git@github.com:rdlabo-team/capacitor-brotherprint.git
```

## How to use

```typescript
import { Plugins } from '@capacitor/core';
const { BrotherPrint } = Plugins;
import { BrotherPrintOptions } from '@rdlabo/capacitor-brotherprint';

@Component({
  selector: 'brother-print',
  templateUrl: 'brother.component.html',
  styleUrls: ['brother.component.scss'],
})
export class BrotherComponent {
  constructor() {
    // Success to print
    BrotherPrint.addListener('onPrint', info => {
      console.log('onPrint');
    });
    // Failed to communication with printer
    BrotherPrint.addListener('onPrintError', info => {
      console.log('onPrintError');
    });
    // Failed to communication with printer
    BrotherPrint.addListener('onPrintFailedCommunication', info => {
      console.log('onPrintFailedCommunication');
    });
  }
  print() {
    BrotherPrint.printImage({
      printerType: 'QL-820NW',
      encodedImage: 'base64 removed mime-type', // base64
    } as BrotherPrintOptions);
  }
  printWithNetWork() {
    const wifi = () =>
      new Promise(resolve => {
        BrotherPrint.addListener('onIpAddressAvailable', info => {
          resolve(info);
        });
      });

    const ble = () =>
      new Promise(resolve => {
        BrotherPrint.addListener('onBLEAvailable', info => {
          resolve(info);
        });
      });

    Promise.all([wifi(), ble()]).then(values => {
      console.log(values);
    });

    BrotherPrint.searchWiFiPrinter();
    BrotherPrint.searchBLEPrinter();
  }
}
```

## Installation

```
$ npm install --save @rdlabo/capacitor-brotherprint
```

### Android configuration

1. prepare the following files under the android folder of Capacitor

- android/BrotherPrintLibrary/BrotherPrintLibrary.aar
- android/BrotherPrintLibrary/build.gradle

`BrotherPrintLibrary.aar` is the Brother Print SDK library. You can get it from the Brother website: https://support.brother.co.jp/j/s/es/dev/ja/mobilesdk/android/index.html?c=jp&lang=ja&navi=offall&comple=on&redirect=on#ver4

The contents of `android/BrotherPrintLibrary/build.gradle` is this.

```
configurations.maybeCreate(“default”)
artifacts.add(“default”, file('BrotherPrintLibrary.aar'))
```

Add the following to android/settings.gradle.

```
include ':BrotherPrintLibrary.aar''
include ':BrotherPrintLibrary'.
project(':BrotherPrintLibrary').projectDir = new File('. /BrotherPrintLibrary/')
```

### iOS configuration

No configuration required.
