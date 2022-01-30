#include <Arduino.h>
#include <OneBitDisplay.h>
#include <string.h>


#pragma once


class OLED 
{
public:

void initOled(int sdaPin);
void displaySplash(uint8_t * img);
void displayText(char * text);
void clearDisplay();

private:
	OBDISP disp; 
  int sclPin = 21;
  
  

};

char * convertToChar(int32_t input);
