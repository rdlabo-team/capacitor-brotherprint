Pod::Spec.new do |s|
  s.name             = 'BRLMPrinterKit'
  s.version          = '4.9.1'
  s.homepage         = 'https://www.brother.co.jp/product/dev/mobile/index.htm'
  s.source           = { :path => './Sources' }
  s.summary          = "Pod for the BRLMPrinterKit / Brother's printers"
  s.description      = "This project is only a Pod for the Brother SDK v#{s.version}"
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'Masahiko Sakakibara' => 'sakakibara@rdlabo.jp' }
  s.ios.deployment_target = '12.0'
  s.ios.vendored_frameworks = 'Sources/BRLMPrinterKit.xcframework'
  s.pod_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
  s.user_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
end
