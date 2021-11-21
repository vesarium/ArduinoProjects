#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <FS.h>   //Include File System Headers
#include "max6675.h"
#include "DHTesp.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFiManager.h>

WiFiManager wifiManager;
DHTesp dht;

int due_watering = 0;
float humidity;
float temperature;

unsigned long timestamp;
unsigned long lastCheck;

// set relay pin numbers
const int light_pin = D6;     // light
const int w_pump_pin = D7;    // waterpumb  
const int humidifier_pin = D0;    // humidifier 
const int duct_fan_pin = D1;      // duct fan   

bool w_pump_status = 0;
bool light_status = 0;
bool humidifier_status = 0;
bool duct_fan_status = 0;

/*
int thermoDO = D5;
int thermoCS = D6;
int thermoCLK = D7;
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
*/

struct Config {
  int watering_period;
  int watering_duration;
  int light_on_time;
  int light_duration;
  int humidity_range[2];
  int temp_range[2];
};

int watering_start = 0;
int next_watering;
int light_turn_off = 0;

/************************* MQTT Server Setup *********************************/

#define AIO_SERVER      "194.5.159.69"
#define AIO_SERVERPORT  1883
#define AIO_CID         "73t9sywq" // 73t9sywq
#define AIO_USERNAME    "grobot"
#define AIO_KEY         "grobot"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_CID, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish airtemp = Adafruit_MQTT_Publish(&mqtt, AIO_CID "/air/temp");
Adafruit_MQTT_Publish airhumidity = Adafruit_MQTT_Publish(&mqtt, AIO_CID "/air/humidity");
Adafruit_MQTT_Publish watering_due = Adafruit_MQTT_Publish(&mqtt, AIO_CID "/watering/due");
Adafruit_MQTT_Publish watering_time = Adafruit_MQTT_Publish(&mqtt, AIO_CID "/watering/time");
Adafruit_MQTT_Publish watering_timer = Adafruit_MQTT_Publish(&mqtt, AIO_CID "/watering/timer");

// Setup a feed called 'onoff' for subscribing to changes to the button
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_CID "/waterpump/onoff");
Adafruit_MQTT_Subscribe setlighttime = Adafruit_MQTT_Subscribe(&mqtt, AIO_CID "/light/time/set");
Adafruit_MQTT_Subscribe setlightduration = Adafruit_MQTT_Subscribe(&mqtt, AIO_CID "/light/duration/set");
Adafruit_MQTT_Subscribe setwateringduration = Adafruit_MQTT_Subscribe(&mqtt, AIO_CID "/waterpump/duration/set");
Adafruit_MQTT_Subscribe setwateringperiod = Adafruit_MQTT_Subscribe(&mqtt, AIO_CID "/waterpump/period/set");

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* confFile = "/conf.json";
Config config; 
StaticJsonDocument<512> doc;

void slidercallback(double x) {
  Serial.print("Hey we're in a slider callback, the slider value is: ");
  Serial.println(x);
}

void onoffcallback(char *data, uint16_t len) {
  Serial.print("Hey we're in a onoff callback, the button value is: ");
  Serial.println(data);
}

void setup(){
  Serial.begin(115200);
  delay(500);
  wifiManager.setConfigPortalTimeout(30);
  wifiManager.autoConnect("Grobot-WiFi","getitgrown");
  
  //Initialize File System
  if(SPIFFS.begin())
  {
    Serial.println("SPIFFS Initialize....ok");
  }
  else
  {
    Serial.println("SPIFFS Initialization...failed");
  }
  File f = SPIFFS.open(confFile, "r");
  DeserializationError error = deserializeJson(doc, f);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));
    
  // Copy values from the JsonDocument to the Config
  config.watering_duration = doc["watering_duration"];
  config.watering_period = doc["watering_period"];
  config.light_on_time = doc["light_on_time"];
  config.light_duration = doc["light_duration"];
  config.temp_range[0] = doc["temp_min_value"];
  config.temp_range[1] = doc["temp_max_value"];
  config.humidity_range[0] = doc["humidity_min_value"];
  config.humidity_range[1] = doc["humidity_max_value"];
   
//  config.w_temp = thermocouple.readCelsius(); 
//  config.last_watering = doc["last_watering"]; 
  f.close();
  timeClient.begin();

  if(config.light_on_time + config.light_duration >= 24){
    light_turn_off = config.light_on_time + config.light_duration - 24;
  }else{
    light_turn_off = config.light_on_time + config.light_duration;
  }
  timeClient.update();
  next_watering = timeClient.getEpochTime() + config.watering_period;

  pinMode(w_pump_pin, OUTPUT);
  digitalWrite(w_pump_pin, LOW);
   
  pinMode(light_pin, OUTPUT);
  digitalWrite(light_pin, HIGH);
  
  pinMode(humidifier_pin, OUTPUT);
  digitalWrite(humidifier_pin, HIGH);
  
  pinMode(duct_fan_pin, OUTPUT);
  digitalWrite(duct_fan_pin, HIGH);
  
  dht.setup(D4, DHTesp::DHT22);

  //slider.setCallback(slidercallback);
  onoffbutton.setCallback(onoffcallback);
  
  // Setup MQTT subscription for time feed.
  //mqtt.subscribe(&slider);
  mqtt.subscribe(&onoffbutton);
  mqtt.subscribe(&setlighttime);
  mqtt.subscribe(&setlightduration);

  mqtt.subscribe(&setwateringduration);
  mqtt.subscribe(&setwateringperiod);
  
}

