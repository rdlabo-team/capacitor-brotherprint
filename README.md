# @rdlabo/capacitor-brotherprint
Capacitor Brother Print is a native Brother Print SDK implementation for iOS & Android. This plugin can use in `QL-820NW` and `QL-800`.

## How to use

```typescript
import { Plugins } from '@capacitor/core';
const { BrotherPrint } = Plugins;
import { BrotherPrintOptions } from '@rdlabo/capacitor-brotherprint';

@Component({
  selector: 'brother-print',
  templateUrl: 'brother.component.html',
  styleUrls: ['brother.component.scss']
})
export class BrotherComponent {
  constructor() {
    // Success to print
    BrotherPrint.addListener('onPrint', (info) => {
      console.log("onPrint");
    });
    // Failed to communication with printer
    BrotherPrint.addListener('onPrintFailedCommunication', (info) => {
      console.log("onPrintFailedCommunication");
    });
  }
  print() {
    BrotherPrint.print({
      printerType: 'QL-820NW',
      encodedImage: 'data:image/png...', // base64
    } as BrotherPrintOptions)
  }
  printWithNetWork() {
    const wifi = () => new Promise(resolve => {
      BrotherPrint.addListener('onIpAddressAvailable', (info) => {
        resolve(info);
      });
    });

    const ble = () => new Promise(resolve => {
      BrotherPrint.addListener('onBLEAvailable', (info) => {
        resolve(info);
      });
    });
    
    Promise.all([wifi(), ble()]).then((values) => {
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
In file `android/app/src/main/java/**/**/MainActivity.java`, add the plugin to the initialization list:

```java
import jp.rdlabo.capacitor.plugin.brotherprint.BrotherPrint;

this.init(savedInstanceState, new ArrayList<Class<? extends Plugin>>() {{
    [...]
  add(BrotherPrint.class);
    [...]
}});
```

and put `BrotherPrintLibrary.aar` in your android project:
https://support.brother.co.jp/j/s/support/html/mobilesdk/guide/getting-started/getting-started-android.html

### iOS configuration
