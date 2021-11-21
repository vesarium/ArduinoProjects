#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include "DHTesp.h"

ESP8266WebServer Server;
AutoConnect      Portal(Server);
WiFiClient client;

const uint16_t port = 8090;
const char * host = "46.22.210.157";
float calibration = 2.50; //change this value to calibrate
const int analogInPin = A0;
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp; 
DHTesp dht;

void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}

void setup() {
  
  Serial.begin(115200);
  Serial.println();
  dht.setup(13, DHTesp::DHT11); // Connect DHT sensor to GPIO 15

  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("HTTP server:" + WiFi.localIP().toString());
  }
  
}
 
void loop()
{
  Portal.handleClient();
  
  float phValue = phmeasure();
  float temperature, humidity;
  delay(dht.getMinimumSamplingPeriod());  
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();  
 
    if (!client.connect(host, port)) {
 
        Serial.println("Connection to host failed");
 
        delay(1000);
        return;
    }
 
    Serial.println("Connected to server successful!");
    //client.print("(" + String(phValue)+","+String(temperature)+","+String(humidity)+")");
    //Serial.println("(" + String(temperature)+","+String(humidity)+")");
    client.print("(" + String("A001") + "," + String(temperature)+","+String(humidity)+")");
    Serial.println("Disconnecting...");
    client.stop();
 
    delay(10000);
    
}


float phmeasure() {
   for(int i=0;i<10;i++) 
     {
      buf[i]=analogRead(analogInPin);
      delay(30);
     }
   for(int i=0;i<9;i++)
    {
      for(int j=i+1;j<10;j++)
        {
          if(buf[i]>buf[j])
            {
              temp=buf[i];
              buf[i]=buf[j];
              buf[j]=temp;
            }
        }
    }
   avgValue=0;
   for(int i=2;i<8;i++)
   avgValue+=buf[i];
   float pHVol=(float)avgValue*5.0/1024/6;
   float phValue = (float)avgValue;
   //float phValue = 5.7 * pHVol + calibration;
   return phValue;
   delay(500);

}
