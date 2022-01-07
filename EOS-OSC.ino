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

#define PAN_DIR             FORWARD
#define TILT_DIR            FORWARD
#define LEVEL_DIR           REVERSE

#define PAN_SCALE           1
#define TILT_SCALE          1
#define LEVEL_SCALE         1

#define VERSION_STRING      "2.0.0.1"

#define PING_AFTER_IDLE_INTERVAL    2500
#define TIMEOUT_AFTER_IDLE_INTERVAL 5000


const String HANDSHAKE_QUERY = "ETCOSC?";
const String HANDSHAKE_REPLY = "OK";

enum WHEEL_TYPE { TILT, PAN, LEVEL};
enum WHEEL_MODE { COARSE, FINE };

struct Encoder
{
  uint8_t pinA;
  uint8_t pinB;
  int pinAPrevious;
  int pinBPrevious;
  float pos;
  bool fine;
  uint8_t direction;
};
struct Encoder panWheel;
struct Encoder tiltWheel;
struct Encoder levelWheel;
enum ConsoleType
{
  ConsoleNone,
  ConsoleEos,
  ConsoleCobalt,
  ConsoleColorSource
};

ConsoleType connectedToConsole = ConsoleEos;
unsigned long lastMessageRxTime = 0;
bool timeoutPingSent = false;


void parseOSCMessage(String& msg)
{
  // check to see if this is the handshake string
  if (msg.indexOf(HANDSHAKE_QUERY) != -1)
  {
    // handshake string found!
    SLIPSerial.beginPacket();
    SLIPSerial.write((const uint8_t*)HANDSHAKE_REPLY.c_str(), (size_t)HANDSHAKE_REPLY.length());
    SLIPSerial.endPacket();

  }
}


void initEncoder(struct Encoder* encoder, uint8_t pinA, uint8_t pinB, uint8_t direction)
{
  encoder->pinA = pinA;
  encoder->pinB = pinB;
  encoder->pos = 0;
  encoder->direction = direction;

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  encoder->pinAPrevious = digitalRead(pinA);
  encoder->pinBPrevious = digitalRead(pinB);

  encoder-> fine = false;
}



int8_t updateEncoder(struct Encoder* encoder)
{
  int8_t encoderMotion = 0;
  int pinACurrent = digitalRead(encoder->pinA);
  int pinBCurrent = digitalRead(encoder->pinB);

  // has the encoder moved at all?
  if (encoder->pinAPrevious != pinACurrent)
  {
    // Since it has moved, we must determine if the encoder has moved forwards or backwards
    encoderMotion = (encoder->pinAPrevious == encoder->pinBPrevious) ? -1 : 1;

    // If we are in reverse mode, flip the direction of the encoder motion
    if (encoder->direction == REVERSE)
      encoderMotion = -encoderMotion;
  }
  encoder->pinAPrevious = pinACurrent;
  encoder->pinBPrevious = pinBCurrent;
  Serial.begin(9600);
  Serial.print(encoderMotion);
  Serial.end();
  return encoderMotion;
}



void sendOscMessage(const String &address, float value)
{
  OSCMessage msg(address.c_str());
  msg.add(value);
  SLIPSerial.beginPacket();
  msg.send(SLIPSerial);
  SLIPSerial.endPacket();
}



void sendEosWheelMove(WHEEL_TYPE type, float ticks)
{
  String wheelMsg("/eos/wheel");


  // This is code to enable fine mode when shift (hardlined) is pressed
  // Disabled until keyboard solution can be created
  /*
  if (digitalRead(SHIFT_BTN) == LOW)
    wheelMsg.concat("/fine");
  else
    wheelMsg.concat("/coarse");
  */
  
  // debug 
  Serial.begin(9600);
  if (type == PAN)
  {
    wheelMsg.concat("/fine/pan");
    Serial.print("pan");
  }
  else if (type == TILT)
  {
    wheelMsg.concat("/fine/tilt");
    Serial.print("tilt");
  }
  else if (type == LEVEL)
  {
    wheelMsg.concat("/level");
    Serial.print("level");
  }
  else
  {
    // something has gone very wrong
    return;
  }
   Serial.end();
  sendOscMessage(wheelMsg, ticks);
}

void sendWheelMove(WHEEL_TYPE type, float ticks)
{
  sendEosWheelMove(type, ticks); 
}

void setup() 
{
    SLIPSerial.begin(115200);
    //This is a hack around an Arduino bug. It was taken from the OSC library
    //examples
    #ifdef BOARD_HAS_USB_SERIAL
      while (!SerialUSB)
    #else 
      while (!Serial);
    #endif

     // This is necessary for reconnecting a device because it needs some time
    // for the serial port to open. The handshake message may have been sent
    // from the console before #lighthack was ready

    SLIPSerial.beginPacket();
    SLIPSerial.write((const uint8_t*)HANDSHAKE_REPLY.c_str(), (size_t)HANDSHAKE_REPLY.length());
    SLIPSerial.endPacket();

    initEncoder(&panWheel, A0, A1, PAN_DIR);
    initEncoder(&tiltWheel, A2, A3, TILT_DIR);
    initEncoder(&levelWheel, A4, A5, LEVEL_DIR);
}

void loop() 
{
  static String curMsg;
  int size;

  int32_t panMotion = updateEncoder(&panWheel);
  int32_t tiltMotion = updateEncoder(&tiltWheel);
  int32_t levelMotion = updateEncoder(&levelWheel);

  panMotion *= PAN_SCALE;
  tiltMotion *= TILT_SCALE;
  levelMotion *= LEVEL_SCALE;

  if (panMotion != 0)
    sendWheelMove(PAN, panMotion);
  if (tiltMotion != 0)
    sendWheelMove(TILT, tiltMotion);
  if (levelMotion != 0)
    sendWheelMove(LEVEL, tiltMotion);

}
