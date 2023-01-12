#include "secrets.h" // Contains all the credentials 


/************************* Declaring Libraries *********************************/
// Important Libraries
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <SimpleTimer.h>

/************************* Pin Declaration *********************************/
#define LED 2

//Counter Initialisation
int c = 0;


/************************* Defining Topics *********************************/
// Topics of MQTT
#define AWS_IOT_PUBLISH_TOPIC1   "esp32/sensor"

#define AWS_IOT_SUBSCRIBE_TOPIC1 "esp32/light"


SimpleTimer Timer; // Declaring Timer Object for Sending data at particular time


// Declaring Oject for Wifi Secure Client
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

/************************* This Function is responsible for WiFi and MQTT connection *********************************/
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  // Create a message handler
  client.setCallback(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC1);

  Serial.println("AWS IoT Connected!");
}

/************************* This Function is responsible for all the Incoming Data *********************************/

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);

  if ( strstr(topic, "esp32/light") )
  {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);
    String Relay_data = doc["status"];
    int value = Relay_data.toInt();
    digitalWrite(LED, value);
    Serial.print("LED - "); Serial.println(value);
  }

}


void setup()
{
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  // Set an interval to 3 secs for the second timer
  Timer.setInterval(3000);

  connectAWS();
}

void loop()
{

  client.loop(); // Handles all the MQTT realted task in the backend


  if (Timer.isReady())
  { // Check is ready a second timer
    Serial.println("Called every 3 sec");

    StaticJsonDocument<200> doc;
    doc["message"] = "Hello from ESP32";
    doc["Counter"] = c;
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer); // print to client
    client.publish(AWS_IOT_PUBLISH_TOPIC1, jsonBuffer);
    Serial.println("Message Published");
    c++;
    Timer.reset();                        // Reset a second timer
  }
}
