#include <ArduinoBLE.h>
double measured_power = -69, N = 2; // valores para ajuste de RSSI
float Lista[60];
float Lista_rssi[60];
long tempoAnterior = 0;
int i;
#define BLE_UUID_data "180C" 
void setup() {
  Serial.begin(9600);
  while (!Serial);
  if(!BLE.begin()) { 
    Serial.println("Sem BLE ligado");
    while(1);
  }
  // seta o nome do BLE
  BLE.setLocalName("Nano33BLECentral");
  Serial.println("BLE Central scan");
  //começa a verificar se tem outro ble por perto
  BLE.scan();
}
void loop() {
  BLEDevice peripheral = BLE.available(); // verifica se achou algum ble disponível
  
  float dist_rssi = peripheral.rssi(); // valor em rssi para uma variável
  if(peripheral) {
    Serial.println("doidera");
    Serial.println(peripheral.address()); // endereço do dispositivo
    if(peripheral.hasLocalName()) {
      Serial.println(peripheral.localName());
      //Serial.println("doidera");

    }
    // pegar rssi somente daqueles com nome cadastrado 
    if (peripheral.localName() == "Nano33BLECentral") {
      Serial.print("RSSI: ");
      Serial.println(dist_rssi);
      
      // transformando o valor do sinal em metros
      double distance = pow(10,((measured_power-peripheral.rssi())/(10*N)));
      Serial.print("distância em m: ");
      Serial.println(distance);
      
      // definindo um limite de sinal para área de interesse
      if (dist_rssi > (-68)) {
        Serial.println("dispositivo dentro da área de interesse");
        Serial.println(peripheral.localName());

        //definir um tempo de intervalo para coleta da distância
        if (millis() >= tempoAnterior+1000){
          tempoAnterior = millis();
          // distância em metros
          Lista[i] = distance;
          //distância em rssi (apenas para teste caso a formula de
          //transformação esteja com problema
          Lista_rssi[i] = dist_rssi;
          Serial.print("\n");
          Serial.println(Lista_rssi[i]);
          i++;
          if (i == 60){
            for (i=60; i>0; i--){
              Serial.print(Lista_rssi[i]);
              Serial.print('\t');
            }
            i = 0;
            }
          }
        }
      }
      //reseta o índice a cada 1min para envir as listas
  }
} 
