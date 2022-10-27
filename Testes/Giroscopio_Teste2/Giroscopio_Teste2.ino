#include <Arduino_LSM9DS1.h>
#include "MadgwickAHRS.h"

// inicializa o filtro do Madgwick:
Madgwick filter;
// o sample rate do sensor é de 104 Hz:
const float sensorRate = 104.00;
float xoldAcc, yoldAcc, zoldAcc, xVel, yVel, zVel, xoldVel, yoldVel, zoldVel;
float xDist, yDist, zDist, Dist, t;
float Lista[60];
float *pRoll = NULL;
int i;

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
  // valores para a aceleração (Acc) 
  float xAcc, yAcc, zAcc;
  
  // checa a capacidade de leitura do IMU
  if (IMU.accelerationAvailable()) {
    // lê e atribui os valores de aceleração e rotação:
    IMU.readAcceleration(xAcc, yAcc, zAcc);

    // atualiza o filtro, o qual computa a orientação
    filter.updateIMU(xAcc, yAcc, zAcc, 0, 0, 0);

    // conversão da aceleração de g para m/s2
    xAcc = xAcc*9.80665;
    yAcc = yAcc*9.80665;
    zAcc = zAcc*9.80665;

    // tempo a cada verificação
    t = 1;
    
    // calculo do movimento em cada eixo usando a regra dos trapezios
    xVel = (xoldVel + (xoldAcc + xAcc))*t/2;
    xDist = (xDist + (xoldVel + xVel))*t/2;
    
    yVel = (yoldVel + (yoldAcc + yAcc))*t/2;
    yDist = (yDist + (yoldVel + yVel))*t/2;
    
    zVel = (zoldVel + (zoldAcc + zAcc))*t/2;
    zDist = (zDist + (zoldVel + zVel))*t/2;

    Dist = (sqrt(pow(xDist, 2) + pow(yDist, 2) + pow(zDist, 2)))*100 + Dist;
    Lista[i] = Dist;
    if (Dist >= 10){
      Serial.print("Movimento detectado: ");
      Serial.print(Dist);
      Serial.print(" cm");
      Serial.print('\n');      
    }
    Serial.print(xAcc);
    Serial.print('\t');
    Serial.print(yAcc);
    Serial.print('\t');
    Serial.println(zAcc);
    Serial.print('\n'); 

    xoldAcc = xAcc;
    yoldAcc = yAcc;
    zoldAcc = zAcc;
    xoldVel = xVel;
    yoldVel = yVel;
    zoldVel = zVel;
    i++;
    
    // reseta o indice e envia a lista
    if (i == 60){
      for (i=60; i>0; i--){
        Serial.print(Lista[i]);
        Serial.print('\t');
        }
      i = 0;
    }
    
    delay(1000);
   }
   // BLE parado, zera as distancias
   Dist = 0.0;
   xDist = 0.0;
   yDist = 0.0;
   zDist = 0.0;
   Lista[i] = Dist;
   i++;
   delay(1000);

   // resetar o indice e enviar a lista
   if (i == 60){
    for (i=60; i>0; i--){
        Serial.print(Lista[i]);
        Serial.print('\t');
        }
     i = 0;
   }
   
  }
