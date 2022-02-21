#include "button.h"
#include "OSC.h"
#include <string.h>

void Button::init(uint8_t upin, buttonType utype, uint8_t ufaderNum = -1)
{

pin = upin; 
type = utype; 

pinMode(pin, INPUT_PULLUP);

previous = digitalRead(pin);  

faderNum = ufaderNum;

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
      case BLACKOUT:

      break;
      case LOAD:

      break;
      case MASTER_GO:

      break;
      case MASTER_BACK:

      break;
      
    }
      sendOscMessage(msg);
      msg = "";
   }
  
}
