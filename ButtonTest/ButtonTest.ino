#include <Wire.h>
#include "htk.h"

#define LED LED_BUILTIN

HT16K33 keymagic(0x70);

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
  static bool noned = false;
  static uint8_t count = 0;
  if (keymagic.keyready()) {
    noned = false;
    Serial.print("INT ");
    Serial.print(intval, BIN);
    Serial.print(' ');
    Serial.println(count++);
    
    HT16K33::keystruct keys = keymagic.keydata();
    Serial.print("\tKS0 ");
    Serial.println(keys.KS0, BIN);
    Serial.print("\tKS1 ");
    Serial.println(keys.KS1, BIN);
    Serial.print("\tKS2 ");
    Serial.println(keys.KS2, BIN);
    digitalWrite(LED, keys.KS0 || keys.KS1 || keys.KS2);
  }
}
