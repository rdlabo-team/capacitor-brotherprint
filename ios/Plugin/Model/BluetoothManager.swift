import CoreBluetooth
import Capacitor

class BluetoothManager: NSObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    private var centralManager: CBCentralManager!
    private var peripheralToConnect: CBPeripheral?
    
    var discoveredPeripherals: [CBPeripheral] = []
    var characteristicValues: [UUID: [String: String]] = [:]
    
    var onDeviceDiscovered: ((CBPeripheral) -> Void)?
    
    let prefixes = ["PJ-", "PT-", "QL-", "TD-", "RJ-", "TD-"]

    override init() {
        super.init()
        centralManager = CBCentralManager(delegate: self, queue: nil)
    }
    
    func getChannels() -> [JSObject] {
        var channels: [JSObject] = []
        
        for valueDictionary in characteristicValues.values {
            if (valueDictionary["manufacturerName"] == "Brother") {
                let channel: JSObject = [
                    "port": valueDictionary["port"] ?? "",
                    "modelName": valueDictionary["modelName"] ?? "",
                    "channelInfo": valueDictionary["channelInfo"] ?? "",
                ]
                
                NSLog(valueDictionary["modelName"] ?? "")
                channels.append(channel)
            }
        }
        
        return channels
    }

    // Bluetoothの状態が更新された時に呼ばれるメソッド
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        if central.state == .poweredOn {
            scanForDevices()
        } else {
            print("Bluetooth is not available.")
        }
    }

    // デバイスをスキャンするメソッド
    func scanForDevices() {
        centralManager.scanForPeripherals(withServices: nil, options: nil)
    }

    // デバイスを発見した時に呼ばれるメソッド
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        if let name = peripheral.name, prefixes.contains(where: { name.hasPrefix($0) }), !discoveredPeripherals.contains(where: { $0.identifier == peripheral.identifier }) {
            print("Discovered \(peripheral.name ?? "unknown device") at \(peripheral.identifier) / \(peripheral.state)")
            onDeviceDiscovered?(peripheral)
            peripheralToConnect = peripheral
            centralManager.connect(peripheral, options: nil)
        }
    }

    // デバイスに接続できた時に呼ばれるメソッド
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        peripheral.delegate = self
        peripheral.discoverServices(nil)
    }

    // 接続に失敗した時に呼ばれるメソッド
    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
        print("Failed to connect: \(error?.localizedDescription ?? "unknown error")")
    }

    // サービスを発見した時に呼ばれるメソッド
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        guard let services = peripheral.services else {
            if !discoveredPeripherals.contains(where: { $0.identifier == peripheral.identifier }) {
                discoveredPeripherals.append(peripheral)
            }
            return
        }
        
        // Device Information
        let deviceInformationUUID = CBUUID(string: "180A")
        
        if !services.contains(where: { $0.uuid == deviceInformationUUID }) {
            if !discoveredPeripherals.contains(where: { $0.identifier == peripheral.identifier }) {
                discoveredPeripherals.append(peripheral)
            }
            return
        }
        
        for service in services {
            if service.uuid == deviceInformationUUID {
                peripheral.discoverCharacteristics(nil, for: service)
            }
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        if error != nil {
            if !discoveredPeripherals.contains(where: { $0.identifier == peripheral.identifier }) {
                discoveredPeripherals.append(peripheral)
            }
            return
        }
        
        // 特性を取得
        if let characteristics = service.characteristics {
            for characteristic in characteristics {
                if characteristic.properties.contains(.read) {
                    peripheral.readValue(for: characteristic)
                }
            }
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        if let error = error {
            print("Error reading value: \(error.localizedDescription)")
            return
        }
        
        // 特性の値を処理する
        if let value = characteristic.value {
            
            let peripheralID = peripheral.identifier
            if characteristicValues[peripheralID] == nil {
                characteristicValues[peripheralID] = ["port": "bluetooth"]
            }
            
            if let stringValue = String(data: value, encoding: .utf8) {
                
                let characteristicNames: [CBUUID: String] = [
                    CBUUID(string: "2A24"): "modelName",
                    CBUUID(string: "2A25"): "channelInfo",
                    CBUUID(string: "2A29"): "manufacturerName"
                ]
                
                if characteristicNames.keys.contains(characteristic.uuid) {
                    let characteristicName = characteristicNames[characteristic.uuid] ?? "Unknown"
                    characteristicValues[peripheralID]?[characteristicName] = stringValue
                }
                
                NSLog("CBCharacteristic")
            }
        }
    }
}
