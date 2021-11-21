#include <ESP8266WiFi.h>          // Replace with WiFi.h for ESP32
#include <ESP8266WebServer.h>     // Replace with WebServer.h for ESP32
#include <AutoConnect.h>
#include "DHTesp.h"

ESP8266WebServer Server;          // Replace with WebServer for ESP32
AutoConnect      Portal(Server);
DHTesp dht;

void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  delay(1000);
  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
  dht.setup(13, DHTesp::DHT11); // Connect DHT sensor to GPIO 15

}

void loop() {
  Portal.handleClient();
  
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(humidity, 1);
  Serial.print("\t");
  Serial.print(temperature, 1);
  Serial.print("\n");
  delay(200);
  
    
}
