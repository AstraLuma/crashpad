#include "htk.h"
#include <Wire.h>

HT16K33::HT16K33(uint8_t addr)
{
  _addr = addr;
  err = HT16K33::NOERROR;
  _nextkey = 0;
}

void HT16K33::begin()
{
    setup(true); // Turn on oscillator
}

void HT16K33::command(htaddr cmd)
{
  Wire.beginTransmission(_addr);
  Wire.write(cmd);
  err = (HT16K33::Error)Wire.endTransmission();
}

// Stream writing
void HT16K33::beginWrite(HT16K33::htaddr addr)
{
  Wire.beginTransmission(_addr);
  Wire.write(addr);
}

void HT16K33::write(uint8_t data)
{
  Wire.write(data);
}

void HT16K33::write(const uint8_t* data, size_t len)
{
  Wire.write(data, len);
}

void HT16K33::endWrite()
{
  err = (HT16K33::Error)Wire.endTransmission();
}

// One-off writing
void HT16K33::write(HT16K33::htaddr addr, uint8_t data)
{
  beginWrite(addr);
  write(data);
  endWrite();
}

void HT16K33::write(HT16K33::htaddr addr, const uint8_t* data, size_t len)
{
  beginWrite(addr);
  write(data, len);
  endWrite();
}

// Stream reading
void HT16K33::beginRead(HT16K33::htaddr addr, uint8_t len)
{
  command(addr);
  Wire.requestFrom(_addr, len);
}

bool HT16K33::available()
{
  return Wire.available() ? true : false;
}

uint8_t HT16K33::read()
{
  return Wire.read();
}

// One-off reading
uint8_t HT16K33::read(HT16K33::htaddr addr)
{
  beginRead(addr, 1);
  return read();
}

void HT16K33::setup(bool osc)
{
  command(0b00100000 | (osc?1:0));
}

void HT16K33::rowint(bool rowint, bool act)
{
  uint8_t cmd = 0b10100000;
  if (rowint) {
    cmd |= 0b1;
  }
  if (act) {
    cmd |= 0b10;
  }
  command(cmd);
}

void HT16K33::display_setup(bool on, Blink blink)
{
  uint8_t cmd = 0b10000000;
  if (on) {
    cmd |= 0b1;
  }
  cmd |= blink;
  command(cmd);
}

void HT16K33::display(const HT16K33::dispstruct *data)
{
  write(0x00, (const uint8_t*)data, sizeof(uint8_t)*HT16K33_ROWS);
}

void HT16K33::dimming(uint8_t level)
{
  command(0b11100000 | ((level - 1) & 0b1111));
}

HT16K33::keystruct HT16K33::keydata()
{
  HT16K33::keystruct rv = {{0,0,0}};
  _nextkey = millis() + HT16K33_KEYTIME;
  beginRead(0x40, 6);
  uint8_t low = read();
  uint16_t high = read();
  rv.keys[0] = (high << 8) | low;
  low = read();
  high = read();
  rv.keys[1] = (high << 8) | low;
  low = read();
  high = read();
  rv.keys[2] = (high << 8) | low;
  endRead();
  return rv;
}

bool HT16K33::interrupt()
{
  return read(0x60);
}

