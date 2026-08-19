# Usage

The following Angular component shows how to set up listeners, search for printers, and print a base64-encoded image.

```typescript
@Component({
  selector: 'brother-print',
  templateUrl: 'brother.component.html',
  styleUrls: ['brother.component.scss'],
})
export class BrotherComponent implements OnInit, OnDestroy {
  readonly #listenerHandlers: PluginListenerHandle[] = [];
  readonly printers = signal<BRLMChannelResult[]>([]);

  async ngOnInit() {
    this.#listenerHandlers.push(
      await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrint, () => {
        console.log('onPrint');
      }),
    );
    this.#listenerHandlers.push(
      await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrintError, (info) => {
        console.log('onPrintError');
      }),
    );
    this.#listenerHandlers.push(
      await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrintFailedCommunication, (info) => {
        console.log('onPrintFailedCommunication');
      }),
    );
    this.#listenerHandlers.push(
      await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrinterAvailable, (printer) => {
        this.printers.update((prev) => [...prev, printer]);
      }),
    );
  }

  async ngOnDestroy() {
    this.#listenerHandlers.forEach((handler) => handler.remove());
  }

  async searchPrinter(port: BRKMPrinterPort) {
    // This method return void. Get the printer list by listening to the event.
    await BrotherPrint.search({
      port,
      searchDuration: 15, // seconds
    });
  }

  print() {
    if (this.printers().length === 0) {
      console.error('No printer found');
      return;
    }

    const defaultPrintSettings: BRLMPrintOptions = {
      modelName: BRLMPrinterModelName.QL_820NWB,
      labelName: BRLMPrinterLabelName.RollW62,
      encodedImage: 'base64 removed mime-type', // base64
      numberOfCopies: 1, // default 1
      autoCut: true, // default true
    };

    BrotherPrint.printImage({
      ...defaultPrintSettings,
      ...{
        port: this.printers()[0].port,
        channelInfo: this.printers()[0].channelInfo,
      },
    });
  }
}
```

See demo for complete code:
https://github.com/rdlabo-dev/capacitor-brotherprint/blob/main/demo/src/app/home/home.page.ts
