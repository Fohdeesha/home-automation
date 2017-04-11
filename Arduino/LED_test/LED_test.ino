#include <PubSubClient.h>
#include <SPI.h>
#include <Ethernet.h>


// Set the MAC address
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x5E, 0x69 };

IPAddress ip(192, 168, 1, 21);
IPAddress server(192, 168, 1, 28);


EthernetClient ethClient;
PubSubClient client(ethClient);

int SoffitR;
int SoffitG;
int SoffitB;
int BLUE = 6;                   
int GREEN = 5; 
int RED = 3; 
int red1;
int green1;
int blue1;

void setup()
{

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
    // Open serial communications
  Serial.begin(9600);
  
    // Start with a hard-coded address:
    Serial.println("Assigning Static IP address:");
    Ethernet.begin(mac, ip);

  client.setServer(server, 1883);
  client.setCallback(callback);
  
     Serial.print("My address:");
  Serial.println(Ethernet.localIP());
  
  // Connect to Broker, give it arduino as the name
  if (client.connect("arduino_LED")) {
    
    // Publish a message to the status topic
    client.publish("status/arduino_LED","Arduino LED is now online");
    
    // Listen for messages on the control topic
    client.subscribe("control/arduino_LED/#");
   }
}

void loop()
{
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  // check for messages on subscribed topics
  payload[length] = '\0';
  Serial.print("Topic: ");
  Serial.println(String(topic));
  


  // check topic to identify type of content
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
Serial.print("RED: ");
Serial.println(red1);
//client.publish("status/arduino_LED", SoffitR);

Serial.print("GREEN: ");
Serial.println(green1);
//client.publish("status/arduino_LED", SoffitG);

Serial.print("BLUE: ");
Serial.println(blue1);
//client.publish("status/arduino_LED", int SoffitB);
//Serial.flush();
    
  }
}
