#include "Encoder.h"



int8_t Encoder::updateButton()
{

    return digitalRead(btnPin);
}


void Encoder::initEncoder(WHEEL_TYPE type, uint8_t pinA, uint8_t pinB, uint8_t direction, uint8_t btnPin = 0)
{
  pinA = pinA;
  pinB = pinB;
  pos = 0;
  direction = direction;


  btnPin = btnPin;

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  pinAPrevious = digitalRead(pinA);
  pinBPrevious = digitalRead(pinB);
  if (btnPin != 0)
  {
    btnPin = btnPin
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
