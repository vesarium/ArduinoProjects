#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "DHTesp.h"
#include "mqtt.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define NTP_OFFSET   60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "europe.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);


DHTesp dht;

// set relay pin numbers
const int relay1 = D6;   // light
const int relay2 = D7;   // waterpumb  
const int relay3 = D5;   // humidifier 
const int relay4 = D8;   // duct fan   

unsigned long lastCheck;
float humidity;
float temperature;
int rel1state;
const char* serverName = "http://getitgrown.com/post-esp-data.php";
String apiKeyValue = "tPmAT5Ab3j7F9";

void setup() {
    timeClient.begin();
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    rel1state=0;
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);
    pinMode(relay4, OUTPUT);

    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    
    // WiFi.mode(WiFi_STA); // it is a good practice to make sure your code sets wifi mode how you want it.

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        //Serial.println("connected...");
    }

    dht.setup(D4, DHTesp::DHT22);
    

}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
 
  if(timeClient.getEpochTime()-lastCheck>=60){
      lastCheck=timeClient.getEpochTime();
      Serial.println(timeClient.getFormattedTime());
      
      if (humidity!=dht.getHumidity() || temperature!=dht.getTemperature()){
        
        switch (rel1state) {
          case 0: digitalWrite(relay1, LOW); digitalWrite(relay2, HIGH); rel1state=1 ;break;
          case 1: digitalWrite(relay2, LOW); digitalWrite(relay3, HIGH); rel1state=2 ;break;
          case 2: digitalWrite(relay3, LOW); digitalWrite(relay4, HIGH); rel1state=3 ;break;
          case 3: digitalWrite(relay4, LOW); digitalWrite(relay1, HIGH); rel1state=0 ;break;
        }
      
        Serial.print(dht.getHumidity());
        Serial.print(" : ");
        Serial.println(dht.getTemperature());
        humidity=dht.getHumidity();
        temperature=dht.getTemperature();
        
      }
  }
    
  
    
    
}
