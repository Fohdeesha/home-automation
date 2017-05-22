#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Arduino Kitchen (LED Strip Control)
byte mac[] = { 0xDE, 0xA2, 0xDA, 0x0F, 0x5E, 0xED };
IPAddress ip(192, 168, 1, 32);
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
      if(String(topic) == "/control/Arduino_Kitchen/color") {
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
  int blue = (blueraw * 0.7) * 2.55;
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
}
