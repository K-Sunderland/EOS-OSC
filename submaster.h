#ifndef SUBMASTER_H
#define SUBMASTER_H

#include <Arduino.h>

class Sub 
{


public: 
void init(uint8_t upin);
float updateSub(); 

private:
   uint8_t sensorPin;      
   int sensorValue;   
   float EMA_a;      
   float EMA_S;         
};

#endif
