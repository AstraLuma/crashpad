#include <Wire.h>
#include "htk.h"

#define LED LED_BUILTIN
#define CHANNEL 1
#define VELOCITY 99
HT16K33 keymagic(0x70);

HT16K33::dispstruct buffer = {0,0,0,0,0,0,0,0};

void setup() 
{
  pinMode(LED, OUTPUT);
  Wire.begin(); // Must be done before any HT stuff works

  digitalWrite(LED, HIGH);
  keymagic.setup(true); // Turn on oscillator
  if (keymagic.err) Serial.println(keymagic.err);
  keymagic.display_setup(true); // Turn on display
  if (keymagic.err) Serial.println(keymagic.err);
  keymagic.rowint(false); // Disable INT
  if (keymagic.err) Serial.println(keymagic.err);
  digitalWrite(LED, LOW);
}

void loop() 
{
  int i;
  for (i = 0; i < 8; i++)
  {
    digitalWrite(LED, i & 1);
    buffer.rows[i] = 0xFF;
    keymagic.display(&buffer);
    delay(1000);
    buffer.rows[i] = 0;
  }
  for (i = 0; i < 8; i++)
  {
    digitalWrite(LED, i & 1);
    for (int r = 0; r < 8; r++)
    {
      buffer.rows[r] = 1 << i;
    }
    keymagic.display(&buffer);
    delay(1000);
  }
  for (i = 0; i < 8; i++)
  {
    buffer.rows[i] = 0;
  }
}
