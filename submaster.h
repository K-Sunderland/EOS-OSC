#ifndef SUBMASTER_H
#define SUBMASTER_H

#include <Arduino.h>

class Sub 
{


public: 
void init(uint8_t upin);
void updateSub(); 

private:
   
   uint8_t sensorPin;      
   int sensorValue;   
   float EMA_a;      
   int EMA_S;         
   int previousValue;

};

#endif
