#include <Arduino.h>
#ifndef BUTTON_H
#define BUTTON_H

enum buttonType {BUMP_GO, BUMP_BACK, MASTER_LOAD, MASTER_GO, MASTER_BACK, FADER_PAGE };

class Button
{
public: 
  void init(uint8_t upin, buttonType utype, uint8_t faderNum = -1, uint8_t uledPin = -1); 
  void updateButton(); 
  void updateLed(); 

private: 
  uint8_t faderNum;
  uint8_t ledPin;
  uint8_t pin;
  int previous; 
  buttonType type; 
};



#endif
