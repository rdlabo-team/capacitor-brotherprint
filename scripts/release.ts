import { readFileSync, writeFileSync } from 'fs';

const environment = ['development', 'production'];
const modulePath = './ios/Sources/BrotherPrintPlugin/module.modulemap' as const;
const packagePath = './Package.swift' as const;

type Env = (typeof environment)[number];

const headerPath: Record<Env, string> = {
  development:
    '../../../demo/ios/LocalPackages/BRLMPrinterKit/Sources/BRLMPrinterKit.xcframework/ios-arm64/BRLMPrinterKit.framework',
  production:
    '../../../../../../ios/LocalPackages/BRLMPrinterKit/Sources/BRLMPrinterKit.xcframework/ios-arm64/BRLMPrinterKit.framework',
};

const packagePathMap: Record<Env, string> = {
  development: './demo/ios/LocalPackages/BRLMPrinterKit',
  production: '../../../ios/LocalPackages/BRLMPrinterKit',
};

if (!environment.includes((process.argv as string[])[process.argv.length - 1])) {
  throw new Error('argv must be "development" or "production"');
}

const currentEnv = process.argv[process.argv.length - 1] as Env;
const oppositeEnv: Env = currentEnv === 'production' ? 'development' : 'production';

(() => {
  const content = readFileSync(modulePath, 'utf-8');
  const updated = content.split(headerPath[oppositeEnv]).join(headerPath[currentEnv]);
  writeFileSync(modulePath, updated);
})();

(() => {
  const content = readFileSync(packagePath, 'utf-8');
  const updated = content.split(packagePathMap[oppositeEnv]).join(packagePathMap[currentEnv]);
  writeFileSync(packagePath, updated);
})();
