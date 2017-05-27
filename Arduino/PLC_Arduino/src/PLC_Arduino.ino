#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <MemoryFree.h>

Adafruit_ADS1115 ads;

byte mac[] = { 0xDE, 0xA2, 0xDA, 0x0F, 0x5E, 0x69 };
IPAddress ip(192, 168, 1, 21);
IPAddress server(192, 168, 1, 28);

int redraw;
int greenraw;
int blueraw;
int redpin = 3;
int greenpin = 5;
int bluepin = 6;
int red;
int green;
int blue;
long ts;
float PSU24v1;
float PSU24v2;
float PSU12v;
float PSU5v;
float LED24v;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
      if(String(topic) == "/control/Arduino_PLC/color") {
    // convert payload to String
    String value = String((char*)payload);
    //value.trim();
    // split string at every "," and store in proper variable
    // convert final result to integer
    redraw = value.substring(0,value.indexOf(',')).toInt();
    greenraw = value.substring(value.indexOf(',')+1,value.lastIndexOf(',')).toInt();
    blueraw = value.substring(value.lastIndexOf(',')+1).toInt();

  int red = redraw * 2.55;
  int green = greenraw * 2.55;
  int blue = (blueraw * 0.6) * 2.55;
  analogWrite(redpin, red);
  analogWrite(greenpin, green);
  analogWrite(bluepin, blue);




  // print obtained values for debugging
Serial.println();
Serial.print("RED: ");
Serial.println(red);

Serial.print("GREEN: ");
Serial.println(green);

Serial.print("BLUE: ");
Serial.println(blue);

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
    if (client.connect("Arduino_PLC")) {
      Serial.println("connected");
     client.publish("/status/Arduino_PLC","Arduino PLC is now online");
     client.subscribe("/control/Arduino_PLC/#");
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

// Change pin 3 from Phase Correct PWM to Fast PWM to match Pin 5 & 6 PWM frequency (976hz)
TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
TCCR2B = _BV(CS22);

  ads.setGain(GAIN_TWOTHIRDS);
  ads.begin();

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
  int adc1, adc2, adc3, adc4, adc5;
  int bytes = freeMemory();

  adc1 = ads.readADC_SingleEnded(0);
  adc2 = ads.readADC_SingleEnded(1);
  adc3 = ads.readADC_SingleEnded(2);
  adc4 = ads.readADC_SingleEnded(3);
  adc5 = analogRead(A0);

// scale ADC output with calibrated voltage divider values and offset
  PSU24v1 = (adc1 + 16) / 1065.200;
  PSU24v2 = (adc2 + 16) / 1065.540;
  PSU12v =  (adc3 + 16) / 1067.542;
  PSU5v =   (adc4 + 16) / 1064.542;
  LED24v =   adc5       /   40.840;



  Serial.print("24v Supply #1: "); Serial.println(PSU24v1);
  Serial.print("24v Supply #2: "); Serial.println(PSU24v2);
  Serial.print("12v Rail: "); Serial.println(PSU12v);
  Serial.print("5v Rail: "); Serial.println(PSU5v);
  Serial.print("LED 24v Supply: "); Serial.println(LED24v);
  Serial.print("Free Memory Bytes: "); Serial.println(bytes);

  Serial.println(" ");

    char Power1[10];
  dtostrf(PSU24v1,4,2,Power1);
  client.publish("/read/Arduino_PLC/PSU1",Power1);

    char Power2[10];
  dtostrf(PSU24v2,4,2,Power2);
  client.publish("/read/Arduino_PLC/PSU2",Power2);

    char Power3[10];
  dtostrf(PSU12v,4,2,Power3);
  client.publish("/read/Arduino_PLC/PSU3",Power3);

    char Power4[10];
  dtostrf(PSU5v,4,2,Power4);
  client.publish("/read/Arduino_PLC/PSU4",Power4);

    char Power5[10];
  dtostrf(LED24v,4,2,Power5);
  client.publish("/read/Arduino_PLC/PSU5",Power5);

    char memorychar[10];
  itoa(bytes,memorychar,10);
  client.publish("/read/Arduino_PLC/memory",memorychar);
}
