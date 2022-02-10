#include <Arduino_LSM9DS1.h>
float x, y, z;
int plusThreshold = 30, minusThreshold = -30;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Inicializado");
  if (!IMU.begin()) {
    Serial.println("Falha ao iniciar IMU");
    while (1);
  }
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
}
void loop() {
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
  }
  if(y > plusThreshold)
  {
    Serial.println("Colisao Frontal");
    delay(500);
  }
  if(y < minusThreshold)
  {
    Serial.println("Colisao Traseira");
    delay(500);
  }
  if(x < minusThreshold)
  {
    Serial.println("Colisao na direita");
    delay(500);
  }
    if(x > plusThreshold)
  {
    Serial.println("Colisao na esquerda");
    delay(500);
  }
}
