#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "Adafruit_MCP9808.h"
#include <MemoryFree.h>

Adafruit_ADS1115 ads;
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x5E, 0x69 };
IPAddress ip(192, 168, 1, 21);
IPAddress server(192, 168, 1, 28);

int SoffitR;
int SoffitG;
int SoffitB;
int BLUE = 6;                   
int GREEN = 5; 
int RED = 3; 
int red1;
int green1;
int blue1;
long ts;
float PSU24v1;
float PSU24v2;
float PSU12v;
float PSU5v;
int AnalogIn1 = 0;
float LED24v;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
      if(String(topic) == "control/arduino_LED/livingroom/color") {
    // convert payload to String
    String value = String((char*)payload);
    //value.trim();
    // split string at every "," and store in proper variable
    // convert final result to integer
    SoffitR = value.substring(0,value.indexOf(',')).toInt();
    SoffitG = value.substring(value.indexOf(',')+1,value.lastIndexOf(',')).toInt();
    SoffitB = value.substring(value.lastIndexOf(',')+1).toInt();

  int red1 = SoffitR * 2.55;
  int green1 = SoffitG * 2.55;
  int blue1 = (SoffitB * 0.7) * 2.55;
  analogWrite(RED, red1);
  analogWrite(GREEN, green1);
  analogWrite(BLUE, blue1);



  
  // print obtained values for debugging
Serial.println();
Serial.print("RED: ");
Serial.println(red1);
//client.publish("status/arduino_LED", SoffitR);

Serial.print("GREEN: ");
Serial.println(green1);
//client.publish("status/arduino_LED", SoffitG);

Serial.print("BLUE: ");
Serial.println(blue1);
//client.publish("status/arduino_LED", int SoffitB);
Serial.println();
//Serial.flush();

  }

}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduino_LED")) {
      Serial.println("connected");
     client.publish("status/arduino_LED","Arduino LED is now online");     
     client.subscribe("control/arduino_LED/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  ads.setGain(GAIN_TWOTHIRDS);
  ads.begin();
  tempsensor.begin();

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  ts = millis();
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(50);

   //Call ADC/data parsing loop every 2s
   if (millis() > ts + 2000) {   
       ts = millis();
       loopADC();
   } 
}

void loopADC() 
{
  int16_t adc1, adc2, adc3, adc4;
  int bytes = freeMemory();

  adc1 = ads.readADC_SingleEnded(0);
  adc2 = ads.readADC_SingleEnded(1);
  adc3 = ads.readADC_SingleEnded(2);
  adc4 = ads.readADC_SingleEnded(3);
  AnalogIn1 = analogRead(A0);

// scale ADC output with calibrated voltage divider values and offset
  PSU24v1 = (adc1 + 16) / 1065.200;
  PSU24v2 = (adc2 + 16) / 1065.540;
  PSU12v = (adc3 + 16) /  1067.542;
  PSU5v = (adc4 + 16) /   1064.542;
  LED24v = AnalogIn1 /    40.840;


  float c = tempsensor.readTempC();
  float temp = c * 9.0 / 5.0 + 32;
  
  Serial.print("24v Supply #1: "); Serial.println(PSU24v1);
  Serial.print("24v Supply #2: "); Serial.println(PSU24v2);
  Serial.print("12v Rail: "); Serial.println(PSU12v);
  Serial.print("5v Rail: "); Serial.println(PSU5v);
  Serial.print("LED 24v Supply: "); Serial.println(LED24v);
  Serial.print("Temperature: "); Serial.println(temp);
  Serial.print("Free Memory Bytes: "); Serial.println(bytes);

  Serial.println(" ");
  
    char Power1[10];
  dtostrf(PSU24v1,4,2,Power1);
  client.publish("/read/arduino_LED/PSU1",Power1);

    char Power2[10];
  dtostrf(PSU24v2,4,2,Power2);
  client.publish("/read/arduino_LED/PSU2",Power2);

    char Power3[10];
  dtostrf(PSU12v,4,2,Power3);
  client.publish("/read/arduino_LED/PSU3",Power3);

    char Power4[10];
  dtostrf(PSU5v,4,2,Power4);
  client.publish("/read/arduino_LED/PSU4",Power4);

    char Power5[10];
  dtostrf(LED24v,4,2,Power5);
  client.publish("/read/arduino_LED/PSU5",Power5);

    char tempchar[10];
  dtostrf(temp,4,2,tempchar);
  client.publish("/read/arduino_LED/temp",tempchar);


    char memorychar[10];
  itoa(bytes,memorychar,10);
  client.publish("/read/arduino_LED/memory",memorychar);
}
