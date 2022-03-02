#include "button.h"
#include "OSC.h"
#include "submaster.h"
#include <string.h>

void Button::init(uint8_t upin, buttonType utype, uint8_t ufaderNum = -1, uint8_t uledPin = -1)
{

pin = upin; 
type = utype; 

pinMode(pin, INPUT_PULLUP);

previous = digitalRead(pin);  

faderNum = ufaderNum;


ledPin = uledPin;

if (ledPin != -1)
{
 pinMode(ledPin, OUTPUT);
 digitalWrite(ledPin, LOW);  
}

}


void Button::updateButton()
{
 bool state; 
 int curValue = digitalRead(pin);

    if(curValue != previous && curValue == 0)
    {
 
      state = true;
    }
    else
    {
     state = false;  
    }
    
  
previous = curValue;
// if button is pressed 
   if(state)
   {
    String msg("");
    switch(type)
    {
      case BUMP_GO:
          msg.concat("/eos/fader/1/");
          msg.concat(faderNum);
          msg.concat("/fire");
      break;
      case BUMP_BACK:
          msg.concat("/eos/fader/1/");
          msg.concat(faderNum);
          msg.concat("/stop");
      break;

      case MASTER_LOAD:

      break;
      case MASTER_GO:

      break;
      case MASTER_BACK:

      break;
      case FADER_PAGE:
          faderPage = faderNum;
      break;
      
    }
      sendOscMessage(msg);
      msg = "";
   }
  
}


void Button::updateLed()
{
if(faderNum == faderPage)
{
 digitalWrite(ledPin, HIGH);
}  
else
{
  digitalWrite(ledPin, LOW);  
}
  
}
