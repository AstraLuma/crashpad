#ifndef HTK_H
#define HTK_H

#include <inttypes.h>
#include "Arduino.h"

// 1 frame = 9.504ms
// t_LED (LED Frame time) 7.6~11.4ms
// This should only be 10ms (per spec), but this value works empirically
#define HT16K33_KEYTIME 20

#define HT16K33_KEYS 3
#define HT16K33_ROWS 8

class HT16K33
{
  public:
    typedef uint8_t htaddr;
    enum Error {
      NOERROR = 0,
      TOOMUCH = 1,
      ADDR_NACK = 2,
      DATA_NACK = 3,
      OTHER = 4
    };
    enum Blink {
      NOBLINK = 0b000,
      TWOHZ = 0b010,
      ONEHZ = 0b100,
      HALFHZ = 0b110
    };
      
    union keystruct {
      uint16_t keys[3];
      struct {
        uint16_t KS0;
        uint16_t KS1;
        uint16_t KS2;
      };
    };
    
    struct dispstruct {
      uint16_t rows[HT16K33_ROWS];
    };

  private:
    uint8_t _addr;
    unsigned long _nextkey;
  
  public:
    Error err;
    HT16K33(uint8_t);
    void begin();
    void command(htaddr);
    // Stream writing
    void beginWrite(htaddr);
    void write(uint8_t);
    void write(const uint8_t*, size_t);
    void endWrite(void);
    // One-off writing
    void write(htaddr, uint8_t);
    void write(htaddr, const uint8_t*, size_t);
    // Stream reading
    void beginRead(htaddr, uint8_t);
    bool available(void);
    uint8_t read(void);
    inline void endRead(void) {}; // Does nothing, but helps clarify client
    // One-off reading
    uint8_t read(htaddr);
    
    // Special commands
    void setup(bool);
    void rowint(bool, bool=false);
    void display_setup(bool, Blink=NOBLINK);
    void display(const dispstruct*);
    void dimming(uint8_t);
    bool interrupt(void);
    keystruct keydata(void);
    inline bool keyready(void) {return millis() >= _nextkey || interrupt();};
};

#endif
