#include "OLED.h"

void OLED::initOled(int sdaPin)
{


  obdI2CInit(&disp, OLED_128x64, -1, 0,0,0, sdaPin,sclPin,-1,400000L);
  obdFill(&disp, 0,1);
}

void OLED::displaySplash(uint8_t * img)
{
  obdLoadBMP(&disp, img, 0,0,1);
  
}

void OLED::displayText(char * text, int x, int y)
{
   obdWriteString(&disp, 0,x,y,text, FONT_8x8, 0, 1);
}

void OLED::clearDisplay()
{
  obdFill(&disp, 0,1);  
}


void OLED::power(bool state)
{
obdPower(&disp, state);  
  
}
char * convertToChar(int input)
{
String displayText = "";
char charBuf[50];
displayText += input;
displayText.toCharArray(charBuf, 50);

return charBuf;
}
