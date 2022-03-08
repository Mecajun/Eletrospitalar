#include <Arduino_LSM9DS1.h>
#include "MadgwickAHRS.h"

// inicializa o filtro do Madgwick:
Madgwick filter;
// o sample rate do sensor é de 104 Hz:
const float sensorRate = 104.00;
float oldRoll, oldPitch, oldHeading;
float *pRoll = NULL;

void setup() {
  Serial.begin(9600);
  // tenta iniciar o IMU
  if (!IMU.begin()) {
    Serial.println("Falha na inicializacao do IMU");
    
    // para o código no caso de falha na inicialização do IMU
    while (true);
  }
  // começa o filtro para rodar na sample rate
  filter.begin(sensorRate);
  
}

void loop() {
  // valores para a aceleração (Acc) e rotação (Gyro)
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;

  // valores de orientacao e comparacao:
  float roll, pitch, heading;
  
  // checa a capacidade de leitura do IMU
  if (IMU.accelerationAvailable() &&
      IMU.gyroscopeAvailable()) {
    // lê e atribui os valores de aceleração e rotação:
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro);

    // atualiza o filtro, o qual computa a orientação
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);

    // define o direcionamento axial (heading), inclinação frontal (pitch) e lateral (roll).
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    
    if (pRoll == NULL){
      pRoll = &oldRoll;
    }
    else{
      // procura detectar movimento, tentando "ignorar" a compensação do próprio módulo
      // funciona comparando o roll com o oldRoll, por exemplo, caso a variação seja maior do que a minima definida (no caso, 3 graus), dá alerta. O menor de 89 serve para o caso de uma virada completa, de 90 para 0, por exemplo.
      if (abs(roll - oldRoll) > 3 and abs(roll - oldRoll) < 89){
        Serial.println("Movimento detectado!");
      }
      if (abs(pitch - oldPitch) > 3 and abs(roll - oldPitch) < 89){
        Serial.println("Movimento detectado!");
      }
      if (abs(heading - oldHeading) > 3 and abs(heading - oldHeading) < 359){
        Serial.println("Movimento detectado!");
      }
    }
    // atribui a primeira leitura ao valor "old"
    oldRoll = roll;
    oldPitch = pitch;
    oldHeading = heading;
    // Caso queira printar os valores tambem
    //Serial.print("Orientacao: ");
    //Serial.print(heading);
    //Serial.print(" ");
    //Serial.print(pitch);
    //Serial.print(" ");
    //Serial.println(roll);
    delay(500)
  }
}
