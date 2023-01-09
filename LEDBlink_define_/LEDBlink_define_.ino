
#define LED 2 // #define will replace pin 2 to the name LED 

// the setup function runs once when you press reset or power the board
void setup()
{

  // To use any Pin as Input or Output, we first need to define that here in Setup part

  pinMode(LED, OUTPUT); // initialize LED as an output.
}

// the loop function runs over and over again forever

void loop()
{
  digitalWrite(LED, HIGH);   // turn the LED ON by making the pin HIGH  ( HIHH -> 3.3V )

  delay(1000);                       // wait for a second (1000 means 1000ms = 1s)

  digitalWrite(LED, LOW);    // turn the LED OFF by making the pin LOW  ( LOW -> 0V )

  delay(1000);                       // wait for a second  (1000 means 1000ms = 1s)
}
