
/************************* Declaring Libraries *********************************/

#include <WiFi.h>
#include <PubSubClient.h>
#include <SimpleTimer.h>

/************************* WiFi Access Point *********************************/

const char* ssid = "SSID";
const char* password = "PASS";

/************************* CloudMQTT Setup *********************************/

const char* mqtt_server = "SERVER ADDRESS";
const int port = PORT_NUMBER;
const char* USERNAME = "USERNAME";
const char* PASSWORD = "PASSWORD";


/************************* Defining Relay Pins *********************************/

#define LED 2

/************************* Topics to Subscribe *********************************/

#define SUB_TOPIC1 "esp32/light"

#define PUB_TOPIC1 "esp32/sensor"

/************************* WiFi Client Setup *********************************/

WiFiClient espClient;
PubSubClient client(espClient);

/************************* Timer Setup *********************************/

SimpleTimer Timer;

int counter = 0;

/************************* WiFi Setup *********************************/

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


/************************* Callback Function *********************************/


void callback(char* topic, byte* payload, unsigned int length)
{


  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();


  // Comparing the topics
  if ( strstr(topic, SUB_TOPIC1) )
  {
    if ((char)payload[0] == '1')
    {
      digitalWrite(LED, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    }
    if ((char)payload[0] == '0')
    {
      digitalWrite(LED, LOW);  // Turn the LED off by making the voltage HIGH
    }
  }
}

/************************* Responsible for MQTT Connection *********************************/
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "esp32client";
    // Attempt to connect
    if (client.connect(clientId.c_str(), USERNAME, PASSWORD) ) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe(SUB_TOPIC1);  // Subscribing to the Topics
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  setup_wifi();

  client.setServer(mqtt_server, port);
  client.setCallback(callback);

  // Set an interval to 5 secs for the second timer
  Timer.setInterval(5000);
}

void loop() {

  // ReConnect to Cloud if disconnected
  if (!client.connected())
  {
    reconnect();
  }

  client.loop(); // Handles all the task in backend


  if (Timer.isReady())
  { // Check is ready a second timer
    Serial.println("Called every 3 sec");
    Serial.print("Sending Data - "); Serial.println(counter);


    String TEMPORARY = (String) counter; // Coverting Int into String
    char* value = &TEMPORARY[0];         // Coverting String into char*

    client.publish(PUB_TOPIC1, value);

    counter = counter + 1;
    Timer.reset();                        // Reset a second timer
  }

}
