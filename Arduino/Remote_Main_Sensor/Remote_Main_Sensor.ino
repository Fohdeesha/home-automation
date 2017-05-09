
#include <Modbus.h>
#include <ModbusSerial.h>
#include <SoftwareSerial.h>

SoftwareSerial myserial(8, 9);

const int TEMP_HREG = 3; 
ModbusSerial mb;

float c;
float f;
int fint;
long ts;

// MAIN HOUSE SENSOR ABOVE STAIRS
void setup() {
    
  //mb.config (&Serial, 9600, SERIAL_8N1, 4);

    mb.config(&myserial, 9600, 4);
    mb.setSlaveId(5);  
    
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

       
   //delay(5);
   
}

void loopTempSense() 
{
   c = millis();
   f = c * 1.8 + 32 - 0.2;
   fint = f * 100;
}
