#include <DHT.h>

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"      

#include "DHT.h"

#define DHTPIN 5    //Dht sensor on PIN 5
#define DHTTYPE DHT11  //Dht sensor module is DHT11

#include <SimpleTimer.h>
 SimpleTimer timer;//https://github.com/tzapu/WiFiManager
#define TOKEN "";//"JOb6fiY19iTC"//password for mqtt cloud
char myConcatenation[80];
char myCharArray[22]="31.265140,29.998630";

int flame;
int flamepin=4;
int smoke;
int smokepin=14;
String temp_str; //see last code block below use these to convert the float that you get back from DHT to a string =str
String hum_str;
char temp[50];
char hum[50];
char topic [] ="NASA/1";
char server[] =  "broker.mqttdashboard.com";//server address
char authMethod[] = "";//"elsfqlff";//username for mqtt cloud
char token[] = TOKEN;
char clientId[] = "zone1";
int counter=0;
long lastMsg = 0;
char msg[50];
int value = 0;
const int control=2;
char payload[4];
char data[4];
DHT dht(DHTPIN, DHTTYPE); //Installiation DHT Sensor to be use 
void callback(char* topic, byte* payload, unsigned int length);
WiFiClient wifiClient;

PubSubClient client(server,1883, callback, wifiClient);
////////////////////////
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}


///////////////////////////////
void setup_wifi() {
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
 //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
}
Serial.println("connected...yeey :)");
}
//////////////////////////////////////
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.println("Message arrived [");
  Serial.println(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.println((char)payload[i]);
  }
  Serial.println();


}
void setup() {
  // Initialize the BUILTIN_LED pin as an output 
  pinMode(flamepin, INPUT);
  pinMode(smokepin,INPUT);
 // timer.setInterval(200, sendUptime);

  Serial.begin(9600); //UART Speed is 9600 bit/second
    client.setCallback(callback);
  setup_wifi();
}
void loop() {
  
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(50);
 }
 Serial.println();
 }
 //client.publish("NASA/1", "1");
      // ... and resubscribe
//client.subscribe("NASA/1");
  client.loop();
  /*
  String totalPacket = String(h);
    totalPacket += ",";
    totalPacket += String(t);
*/

int   h = dht.readHumidity(); //Read the value of humanity from DHT Sensor and save it in h variable
  // Read temperature as Celsius (the default)
  int   t = dht.readTemperature(); //Read the value of Temparture from DHT Sensor and save it in h variable
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true); //Convert the value of value of temparture from DHT Sensor to celoius

smoke=digitalRead(smokepin);
flame =digitalRead(flamepin);
delay(50);
flame=!flame;
smoke=!smoke;
int id=1;

///////
/*
  String totalPacket = String(h);
    totalPacket += ",";
    totalPacket += String(t)
    ////////
    */
   sprintf(myConcatenation,"%i,%s,%i, %i, %i, %i",id, myCharArray, flame, smoke,t, h);
    Serial.println(myConcatenation);
client.publish("NASA/1",myConcatenation);

delay(450);
// client.publish("NASA/1",&totalPacket[0]);    

    
  }


  
void sendUptime()
{
}

