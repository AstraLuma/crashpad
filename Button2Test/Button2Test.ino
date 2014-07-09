#include <Wire.h>
#include "htk.h"

#define LED LED_BUILTIN
#define CHANNEL 1
#define VELOCITY 99

HT16K33 keymagic(0x70);

HT16K33::keystruct last = {0,0,0};

void setup() 
{
  pinMode(LED, OUTPUT);
  Wire.begin(); // Must be done before any HT stuff works

  digitalWrite(LED, HIGH);
  keymagic.setup(true); // Turn on oscillator
  if (keymagic.err) Serial.println(keymagic.err);
  keymagic.display_setup(false); // Turn off display
  if (keymagic.err) Serial.println(keymagic.err);
  keymagic.rowint(false); // Disable INT
  if (keymagic.err) Serial.println(keymagic.err);
  digitalWrite(LED, LOW);
}

void loop() 
{
  uint8_t intval = keymagic.interrupt();
  if (keymagic.keyready()) 
  {
    HT16K33::keystruct keys = keymagic.keydata();
    digitalWrite(LED, keys.KS0 || keys.KS1 || keys.KS2);
    for (uint8_t i = 0; i < 3; i++) 
    {
      for (uint8_t m = 0; m < 13; m++) 
      {
        size_t b = 1 << m;
        if ((keys.keys[i] & b) != (last.keys[i] & b)) 
        {
          unsigned int note = i * 13 + m + 1;
          if (keys.keys[i] & b) 
          {
            Serial.print("ON  ");
            Serial.println(note);
          }
          else
          {
            Serial.print("OFF ");
            Serial.println(note);
          }
        }
      }
    }
    last = keys;
  }
}
