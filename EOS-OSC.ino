#include <OneBitDisplay.h>
#include "OLED.h"
#include "Encoder.h"
#include "OSCMain.cpp"



enum class State {Initialize, Run, Switch, Update};

OLED displays[6];
Encoder test;

int sdaPins[] = {22,23,24,25,26,27};
int val = 0;

 

const uint8_t etcSplash[] PROGMEM = {
      0x42,0x4d,0x40,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0x00,0x00,0x00,0x28,0x00,
      0x00,0x00,0x80,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,
      0x00,0x00,0x02,0x04,0x00,0x00,0x12,0x0b,0x00,0x00,0x12,0x0b,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
      0xff,0xf7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xcf,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xde,0x3b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0x79,0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xce,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xfe,0x33,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0xff,0xff,0xff,0x73,0x39,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0xcf,0x7f,0xff,0xff,0xff,0xbb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xde,0x39,0xde,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0x79,0xce,0x37,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xcc,0x33,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xfe,0x73,0x39,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0xff,0xff,0xff,0xff,0xf9,0x9b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xe7,0xcf,0x7f,0xff,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xce,0x31,0xcc,0x33,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xf9,0xc4,0x23,0x3f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xce,0x31,0x19,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0x73,0xbb,0x9b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xe7,0xce,0x39,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0xce,0x30,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0x79,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xdf,0x79,0xf0,0x00,0x1f,0xfe,0x3f,0xff,0x00,0x7f,0xff,0xff,0xff,0xff,
      0xff,0xe7,0x8e,0x30,0xf8,0x00,0x0f,0xfe,0x3f,0xfc,0x00,0x3f,0xff,0xff,0xff,0xff,
      0xff,0xf7,0x8c,0x30,0xfc,0x00,0x07,0xfe,0x3f,0xf0,0x00,0x1f,0xff,0xff,0xff,0xff,
      0xff,0xff,0xde,0x30,0xff,0xff,0xff,0xfe,0x3f,0xe0,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x3f,0xc3,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0x79,0xff,0xff,0xff,0xfe,0x3f,0xc7,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0x8e,0x38,0xff,0xff,0xff,0xfe,0x3f,0xc7,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xe3,0x04,0x10,0xf0,0x00,0x1f,0xfe,0x3f,0xc7,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xe3,0x8c,0x10,0xf8,0x00,0x0f,0xfe,0x3f,0xc7,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0x8e,0x30,0xfc,0x00,0x07,0xfe,0x3f,0xc7,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0x79,0xff,0xff,0xff,0xfe,0x3f,0xc7,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x3f,0xc3,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xde,0x30,0xff,0xff,0xff,0xfe,0x3f,0xe1,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xe7,0x8c,0x30,0xf0,0x00,0x1f,0x00,0x00,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xe7,0x8e,0x31,0xf8,0x00,0x0f,0x80,0x00,0x78,0x00,0x7f,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0x79,0xfc,0x00,0x07,0xc0,0x00,0x3c,0x00,0x3f,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x1f,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0xce,0x30,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xe7,0xdf,0x79,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xfe,0x73,0xb9,0x9b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xcc,0x21,0x19,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0x79,0xc4,0x21,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xde,0x31,0xc4,0x33,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xe7,0xdf,0x7f,0xfe,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0xff,0xff,0xff,0xff,0xb9,0x9b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xfe,0x73,0x39,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xcc,0x33,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0x79,0xcc,0x37,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xde,0x39,0xce,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0xcf,0x7f,0xff,0xff,0xff,0xbb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xfe,0x73,0x39,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xce,0x73,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xce,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0x79,0xcf,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xde,0x39,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xf7,0xcf,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00 };


void setup()
{
  for(uint8_t i =0; i <6; i++)
  {
    displays[i].initOled(sdaPins[i]);
  }
  test.initEncoder(PAN, 30,31, FORWARD, 32);
}

void loop()
{
if(test.updateEncoder() != 0)
{
  displays[0].displayText((char*)"Hello");
  displays[0].clearDisplay;
}
}
