/*
 * MusicEngine library for ESP8266 in Arduino IDE minimal example
 * by MMOLE (@maxint-rd)
 * see https://github.com/maxint-rd/ESP-MusicEngine
 */
#include <MusicEngine.h>

// Initialize MusicEngine object (buzzer between pin 14 and GND)
MusicEngine music(14);

void setup()
{
    // only play one tune
    music.play("T180 L8 CDEC. r CDEC. r EFG. r EFG. r GAGFEC. r GAGFEC. r L4 C<A>C. r C<A>C.");
}

void loop()
{
    // nothing to do
}
