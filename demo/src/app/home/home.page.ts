import {
  Component,
  inject,
  model,
  OnDestroy,
  OnInit,
  signal,
} from '@angular/core';
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
  IonRadioGroup,
  IonRadio,
  IonSelect,
  IonSelectOption,
  IonInput,
  Platform,
} from '@ionic/angular/standalone';
import {
  BRKMPrinterCustomPaperType,
  BRKMPrinterCustomPaperUnit,
  BRKMPrinterPort,
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
  BRLMPrinterScaleMode,
  BRLMPrinterVerticalAlignment,
  ErrorInfo,
} from '../../../../src';
import { FormsModule } from '@angular/forms';
import { printData } from '../print-data';
import { setPlatformOptions } from 'ionicons/components';

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
    IonRadioGroup,
    IonRadio,
    FormsModule,
    IonSelect,
    IonSelectOption,
    IonInput,
  ],
})
export class HomePage implements OnInit, OnDestroy {
  readonly listenerHandlers: PluginListenerHandle[] = [];
  readonly printerPortEnum = BRKMPrinterPort;
  readonly modelNames = Object.values(BRLMPrinterModelName);
  readonly labelNames = Object.keys(BRLMPrinterLabelName);
  readonly paperTypes = Object.values(BRKMPrinterCustomPaperType);
  readonly paperUnits = Object.values(BRKMPrinterCustomPaperUnit);

  readonly useModel = model<BRLMPrinterModelName>(
    BRLMPrinterModelName.TD_2350D_300,
  );
  readonly useLabel = model<BRLMPrinterLabelName>(
    BRLMPrinterLabelName.RollW62RB,
  );

  readonly paperType = model<BRKMPrinterCustomPaperType>(
    BRKMPrinterCustomPaperType.dieCutPaper,
  );
  readonly paperUnit = model<BRKMPrinterCustomPaperUnit>(
    BRKMPrinterCustomPaperUnit.mm,
  );
  readonly tapeWidth = model<number>(60.0);
  readonly tapeLength = model<number>(60.0);
  readonly marginTop = model<number>(1.0);
  readonly marginRight = model<number>(2.0);
  readonly marginBottom = model<number>(1.0);
  readonly marginLeft = model<number>(2.0);
  readonly paperMarkPosition = model<number>(0);
  readonly paperMarkLength = model<number>(0);
  readonly gapLength = model<number>(2.0);

  readonly printers = signal<BRLMChannelResult[]>([]);
  readonly base64: string = printData();
  readonly platform = inject(Platform);

  async ngOnInit() {
    this.listenerHandlers.push(
      await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrint, () => {
        console.log('onPrint');
      }),
    );
    this.listenerHandlers.push(
      await BrotherPrint.addListener(
        BrotherPrintEventsEnum.onPrintError,
        () => {
          console.log('onPrintError');
        },
      ),
    );
    this.listenerHandlers.push(
      await BrotherPrint.addListener(
        BrotherPrintEventsEnum.onPrintFailedCommunication,
        (info: ErrorInfo) => {
          console.log('onPrintFailedCommunication');
        },
      ),
    );
    this.listenerHandlers.push(
      await BrotherPrint.addListener(
        BrotherPrintEventsEnum.onPrinterAvailable,
        (printer: BRLMChannelResult) => {
          this.printers.update(prev => [...prev, printer]);
        },
      ),
    );
  }

  async ngOnDestroy() {
    this.listenerHandlers.forEach(handler => handler.remove());
  }

  async searchPrinter(port: BRKMPrinterPort) {
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
      ...{
        modelName: this.useModel(),
        encodedImage: this.base64.slice(this.base64.indexOf(',') + 1),
        numberOfCopies: 1, // default 1
        autoCut: true, // default true

        scaleMode: BRLMPrinterScaleMode.FitPageAspect,
        imageRotation: BRLMPrinterImageRotation.Rotate90,
        verticalAlignment: BRLMPrinterVerticalAlignment.Center,
        horizontalAlignment: BRLMPrinterHorizontalAlignment.Center,
        printQuality: BRLMPrinterPrintQuality.Best,
      },
      ...{
        labelName: this.useLabel(),
      },
      ...{
        paperType: this.paperType(),
        paperUnit: this.paperUnit(),
        tapeWidth: this.tapeWidth(),
        tapeLength: this.tapeLength(),
        gapLength: this.gapLength(),

        marginTop: this.marginTop(),
        marginRight: this.marginRight(),
        marginBottom: this.marginBottom(),
        marginLeft: this.marginLeft(),

        paperMarkPosition: this.paperMarkPosition(),
        paperMarkLength: this.paperMarkLength(),
      },
    };

    console.log(this.paperType());

    BrotherPrint.printImage({
      ...defaultPrintSettings,
      ...{
        channelInfo: channel.channelInfo,
        port: channel.port,
      },
    });
  }

  protected readonly setPlatformOptions = setPlatformOptions;
}
