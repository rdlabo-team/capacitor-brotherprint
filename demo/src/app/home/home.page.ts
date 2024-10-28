import { Component, OnDestroy, OnInit, signal } from '@angular/core';
import {
  IonHeader,
  IonToolbar,
  IonTitle,
  IonContent,
  IonList,
  IonItem,
  IonItemGroup,
  IonLabel,
  IonText,
} from '@ionic/angular/standalone';
import {
  BRLMChannelResult,
  BRLMPrinterLabelName,
  BRLMPrinterModelName,
  BRLMPrintOptions,
  BrotherPrint,
  BrotherPrintEventsEnum,
} from '@rdlabo/capacitor-brotherprint';
import { PluginListenerHandle } from '@capacitor/core';
import {
  BRLMPrinterHorizontalAlignment,
  BRLMPrinterImageRotation,
  BRLMPrinterPrintQuality,
  BRLMPrinterPrintResolution,
  BRLMPrinterVerticalAlignment,
} from '../../../../src';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
  standalone: true,
  imports: [
    IonHeader,
    IonToolbar,
    IonTitle,
    IonContent,
    IonList,
    IonItem,
    IonItemGroup,
    IonLabel,
    IonText,
  ],
})
export class HomePage implements OnInit, OnDestroy {
  readonly listenerHandlers: PluginListenerHandle[] = [];
  printers = signal<BRLMChannelResult[]>([]);
  readonly base64 =
    'data:image/png;base64,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';

  async ngOnInit() {
    this.listenerHandlers.push(
      await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrint, () => {
        console.log('onPrint');
      }),
    );
    this.listenerHandlers.push(
      await BrotherPrint.addListener(
        BrotherPrintEventsEnum.onPrintError,
        info => {
          console.log('onPrintError');
        },
      ),
    );
    this.listenerHandlers.push(
      await BrotherPrint.addListener(
        BrotherPrintEventsEnum.onPrintFailedCommunication,
        info => {
          console.log('onPrintFailedCommunication');
        },
      ),
    );
    this.listenerHandlers.push(
      await BrotherPrint.addListener(
        BrotherPrintEventsEnum.onPrinterAvailable,
        printer => {
          this.printers.update(prev => [...prev, printer]);
        },
      ),
    );
  }

  async ngOnDestroy() {
    this.listenerHandlers.forEach(handler => handler.remove());
  }

  async searchPrinter(port: 'wifi' | 'bluetooth' | 'bluetoothLowEnergy') {
    // This method return void. Get the printer list by listening to the event.
    await BrotherPrint.search({
      port,
      searchDuration: 15, // seconds
    });
  }

  print(channel: BRLMChannelResult) {
    if (this.printers().length === 0) {
      alert('No printer found');
      return;
    }

    const result = confirm('Do you want to print?');
    if (!result) {
      return;
    }

    const defaultPrintSettings: BRLMPrintOptions = {
      modelName: BRLMPrinterModelName.QL_820NWB,
      labelName: BRLMPrinterLabelName.W29H90,
      encodedImage: this.base64.slice(this.base64.indexOf(',') + 1),
      numberOfCopies: 1, // default 1
      autoCut: true, // default true

      imageRotation: BRLMPrinterImageRotation.Rotate90,
      verticalAlignment: BRLMPrinterVerticalAlignment.Center,
      horizontalAlignment: BRLMPrinterHorizontalAlignment.Center,
      printQuality: BRLMPrinterPrintQuality.Best,
      resolution: BRLMPrinterPrintResolution.High,
    };

    BrotherPrint.printImage({
      ...defaultPrintSettings,
      ...{
        ipAddress: channel.ipAddress,
        localName: channel.nodeName,
        macAddress: channel.macAddress,
        serialNumber: channel.serialNumber,
        port: channel.port,
      },
    });
  }
}
