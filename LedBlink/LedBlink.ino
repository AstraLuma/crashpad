#include <Wire.h>
#include "htk.h"

#define LED LED_BUILTIN
#define CHANNEL 1
#define VELOCITY 99
HT16K33 keymagic(0x70);

HT16K33::dispstruct on  = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
HT16K33::dispstruct off = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

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
  digitalWrite(LED, HIGH);
  keymagic.display(&on);
  delay(1000);
  digitalWrite(LED, LOW);
  keymagic.display(&off);
  delay(1000);
}
