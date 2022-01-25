#include <Arduino.h>
#include <OneBitDisplay.h>



#pragma once




class OLED 
{
public:
	OLED(int sdaPin, int sclPin);
	void displaySplash();
 
  OBDISP * getDisplay();

  
private:
	OBDISP disp; 

  

};
