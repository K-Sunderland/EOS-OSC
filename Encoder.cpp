#include "Encoder.h"



int8_t Encoder::updateButton()
{

    return digitalRead(btnPin);
}


void Encoder::initEncoder(uint8_t upinA, uint8_t upinB, uint8_t udirection, WHEEL_TYPE utype, uint8_t ubtnPin = 0)
{
  pinA = upinA;
  pinB = upinB;
  pos = 0;
  direction = udirection;
  type = utype;

  btnPin = ubtnPin;

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  pinAPrevious = digitalRead(pinA);
  pinBPrevious = digitalRead(pinB);
  if (btnPin != 0)
  {
    btnPin = btnPin;
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
    return encoderMotion;
}
