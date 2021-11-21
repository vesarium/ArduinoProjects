//#include <SPI.H>
#include <FS.H>           // Include the SPIFFS library


void setup() {    
  Serial.begin(115200);

//Initialize File System
  if(SPIFFS.begin())
  {
    SPIFFS.format();
    Serial.println("SPIFFS Initialize....ok");
  }
  else
  {
    Serial.println("SPIFFS Initialization...failed");
  }

}

void loop() {    }
