#include "submaster.h"
#include "OSC.h" 

void Sub::init(uint8_t upin)
{
  sensorPin = upin;
  pinMode(upin, INPUT);

  sensorValue = 0;    //initialization of sensor variable, equivalent to EMA Y
  EMA_a = 0.2;      //initialization of EMA alpha
  EMA_S = analogRead(sensorPin);  //set EMA S for t=1
  previousValue = EMA_S; 
}

void Sub::updateSub()
{
  sensorValue = analogRead(sensorPin);                //read the sensor value using ADC
  EMA_S = (EMA_a * sensorValue) + ((1 - EMA_a) * EMA_S); //run the EMA
    
  if(EMA_S != previousValue)
  {
    
     sendSubLevel(1, EMA_S/1023);  

 
  }

  previousValue = EMA_S;
}
