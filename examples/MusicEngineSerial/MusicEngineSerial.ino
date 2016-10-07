/*
 * MusicEngine library example for ESP8266 in Arduino IDE
 * by MMOLE (@maxint-rd)
 * see https://github.com/maxint-rd/ESP-MusicEngine
 */
#include <MusicEngine.h>

// define pin and initialize MusicEngine class
#define BUZ_PIN 14
MusicEngine music(BUZ_PIN);

void setup()
{ // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("=================="));
  Serial.println(F("Serial MusicEngine"));
  Serial.println(F("=================="));
  Serial.println(F("Please type your notes and hit Enter."));
  Serial.println(F("Example tune: T180 L8 CDEC. r CDEC. r EFG. r EFG. r GAGFEC. r GAGFEC. r L4 C<A>C. r C<A>C."));
  music.play("T240 L16 O8 rCDEDC");      // give a short blurp
}

void loop()
{ // put your main code here, to run repeatedly:
  while(!Serial.available())
  { // wait for a serial string to be entered, give a beep and a dot to indicate we're waiting
    for(int n=0; n<100 && !Serial.available(); n++)
      delay(100);    // just delay while waiting for input
    Serial.print(".");
    music.play("T250 L64 O7 B");      // give a short tick-like beep
  }

  if(Serial.available() > 0)
  { // assume the string is valid MML and try to play it
    char szBuf[256];  // serial buffer seems to be only 128 bytes
    int nRead=Serial.readBytesUntil('\n', szBuf, sizeof(szBuf)-1);
    szBuf[nRead]='\0';  // terminate string
    Serial.println(F("\nPlaying tune:"));
    Serial.println(szBuf);
    Serial.print(F("Please type new notes to play something else."));
    music.play(szBuf);
  }
}
