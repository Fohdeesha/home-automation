
#include <Modbus.h>
#include <ModbusSerial.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

#define ONE_WIRE_BUS 2 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
SoftwareSerial myserial(10, 11);

const int TEMP_HREG = 3; 
ModbusSerial mb;

float c;
float f;
int fint;
long ts;


void setup() {
    // Config Modbus Serial (port, speed, byte format) 
  mb.config(&myserial, 19200, 4);

    mb.setSlaveId(5);  

    // Add SENSOR_IREG register - Use addIreg() for analog Inputs
    mb.addHreg(TEMP_HREG);
    ts = millis();
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();
   mb.Hreg(TEMP_HREG, fint);

   //Call Temp Loop every 1s
   if (millis() > ts + 1000) {   
       ts = millis();
       loopTempSense();
   }

       
   delay(50);
   
}

void loopTempSense() 
{
   sensors.requestTemperatures();
   c = sensors.getTempCByIndex(0);
   f = c * 1.8 + 32 - 0.2;
   fint = f * 100;
}
