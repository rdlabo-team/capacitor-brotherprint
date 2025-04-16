Pod::Spec.new do |s|
  s.name             = 'BRLMPrinterKit'
  s.version          = '4.12.0'
  s.homepage         = 'https://support.brother.co.jp/j/s/support/html/mobilesdk/index.html'
  s.source           = { :path => './Sources' }
  s.summary          = "Pod for the BRLMPrinterKit / Brother's printers"
  s.description      = "This project is only a Pod for the Brother SDK v#{s.version}"
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'Masahiko Sakakibara' => 'sakakibara@rdlabo.jp' }
  s.ios.deployment_target = '14.0'
  s.swift_version = '5.1'
  s.ios.vendored_frameworks = 'Sources/BRLMPrinterKit.xcframework'
  s.pod_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
  s.user_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
end
