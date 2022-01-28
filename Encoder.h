#pragma once
#include <Arduino.h>

#define FORWARD             0
#define REVERSE             1


enum WHEEL_TYPE { TILT, PAN, LEVEL, RED, GREEN, BLUE};



class Encoder
{
public:
    Encoder(uint8_t pinA, uint8_t pinB, uint8_t direction, WHEEL_TYPE, uint8_t btnPin = -1);
    int8_t updateEncoder();
    int8_t updateButton();
private:
    WHEEL_TYPE type; 
    uint8_t direction;
    uint8_t btnPin;


    uint8_t pinA;
    uint8_t pinB;
    
    int pinAPrevious;
    int pinBPrevious;
    float pos;
    
    bool btnPrevious;
    bool btnCurState;
    
    bool fine;
 

};
