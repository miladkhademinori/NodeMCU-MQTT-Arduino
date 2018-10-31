//#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <String.h>
//#include <Wire.h>
//#include <Adafruit_INA219.h>
 
// Connect to the WiFi
const char* ssid = "AIMS";
const char* password = "aims7933";
const char* mqtt_server = "172.21.216.242";


// wifi 
WiFiClient espClient;
PubSubClient client(espClient);

// variables
int lightOfPhotocell=0;
unsigned long time1=0;
char* temporary;
char buf[10];
String accelerometer_x;
//String accelerometer_y;
char receivedChar[8];

// ports 
const int analoginpin = A0; // analog input pin
const int analogoutpin = 5; // analog output

// functions 
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived  [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  receivedChar[i] = (char)payload[i];
  
  }
  Serial.print(receivedChar);
  if (atoi(receivedChar)>0)
  analogWrite(analogoutpin,255);
  else
  analogWrite(analogoutpin,0);
  Serial.println();
}
 
 
void reconnect() {
 // Loop until we're reconnected to server
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
  Serial.println("connected to the mqtt broker");
  // ... and subscribe to topic
  client.subscribe("accelerometer_x");
  //client.subscribe("accelerometer_y");
 } 
 else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(2000);
  }
 }
}


// setup 
void setup()
{
                   
                    Serial.begin ( 115200 );
                    WiFi.begin ( ssid, password );
                    Serial.println ( "" );
                    // Wait for connection
                    while ( WiFi.status() != WL_CONNECTED ) {
                    delay ( 500 );
                    Serial.print ( "." );
                  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );
  
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 }


// loop 
void loop()
{
  
 if (!client.connected()) {
  reconnect();
 }
 if (millis() > (time1 + 2000)) {
 time1 = millis();
lightOfPhotocell = analogRead(analoginpin);
sprintf (buf, "%d", lightOfPhotocell);
//temporary = dtostrf (analogRead(analoginpin),5,2,message_buffer); // TMP36 sensor calibration
client.publish("lightOfPhotocell",buf);
Serial.print ( buf );
Serial.println();
}
 client.loop();

}
