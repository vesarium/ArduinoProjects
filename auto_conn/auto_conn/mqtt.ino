#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* MQTT Server Setup *********************************/

#define AIO_SERVER      "194.5.159.69"
#define AIO_SERVERPORT  1883
#define AIO_CID         "73t9sywq" // 83t9sywq
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


// Setup a feed called 'onoff' for subscribing to changes to the button
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/waterpump/onoff");
Adafruit_MQTT_Subscribe wateringtime = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/waterpump/time/get");
Adafruit_MQTT_Subscribe wateringtimer = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/waterpump/timer/get");
Adafruit_MQTT_Subscribe setwateringtime = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/waterpump/time/set");
Adafruit_MQTT_Subscribe setwateringtimer = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/waterpump/timer/set");

Adafruit_MQTT_Subscribe aircononoff = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/aircon/onoff");