bool saveConfig() {
   doc["watering_period"] = config.watering_period;
   doc["watering_duration"] = config.watering_duration;
   doc["light_on_time"] = config.light_on_time;
   doc["light_duration"] = config.light_duration;

   if(config.light_on_time + config.light_duration >= 24){
    light_turn_off = config.light_on_time + config.light_duration - 24;
   }else{
    light_turn_off = config.light_on_time + config.light_duration;
   }
  
  File configFile = SPIFFS.open(confFile, "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  serializeJson(doc, configFile);
  configFile.close();
  return true;
}

int getCurrentHour() {
  int current_hour = ((int)timeClient.getEpochTime()  % 86400L) / 3600;
  return current_hour;
  
}

int getCurrentMinute() {
  int current_minute = ((int)timeClient.getEpochTime()  % 3600) / 60;
  return current_minute;  
}


void turnOnWatering(){
  digitalWrite(w_pump_pin, HIGH); 
  w_pump_status = 1;
  watering_start = timeClient.getEpochTime();
  Serial.println("watering started");
}
void turnOffWatering(){
  digitalWrite(w_pump_pin, LOW);
  w_pump_status = 0; 
  Serial.println("watering stopped");
}

void turnOnLight(){
  digitalWrite(light_pin, LOW); 
  Serial.println("Light is switched on");
  light_status = 1; 
}
void turnOffLight(){
  digitalWrite(light_pin, HIGH); 
  Serial.println("Light is switched off");
  light_status = 0; 
}

void turnOnFan(){
  digitalWrite(duct_fan_pin, LOW); 
  Serial.println("Fan is switched on");
  duct_fan_status = 1; 
}
void turnOffFan(){
  digitalWrite(duct_fan_pin, HIGH); 
  Serial.println("Fan is switched off");
  duct_fan_status = 0; 
}

void turnOnHumidifier(){
  digitalWrite(humidifier_pin, LOW); 
  Serial.println("Humidifier is switched on");
  humidifier_status = 1; 
}
void turnOffHumidifier(){
  digitalWrite(humidifier_pin, HIGH); 
  Serial.println("Humidifier is switched off");
  humidifier_status = 0; 
}



void loop() {
  timeClient.update();
  if (duct_fan_status == 0){
    turnOnFan();
  }
  //Serial.println(light_turn_off);
  //Serial.println(config.light_duration);
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
   /* 
    if (subscription == &wateringtime) {
      
      getWateringTime();
      watering_time.publish(config.w_time, 1);
    }
    

    if (subscription == &wateringtimer) {      
      getWateringTimer();
      watering_timer.publish(config.w_timer, 1);
    }

    */   

    if (subscription == &setlighttime){
      config.light_on_time = atoi((char *)setlighttime.lastread);
      saveConfig();   
    }

    if (subscription == &setlightduration){
      config.light_duration = atoi((char *)setlightduration.lastread);
      saveConfig();   
    }

    if (subscription == &setwateringduration){
      config.watering_duration = atoi((char *)setwateringduration.lastread);
      
      saveConfig();   
    }
    
    if (subscription == &setwateringperiod){
      config.watering_period = atoi((char *)setwateringperiod.lastread);
      next_watering = timeClient.getEpochTime() + config.watering_period;      
      saveConfig();     
    }   
    
    if (subscription == &onoffbutton) {
      //Serial.print(F("On-Off button: "));
      //Serial.println((char *)onoffbutton.lastread);
      
      if (strcmp((char *)onoffbutton.lastread, "ON") == 0) {
        //digitalWrite(w_pump_pin, LOW); 
        turnOnWatering();
        
      }
      if (strcmp((char *)onoffbutton.lastread, "OFF") == 0) {
        //digitalWrite(w_pump_pin, HIGH); 
        turnOffWatering(); 
        
      }
    }
  }
  if(config.light_on_time + config.light_duration < 24){
    if(getCurrentHour() >= config.light_on_time && getCurrentHour() < config.light_on_time + config.light_duration){
      if(light_status == 0){
        turnOnLight();
      }   
    }else{
      if(light_status == 1){
        turnOffLight();
      }
    }
  }else{
    if(getCurrentHour() > light_turn_off && getCurrentHour() < config.light_on_time){
      if(light_status == 1){
        turnOffLight();
      }
    }else{
      if(light_status == 0){
        turnOnLight();
      }      
    }
  }  
  
  if(next_watering<=timeClient.getEpochTime()){
    next_watering = timeClient.getEpochTime() + config.watering_period;
    turnOnWatering();
  }
  
  if((timeClient.getEpochTime() - watering_start) >= config.watering_duration && w_pump_status == 1){
    turnOffWatering();    
  }
  
  timestamp = timeClient.getEpochTime();
  
  if(timeClient.getEpochTime()-lastCheck>=20){
      
    lastCheck=timeClient.getEpochTime();
    if (humidity!=dht.getHumidity() || temperature!=dht.getTemperature()){

// PUBLISH JSON       
//airhumidity.publish("{\"temperature\": \"99\"}");

      airtemp.publish(dht.getTemperature(), 1);
      airhumidity.publish(dht.getHumidity(), 1);

      watering_due.publish((next_watering - timeClient.getEpochTime())/60, 1); 
      
    }       
   }
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
