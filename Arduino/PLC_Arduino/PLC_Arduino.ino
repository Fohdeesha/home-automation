#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;

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
int PSU24v2;
int PSU12v;
int PSU5v;

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

   if (millis() > ts + 3000) {   
       ts = millis();
       //Call ADC loop every 3s
       loopADC();
   } 
}

void loopADC() 
{
  int16_t adc1, adc2, adc3, adc4;

  adc1 = ads.readADC_SingleEnded(0);
  adc2 = ads.readADC_SingleEnded(1);
  adc3 = ads.readADC_SingleEnded(2);
  adc4 = ads.readADC_SingleEnded(3);

// scale ADC output with calibrated voltage divider values and offset
  PSU24v1 = (adc1 + 16) / 10.654;
  PSU24v2 = (adc2 + 16) / 10.654;
  PSU12v = (adc3 + 16) / 10.670;
  PSU5v = (adc4 + 16) / 10.650;
  
  Serial.print("24v Supply #1: "); Serial.println(PSU24v1);
  Serial.print("24v Supply #2: "); Serial.println(PSU24v2);
  Serial.print("12v Rail: "); Serial.println(PSU12v);
  Serial.print("5v Rail: "); Serial.println(PSU5v);
  Serial.println(" ");
    char Power1[6];
  dtostrf(PSU24v1,4,2,Power1);

  client.publish("/read/arduino_LED/PSU1",Power1);
  
}
