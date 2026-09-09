import { readFileSync, writeFileSync } from 'node:fs';
import { format } from 'prettier';
const models = JSON.parse(readFileSync('printer-models.json', 'utf8'));
const header = '// Generated from printer-models.json. Run npm run generate:models.\n';
const outputs = {
  'src/models.ts': await format(
    header +
      `import { BRLMPrinterModelName as Model, BRLMPrinterPort as Port } from './brother-printer.enum';

export const printerPorts: Record<Model, readonly Port[]> = {${models.map((m) => `[Model.${m.name}]: [${m.ports.map((p) => `Port.${p}`).join(',')}]`).join(',')}};
export const printerAliases: Record<string, Model> = {${models.flatMap((m) => m.aliases.map((a) => `${a}: Model.${m.name}`)).join(',')}};`,
    { parser: 'typescript', singleQuote: true, printWidth: 120 },
  ),
  'android/src/main/java/jp/rdlabo/capacitor/plugin/brotherprint/PrinterModels.kt':
    header +
    `package jp.rdlabo.capacitor.plugin.brotherprint

internal val printerModels = mapOf(
${models.map((m) => `    "${m.name}" to "${m.android}"`).join(',\n')}
)
`,
  'ios/Sources/BrotherPrintPlugin/PrinterModelCatalog.swift':
    header +
    `import Foundation

let printerModelNames: Set<String> = [${models
      .filter((m) => m.ios)
      .map((m) => `"${m.name}"`)
      .join(', ')}]
let printerSearchModels: [String] = [${models
      .filter((m) => m.ios && m.ports.includes('wifi'))
      .map((m) => `"Brother ${m.product}"`)
      .join(', ')}]
`,
  'ios/Sources/BrotherPrintPlugin/Model/PrinterModelMapping.swift':
    header +
    `import BRLMPrinterKit

func nativePrinterModel(_ name: String) -> BRLMPrinterModel {
    switch name {
${models
  .filter((m) => m.ios)
  .map((m) => `    case "${m.name}": return .${m.ios}`)
  .join('\n')}
    default: return .unknown
    }
}
`,
};
for (const [file, text] of Object.entries(outputs)) {
  if (process.argv.includes('--check')) {
    if (readFileSync(file, 'utf8') !== text) throw new Error(`Stale generated model mapping: ${file}`);
  } else writeFileSync(file, text);
}
