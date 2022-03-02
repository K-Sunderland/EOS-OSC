#include <Arduino.h>
#ifndef BUTTON_H
#define BUTTON_H

enum buttonType {BUMP_GO, BUMP_BACK, MASTER_LOAD, MASTER_GO, MASTER_BACK, FADER_PAGE };

class Button
{
public: 
  void init(uint8_t upin, buttonType utype, uint8_t faderNum = -1, int pageNum = -1, uint8_t ledNum = -1; 
  void updateButton(); 

private: 
  uint8_t faderNum;
  int faderPage; 
  uint8_t ledNum;
  uint8_t pin;
  int previous; 
  buttonType type; 
};



#endif
