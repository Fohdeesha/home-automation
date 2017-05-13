#include <Modbus.h>
#include <ModbusSerial.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
SoftwareSerial myserial(8, 9);

// Set Modbus Variables Here
const int SlaveNum = 5;
const int Temp_Register = 1;
const int SrlSpeed = 9600;
char      Name[] = "Modbus High Accuracy Temp Sensor";
char      Location[] = "Above Upstairs Stairwell";
char      Version[] = "Firmware Build v1.1";


float c;
float f;
int fint;
long ts;
const int TEMP_HREG = Temp_Register - 1;
ModbusSerial mb;

void setup() {

  // Print Sensor Information over local USB for debugging 
  Serial.begin(9600);
  Serial.println("Initializing Local Serial Debug Output:");
  Serial.println(Name);
  Serial.println("Author: Fohdeesha");
  Serial.println(Version);
  Serial.print("Physical Location: "); Serial.println(Location);
  Serial.print("RS485 Bus Speed: "); Serial.print(SrlSpeed); Serial.println(" 8N1"); 
  Serial.print("Modbus Slave ID: "); Serial.println(SlaveNum);
  Serial.print("Modbus Temp Register: "); Serial.println(Temp_Register);


    tempsensor.begin();
    mb.config(&myserial, SrlSpeed, 4);
    mb.setSlaveId(SlaveNum);  
    
    mb.addHreg(TEMP_HREG);
    ts = millis();
  Serial.println("Initialization Complete");
  Serial.println(" ");
}

void loop() {

   mb.task();
   mb.Hreg(TEMP_HREG, fint);

   //Call Temp Loop every 1s
   if (millis() > ts + 1000) {   
       ts = millis();
       loopTempSense();
   }

   //delay(5);
   
}

void loopTempSense() 
{
   c = tempsensor.readTempC();
   f = c * 1.8 + 32;
   fint = f * 100;
   
  Serial.print("Temperature: "); Serial.println(f);
   
}
