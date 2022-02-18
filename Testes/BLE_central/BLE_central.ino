#include <ArduinoBLE.h>

#define measured_power -69
#define N 2

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  Serial.println("BLE Central scan");

  // start scanning for peripheral
  BLE.scan();
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral
    Serial.println("Discovered a peripheral");
    Serial.println("-----------------------");
    
    // print address
    Serial.print("Address: ");
    Serial.println(peripheral.address());
    
    // print the local name, if present
    if (peripheral.hasLocalName()) {
      Serial.print("Local Name: ");
      Serial.println(peripheral.localName());
    }
    
    if (peripheral.localName() == "Nano33BLE"){
      
      peripheral.connect();
      Serial.println("Nano33BLE conectado");
      
      while(peripheral.connect()){
        
        // print the advertised service UUIDs, if present
        if (peripheral.hasAdvertisedServiceUuid()) {
          Serial.print("Service UUIDs: ");
            // loop the services of the peripheral and explore each
          for (int i = 0; i < peripheral.serviceCount(); i++) {
            BLEService service = peripheral.service(i);
        
            for (int i = 0; i < service.characteristicCount(); i++) {
              BLECharacteristic characteristic = service.characteristic(i);
          
              if (characteristic.valueLength() > 0) {
                // print out the value of the characteristic
                Serial.print(", value 0x");
                printData(characteristic.value(), characteristic.valueLength());
              }
            }
           }
          Serial.println();
        }
        
        // print the RSSI
        Serial.print("RSSI: ");
        Serial.println(peripheral.rssi());
    
        // print the distance
        Serial.print("Distance: ");
        float distance = pow(10,((measured_power-peripheral.rssi())/(10*N)));
        Serial.print(distance);
        Serial.println(" m");
        Serial.println(" ");
      }
      Serial.println("Nano33BLE desconectado");
    }
  }
}
