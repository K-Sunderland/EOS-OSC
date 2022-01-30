#include <Arduino.h>
#include <string.h>
#include "Encoder.h"



void initSerial();

void parseOSCMessage(String& msg);

void sendOscMessage(const String &address, float value);

void sendWheelMove(WHEEL_TYPE type, float ticks);

void checkOSC();
