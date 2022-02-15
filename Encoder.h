#pragma once 
#include <Arduino.h>


#define FORWARD             0
#define REVERSE             1

//#ifndef ENCODER_H
//#define ENCODER_H 


 enum WHEEL_TYPE { TILT, PAN, LEVEL, RED, GREEN, BLUE, WHITE, AMBER, ZOOM, SELECTOR};





class Encoder
{
  
public:
    void initEncoder(uint8_t upinA, uint8_t upinB, uint8_t ubtnPin, uint8_t udirection, WHEEL_TYPE utype, uint8_t uscale);
    int8_t updateEncoder();
    WHEEL_TYPE getType();
    bool updateButton();
    void changeType(WHEEL_TYPE newType);
  

private:
    WHEEL_TYPE type; 
    uint8_t direction;
    uint8_t btnPin;
    uint8_t scale;

    uint8_t pinA;
    uint8_t pinB;
    
    int pinAPrevious;
    int pinBPrevious;

    
    uint8_t btnPrevious;
    uint8_t btnCurState;
    
    bool fine;
 

};
//#endif
