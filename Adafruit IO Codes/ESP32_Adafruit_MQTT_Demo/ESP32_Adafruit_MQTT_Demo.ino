/************************* Library Declaration *********************************/
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


/************************* Pin Declaration *********************************/
#define LED 2

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "SmS_jiofi"
#define WLAN_PASS       "sms123458956"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "techiesms"
#define AIO_KEY         "912b30c900574034a653f41e2b4df838"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP32 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


/****************************** Feeds(Topics) ***************************************/

// Setup a feed called 'sensor' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish sensor = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensor");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe light = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light");

/*************************** Sketch Code ************************************/

void setup()
{
  Serial.begin(115200); // Beginning Serial Communication between ESP32 and Computer at 115200 baud rate

  pinMode(LED, OUTPUT); // Declaring LED as OUTPUT

  Serial.println("Adafruit MQTT demo"); // It will be printed on Serial Monitor


  /*************************** WiFi Connection Code ************************************/
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());




  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&light);
}

uint32_t x = 0;

void loop()
{
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &light)
    {
      // Storing the data
      char* Data = (char*) light.lastread;
      Serial.print("Got: ");
      Serial.println(Data);

      // comparing the data
      if (strstr(Data, "1"))
      {
        digitalWrite(LED, HIGH);
      }
      if (strstr(Data, "0"))
      {
        digitalWrite(LED, LOW);
      }
    }
  }

  // Now we can publish stuff!
  Serial.print("\nSending sensor val ");
  Serial.print(x);
  Serial.print("...");
  sensor.publish(x);
  x++;

}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect()
{
  int ret;

  // Stop if already connected.
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  int retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
