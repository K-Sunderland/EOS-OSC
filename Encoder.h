#include <Arduino.h>


#define FORWARD             0
#define REVERSE             1

#ifndef ENCODER_H
#define ENCODER_H 


 enum WHEEL_TYPE { TILT, PAN, LEVEL, RED, GREEN, BLUE, ZOOM};





class Encoder
{
  
public:
    void initEncoder(uint8_t pinA, uint8_t pinB, uint8_t direction, WHEEL_TYPE, uint8_t btnPin = 0);
    void updateEncoder();


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
#endif
