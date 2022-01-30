#include "definitions.h"
#include <Arduino.h>
#include <string.h>




void initSerial();

void parseOSCMessage(String& msg);

void sendOscMessage(const String &address, float value);

void sendWheelMove(WHEEL_TYPE type, float ticks);

void checkOSC();
