import { execFileSync } from 'node:child_process';
import { cpSync, mkdirSync, mkdtempSync, readFileSync, writeFileSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { join, resolve } from 'node:path';
import assert from 'node:assert/strict';

// Stage exactly npm's file list. Production path rewriting never touches the checkout.
const root = process.cwd();
const staging = mkdtempSync(join(tmpdir(), 'brother-package-'));
const packageDir = join(staging, 'package');
mkdirSync(packageDir);
const listing = JSON.parse(
  execFileSync('npm', ['pack', '--dry-run', '--ignore-scripts', '--json'], { encoding: 'utf8' }),
)[0];
for (const { path } of listing.files) {
  assert(!/\.aar$|\.xcframework\/|\.framework\/|\.apk$/.test(path), `Proprietary SDK/build artifact included: ${path}`);
  const target = join(packageDir, path);
  mkdirSync(resolve(target, '..'), { recursive: true });
  cpSync(join(root, path), target);
}
const paths = [
  ['Package.swift', './demo/ios/LocalPackages/BRLMPrinterKit', '../../../ios/LocalPackages/BRLMPrinterKit'],
  [
    'ios/Sources/BrotherPrintPlugin/module.modulemap',
    '../../../demo/ios/LocalPackages/BRLMPrinterKit',
    '../../../../../../ios/LocalPackages/BRLMPrinterKit',
  ],
];
for (const [file, development, production] of paths) {
  const path = join(packageDir, file);
  const content = readFileSync(path, 'utf8').replaceAll(development, production);
  assert(content.includes(production) && !content.includes('/demo/'), `${file}: invalid production path`);
  writeFileSync(path, content);
}
// Both Node entry points must load, and TypeScript must resolve the packaged public API.
const consumer = join(staging, 'consumer');
const installed = join(consumer, 'node_modules/@rdlabo/capacitor-brotherprint');
mkdirSync(resolve(installed, '..'), { recursive: true });
cpSync(packageDir, installed, { recursive: true });
cpSync(join(root, 'node_modules/@capacitor/core'), join(consumer, 'node_modules/@capacitor/core'), { recursive: true });
const legacyPaths = ['dist/esm/brother-printer.enum', 'dist/esm/brother-printer.enum.js', 'dist/plugin.cjs.js'];
execFileSync(
  process.execPath,
  [
    '-e',
    `for (const path of ${JSON.stringify(legacyPaths)}) require.resolve('@rdlabo/capacitor-brotherprint/' + path);`,
  ],
  { cwd: consumer, stdio: 'inherit' },
);
for (const args of [
  ['-e', "const p=require('@rdlabo/capacitor-brotherprint'); if(typeof p.BrotherPrinter!=='function') process.exit(1)"],
  [
    '--input-type=module',
    '-e',
    "import {BrotherPrinter} from '@rdlabo/capacitor-brotherprint'; if(typeof BrotherPrinter!=='function') process.exit(1)",
  ],
])
  execFileSync(process.execPath, args, { cwd: consumer, stdio: 'pipe' });
const example = readFileSync(join(root, 'examples/plain-typescript.ts'), 'utf8').replace(
  "'../src'",
  "'@rdlabo/capacitor-brotherprint'",
);
writeFileSync(join(consumer, 'index.ts'), example);
execFileSync(
  join(root, 'node_modules/.bin/tsc'),
  [
    '--noEmit',
    '--strict',
    '--skipLibCheck',
    '--target',
    'es2022',
    '--module',
    'nodenext',
    '--moduleResolution',
    'nodenext',
    join(consumer, 'index.ts'),
  ],
  { stdio: 'inherit' },
);
const destination = resolve(process.env.BROTHER_PACK_DESTINATION ?? join(root, 'dist'));
mkdirSync(destination, { recursive: true });
const result = JSON.parse(
  execFileSync('npm', ['pack', '--ignore-scripts', '--json', '--pack-destination', destination], {
    cwd: packageDir,
    encoding: 'utf8',
  }),
)[0];
console.log(`Verified production package: ${join(destination, result.filename)}`);
