#include <NTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FS.h>   //Include File System Headers

struct Config {
  char game[64];
  int codes[];
};

const char *ssid     = "BTHub6-932P";
const char *password = "WxJgdHvR9Li9";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


const char* filename = "/samplefile.txt";
const char* filename2 = "/config.txt";
Config config; 
StaticJsonDocument<512> doc;


  
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  

  //Initialize File System
  if(SPIFFS.begin())
  {
    Serial.println("SPIFFS Initialize....ok");
  }
  else
  {
    Serial.println("SPIFFS Initialization...failed");
  }
  File f2 = SPIFFS.open(filename2, "r");
  DeserializationError error = deserializeJson(doc, f2);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
    
  for (int i=0; i< doc["codes"].size(); i++){
    config.codes[i] = doc["codes"][i];  
  }


  
  /*
  config.port = doc["port"] | 2731;
  strlcpy(config.hostname,                  // <- destination
          doc["hostname"] | "example.com",  // <- source
          sizeof(config.hostname));         // <- destination's capacity
  */

/*
  //Format File System
  if(SPIFFS.format())
  {
    Serial.println("File System Formated");
  }
  else
  {
    Serial.println("File System Formatting Error");
  }
*/
  //Create New File And Write Data to It
  //w=Write Open file for writing
  /*File f = SPIFFS.open(filename, "w");
  
  if (!f) {
    Serial.println("file open failed");
  }
  else
  {
      //Write data to file
      Serial.println("Writing Data to File");
      f.print("This is sample data which is written in file");
      f.close();  //Close file
  }
  */

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }else
  {
    Serial.print ( "Connected!" );
  }

  timeClient.begin();

}

void loop() {
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

  delay(1000);

  
  
  //Read File data
  File f = SPIFFS.open(filename, "r");

  for (int i=0; i< doc["codes"].size(); i++){
    Serial.println(config.codes[i]);
  }    

  
  if (!f) {
    Serial.println("file open failed");
  }
  else
  {
      //Serial.println("Reading Data from File:");
      //Data from file
      for(int i=0;i<f.size();i++) //Read upto complete file size
      {
        //Serial.print((char)f.read());
      }
      f.close();  //Close file
      //Serial.println("File Closed");
  }
  delay(5000);
}
