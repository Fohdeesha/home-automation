#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Arduino Kitchen (LED Strip Control)
byte mac[] = { 0xDE, 0xA2, 0xDA, 0x0F, 0x5E, 0xED };
IPAddress ip(192, 168, 1, 32);
IPAddress server(192, 168, 1, 28);
unsigned long ts = millis();

int redpin = 3;
int greenpin = 5;
int bluepin = 6;

void callback(char* topic, byte* payload, unsigned int length) {

  int redraw;
  int greenraw;
  int blueraw;
  int red;
  int green;
  int blue;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
      if(String(topic) == "/control/Arduino_Kitchen/color") {
    // convert payload to String
    String value = String((char*)payload);
    // split string at every "," and store in proper variable
    // convert final result to integer
    redraw = value.substring(0,value.indexOf(',')).toInt();
    greenraw = value.substring(value.indexOf(',')+1,value.lastIndexOf(',')).toInt();
    blueraw = value.substring(value.lastIndexOf(',')+1).toInt();

    red =   redraw   * 2.55;
    green = greenraw * 2.55;
    blue =  blueraw  * 1.50;
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

  }

}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Arduino_Kitchen")) {
      Serial.println("connected");
     client.publish("/status/Arduino_Kitchen","Arduino Kitchen is now online");
     client.subscribe("/control/Arduino_Kitchen/#");
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

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(50);
}
