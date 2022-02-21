#include "submaster.h"
#include "OSC.h" 

void Sub::init(uint8_t upin, faderType utype, uint8_t ufaderNum = -1)
{
  sensorPin = upin;
  pinMode(upin, INPUT);

  sensorValue = 0;    //initialization of sensor variable, equivalent to EMA Y
  EMA_a = 0.2;      //initialization of EMA alpha
  EMA_S = analogRead(sensorPin);  //set EMA S for t=1
  previousValue = EMA_S; 

  type = utype; 
  faderNum = ufaderNum;  
  
  
  
}

void Sub::updateSub()
{
  
  sensorValue = analogRead(sensorPin);                //read the sensor value using ADC
  EMA_S = (EMA_a * sensorValue) + ((1 - EMA_a) * EMA_S); //run the EMA

  if(EMA_S != previousValue)
  {  
   float value = float(EMA_S/1017);
    //add paging ability in future
    switch(type)
    {
      case FADER:
      
      String faderMsg("/eos/fader/1/"); 
      faderMsg.concat(faderNum);
      sendOscMessage(faderMsg, value); 
      
      break;
      
    }
 
  }


  
  previousValue = EMA_S;
}
