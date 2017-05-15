#include <Arduino.h>


#include <Modbus.h>
#include <ModbusSerial.h>
#include <SoftwareSerial.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2 //define data pin for DS18B20
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
SoftwareSerial myserial(8, 9);

// Set All Variables Here Prease
const int SlaveNum = 6;
const int Temp_Register = 1;
const int SrlSpeed = 9600;
char      Name[] = "Modbus DS18B20 Temp Sensor";
char      Location[] = "Laundry Room Water Heater";
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
   sensors.requestTemperatures();
   c = sensors.getTempCByIndex(0);
   f = c * 1.8 + 32;
   fint = f * 100;

  Serial.print("Temperature: "); Serial.println(f);

}
