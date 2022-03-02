#ifndef SUBMASTER_H
#define SUBMASTER_H

#include <Arduino.h>
#include <string.h>

enum faderType {FADER, GRANDMASTER, MASTER_LEFT, MASTER_RIGHT};

class Sub 
{


public: 
void init(uint8_t upin, faderType utype, uint8_t ufaderNum = -1);
void updateSub(); 

private:
   faderType type; 
   uint8_t faderNum; 
   uint8_t sensorPin;      
   int sensorValue;   
   float EMA_a;      
   int EMA_S;         
   int previousValue;

};

extern int faderPage; 

#endif
