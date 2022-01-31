#pragma once 
#include <Arduino.h>


#define FORWARD             0
#define REVERSE             1

//#ifndef ENCODER_H
//#define ENCODER_H 


 enum WHEEL_TYPE { TILT, PAN, LEVEL, RED, GREEN, BLUE, ZOOM};





class Encoder
{
  
public:
    void initEncoder(uint8_t upinA, uint8_t upinB, uint8_t ubtnPin, uint8_t udirection, WHEEL_TYPE utype, uint8_t uscale);
    void updateEncoder();


private:
    WHEEL_TYPE type; 
    uint8_t direction;
    uint8_t btnPin;
    uint8_t scale;

    uint8_t pinA;
    uint8_t pinB;
    
    int pinAPrevious;
    int pinBPrevious;

    
    bool btnPrevious;
    bool btnCurState;
    
    bool fine;
 

};
//#endif
