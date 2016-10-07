/*
 Based on ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-07/ESP-12 module and play a tune
 This example code is in the public domain
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 On the ESP-07 and ESP-12 modules the LED is connected to GPIO2.
 When using the board "Generic ESP8266 Module" LED_BUILTIN needs to be redefined.
*/
#define LED_BUILTIN 2 // pin 2 is used on Witty, ESP-07 and ESP-12

#include <MusicEngine.h>
#define BUZ_PIN 14
MusicEngine music(BUZ_PIN);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP module)
  delay(100);                      // Wait for a tenth of a second to flash the led
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

  
  music.play("T180 L8 CDEC. r CDEC. r EFG. r EFG. r GAGFEC. r GAGFEC. r L4 C<A>C. r C<A>C.");

  delay(10000);                      // Wait for ten seconds (to let the music play)
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  music.play("T240 L16 O8 C");      // give a short beep
  delay(1000);                      // Wait for one second to show the led switched on
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  music.play("T240 L32 O8 D");      // give a shorter slightly higher beep
  delay(2000);                      // Wait for two second to show the led switched off
}
