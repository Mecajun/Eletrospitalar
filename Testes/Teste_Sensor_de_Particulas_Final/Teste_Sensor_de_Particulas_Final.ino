/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * DESCRIPTION
 * 
 * Dust Sensor for SamYoung DSM501
 *   connect the sensor as follows :
 *        Pin 2 of dust sensor PM1      -> Digital 3 (PMW)
 *    Pin 3 of dust sensor          -> +5V 
 *    Pin 4 of dust sensor PM2.5    -> Digital 6 (PWM) 
 *    Pin 5 of dust sensor          -> Ground
 * Datasheet: http://www.samyoungsnc.com/products/3-1%20Specification%20DSM501.pdf
* Contributor: epierre
**/
  

#include <SPI.h>

#define DUST_SENSOR_DIGITAL_PIN_PM10  3
#define DUST_SENSOR_DIGITAL_PIN_PM25  6

unsigned long SLEEP_TIME = 30*1000; // Sleep time between reads (in milliseconds)
//VARIABLES
int val = 0;           // variable to store the value coming from the sensor
float valDUSTPM25 =0.0;
float lastDUSTPM25 =0.0;
float valDUSTPM10 =0.0;
float lastDUSTPM10 =0.0;
unsigned long duration;
unsigned long duration2;
unsigned long starttime;
unsigned long endtime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio = 0;
float ratio2 = 0;
long concentrationPM25 = 0;
long concentrationPM10 = 0;
long concent[2];
int temp=20; //external temperature, if you can replace this with a DHT11 or better 
float ppmv;

void setup()  
{
  Serial.begin(9600);
  pinMode(DUST_SENSOR_DIGITAL_PIN_PM10,INPUT);
  pinMode(DUST_SENSOR_DIGITAL_PIN_PM25,INPUT);
  
}

void loop()      
{    

  //get PM 2.5 density of particles over 2.5 μm.
  long *concentration = getPM(DUST_SENSOR_DIGITAL_PIN_PM25, DUST_SENSOR_DIGITAL_PIN_PM10);
  //concentrationPM10=getPM(DUST_SENSOR_DIGITAL_PIN_PM10);
  Serial.print("PM25: ");
  Serial.print(concentration[0]);
  Serial.print("mg/m3 ");
  //ppmv=mg/m3 * (0.08205*Tmp)/Molecular_mass
  //0.08205   = Universal gas constant in atm·m3/(kmol·K)
  ppmv=(float)(((concentration[0]*0.0283168)/100) *  ((0.08205*temp)/0.01))/1000;
  Serial.print("PM25: ");
  Serial.print(ppmv);
  Serial.print("ppmv "); 
  

 //get PM 1.0 - density of particles over 1 μm.
 if (concentration[1] > 0){
  Serial.print("PM10: ");
  Serial.print(concentration[1]);
  Serial.print("ug/m3 "); 
  //ppmv=mg/m3 * (0.08205*Tmp)/Molecular_mass
  //0.08205   = Universal gas constant in atm·m3/(kmol·K)
  ppmv=(((concentration[1]*0.0283168/100) *  (0.08205*temp)/0.01))/1000;
  Serial.print("PM10: ");
  Serial.print(ppmv);
  Serial.print("ppmv\n");
 }else{
  Serial.print("PM10: ");
  Serial.print(0.0);
  Serial.print("ug/m3 "); 
  Serial.print("PM10: ");
  Serial.print(0.0);
  Serial.print("ppmv\n");
  
  }
  
}


long *getPM(int DUST_SENSOR_DIGITAL_PIN, int DUST_SENSOR_DIGITAL_PIN2) {

  starttime = millis();

  while (1) {
  
    duration = pulseIn(DUST_SENSOR_DIGITAL_PIN, LOW);
    duration2 = pulseIn(DUST_SENSOR_DIGITAL_PIN2, LOW);
    lowpulseoccupancy += duration;
    lowpulseoccupancy2 += duration2;
    endtime = millis();
    
    if ((endtime-starttime) > sampletime_ms)
    {
    ratio = (lowpulseoccupancy-endtime+starttime)/(sampletime_ms*10.0);  // Integer percentage 0=>100
    ratio2 = (lowpulseoccupancy2-endtime+starttime)/(sampletime_ms*10.0);
    concent[0] = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    concent[1] = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62;
    
    lowpulseoccupancy = 0;
    lowpulseoccupancy2 = 0;
    return (concent);    
    }
  }  
}
