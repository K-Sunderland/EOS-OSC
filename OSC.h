#ifndef OSC_H
#define OSC_H

#include "encoder.h"
#include <Arduino.h>
#include <string.h>

enum ConsoleType
{
  ConsoleNone,
  ConsoleEos
};

extern ConsoleType connectedToConsole;
extern bool connected; 





void initSerial();



void parseOSCMessage(String& msg);

void sendOscMessage(const String &address, float value);

void sendWheelMove(WHEEL_TYPE type, float ticks);

void checkOSC();

#endif
