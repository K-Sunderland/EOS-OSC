#include "Encoder.h"

Encoder::Encoder(uint8_t pinA, uint8_t pinB, uint8_t direction, WHEEL_TYPE, uint8_t btnPin)
{
  
}

int8_t Encoder::updateButton()
{

    return digitalRead(btnPin);
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
