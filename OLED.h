#pragma once
#include <Arduino.h>
#include <OneBitDisplay.h>
#include <string.h>


#pragma once


class OLED 
{
public:

void initOled(int sdaPin);
void displaySplash(uint8_t * img);
void displayText(char * text, int x, int y);
void clearDisplay();
void power(bool state);
private:
	OBDISP disp; 
  int sclPin = 21;
  
  

};

char * convertToChar(int input);
