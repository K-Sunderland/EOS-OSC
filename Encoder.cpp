#include "Encoder.h"
#include "OSC.h"


void Encoder::initEncoder(uint8_t upinA, uint8_t upinB, uint8_t ubtnPin, uint8_t udirection, WHEEL_TYPE utype, uint8_t uscale)
{
  pinA = upinA;
  pinB = upinB;
  direction = udirection;
  scale = uscale;
  type = utype;

  btnPin = ubtnPin;

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  pinAPrevious = digitalRead(pinA);
  pinBPrevious = digitalRead(pinB);
  if (btnPin != -1)
  {
    pinMode(btnPin, INPUT_PULLUP);
    btnPin = ubtnPin;
    btnPrevious = digitalRead(btnPin);
  }
}

int8_t Encoder::updateEncoder()
{
    int8_t encoderMotion = 0;
    int pinACurrent = digitalRead(pinA); 
    int pinBCurrent = digitalRead(pinB);

    // has the encoder moved at all?
    if (pinAPrevious != pinACurrent)
    {
      
        // Since it has moved, we must determine if the encoder has moved forwards or backwards
        encoderMotion = (pinAPrevious == pinBPrevious) ? -1 : 1;

        // If we are in reverse mode, flip the direction of the encoder motion
        if (direction == REVERSE)
            encoderMotion = -encoderMotion;
    }
    pinAPrevious = pinACurrent;
    pinBPrevious = pinBCurrent;

  encoderMotion *= scale; 
    if(type != SELECTOR)
    {
      if(encoderMotion != 0)
      {
         sendWheelMove(type, encoderMotion);     
         return 0;
      }
    }
    else
    {

      return encoderMotion;
    }
    }
    
WHEEL_TYPE Encoder::getType()
{
  return type;  
}

bool Encoder::updateButton()
{
    bool state;
    btnCurState = digitalRead(btnPin);

    if(btnCurState != btnPrevious && btnCurState == 1)
    {
    state = true; 

    }
    else
    {
    state = false;  
    }
    btnPrevious = btnCurState;
    return state;
}

void Encoder::changeType(WHEEL_TYPE newType)
{
    type = newType;
}
