#include <Arduino.h>
#include <OneBitDisplay.h>
#include <string.h>


#pragma once


class OLED 
{
public:

void initOled(int sdaPin);
void displaySplash(uint8_t * img);
void displayText(string text);

private:
	OBDISP disp; 
  int sclPin = 21;
  
  

};
