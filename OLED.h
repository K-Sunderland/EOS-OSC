#include <Arduino.h>
#include <OneBitDisplay.h>



#pragma once


class OLED 
{
public:

void initOled(int sdaPin);
void displaySplash(uint8_t * img);

private:
	OBDISP disp; 
  int sclPin = 21;
  
  

};
