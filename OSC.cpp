#include "OSC.h";
#include <OSCBoards.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <OSCMatch.h>
#include <OSCMessage.h>
#include <OSCTiming.h>
#ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial(thisBoardsSerialUSB);
#else
#include <SLIPEncodedSerial.h>
SLIPEncodedSerial SLIPSerial(Serial);
#endif

#include <string.h>


#define FORWARD             0
#define REVERSE             1

ConsoleType connectedToConsole = ConsoleEos;
bool connected = false;


#define BOX_NAME_STRING     "facepanel"

const String HANDSHAKE_QUERY = "ETCOSC?";
const String HANDSHAKE_REPLY = "OK";
unsigned long lastMessageRxTime = 0;
bool timeoutPingSent = false;

#define PING_AFTER_IDLE_INTERVAL    2500
#define TIMEOUT_AFTER_IDLE_INTERVAL 5000






void initSerial()
{
    SLIPSerial.begin(115200);  



}
void parseOSCMessage(String& msg)
{
  // check to see if this is the handshake string
  if (msg.indexOf(HANDSHAKE_QUERY) != -1)
  {
    // handshake string found!
    SLIPSerial.beginPacket();
    SLIPSerial.write((const uint8_t*)HANDSHAKE_REPLY.c_str(), (size_t)HANDSHAKE_REPLY.length());
    SLIPSerial.endPacket();
    connected = true;
    connectedToConsole = ConsoleEos;
  }
 

}

void sendOscMessage(const String &address, float value)
{
  OSCMessage msg(address.c_str());
  msg.add(value);
  SLIPSerial.beginPacket();
  msg.send(SLIPSerial);
  SLIPSerial.endPacket();
}


void sendWheelMove(WHEEL_TYPE type, float ticks)
{
  String wheelMsg("/eos/wheel");
  wheelMsg.concat("/coarse");

  switch (type)
  {
    case PAN:
      wheelMsg.concat("/pan");
      break;
      
    case TILT:
      wheelMsg.concat("/tilt");
      break;
      
    case LEVEL:
      wheelMsg.concat("/level");
      break;
      
    case RED:
      wheelMsg.concat("/red");
      break;
      
    case GREEN:
      wheelMsg.concat("/green");
      break;
      
    case BLUE:
      wheelMsg.concat("/blue");
      break;
    case ZOOM:
      wheelMsg.concat("/zoom");
      break;
    default:
      return;
      break;
  }


  sendOscMessage(wheelMsg, ticks);
}

void checkOSC()
{
  static String curMsg;
  int size = SLIPSerial.available();
  if (size > 0)
  {
    // Fill the msg with all of the available bytes
    while (size--)
      curMsg += (char)(SLIPSerial.read());
  }
  
  if (SLIPSerial.endofPacket())
  {
    parseOSCMessage(curMsg);
    lastMessageRxTime = millis();
    // We only care about the ping if we haven't heard recently
    // Clear flag when we get any traffic
    timeoutPingSent = false;
    curMsg = String();
  }
  
  if (lastMessageRxTime > 0)
  {
    unsigned long diff = millis() - lastMessageRxTime;
    //We first check if it's been too long and we need to time out
    if (diff > TIMEOUT_AFTER_IDLE_INTERVAL)
    {
      connectedToConsole = ConsoleNone;
      lastMessageRxTime = 0;
      timeoutPingSent = false;
    }

    //It could be the console is sitting idle. Send a ping once to
    // double check that it's still there, but only once after 2.5s have passed
    if (!timeoutPingSent && diff > PING_AFTER_IDLE_INTERVAL)
    {
      OSCMessage ping("/eos/ping");
      ping.add(BOX_NAME_STRING "_hello"); // This way we know who is sending the ping
      SLIPSerial.beginPacket();
      ping.send(SLIPSerial);
      SLIPSerial.endPacket();
      timeoutPingSent = true;
    }
  }  
}
