
/************************* Declaring Library *********************************/

#include "DHT.h" // https://github.com/adafruit/DHT-sensor-library


/************************* Declaring Pin *********************************/

#define DHTPIN 2     // Digital pin connected to the DHT sensor


/************************* Declaring DHT Sensor Type  *********************************/
#define DHTTYPE DHT11   // DHT 11


/************************* Declaring Object for DHT class  *********************************/
DHT dht(DHTPIN, DHTTYPE);

// Link for understanding Classes & Object - https://www.w3schools.com/cpp/cpp_classes.asp

void setup()
{
  Serial.begin(115200); // Beginning Serial Communication with Computer

  dht.begin(); // beginning communication with Sensor
}

void loop()
{

  // Read humidity
  float h = dht.readHumidity();

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  /************************* Printing Data on Serial Monitor  *********************************/
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.print("°C ");
}
