#include <Arduino.h>
#ifndef BUTTON_H
#define BUTTON_H

enum buttonType {BUMP_GO, BUMP_BACK, BLACKOUT, LOAD, MASTER_GO, MASTER_BACK };

class Button
{
public: 
  void init(uint8_t upin, buttonType utype, uint8_t faderNum = -1); 
  void updateButton(); 

private: 
  uint8_t faderNum;
  uint8_t pin;
  int previous; 
  buttonType type; 
};



#endif
