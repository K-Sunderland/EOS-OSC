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

// Change these values to switch which direction increase/decrease pan/tilt
#define PAN_DIR             FORWARD
#define TILT_DIR            FORWARD
#define LEVEL_DIR       REVERSE

// Use these values to make the encoder more coarse or fine.
// This controls the number of wheel "ticks" the device sends to the console
// for each tick of the encoder. 1 is the default and the most fine setting.
// Must be an integer.
#define PAN_SCALE           10
#define TILT_SCALE          10
#define LEVEL_SCALE     3


#define BOX_NAME_STRING     "box1"

const String HANDSHAKE_QUERY = "ETCOSC?";
const String HANDSHAKE_REPLY = "OK";
unsigned long lastMessageRxTime = 0;
bool timeoutPingSent = false;

// Change these values to alter how long we wait before sending an OSC ping
// to see if Eos is still there, and then finally how long before we
// disconnect and show the splash screen
// Values are in milliseconds
#define PING_AFTER_IDLE_INTERVAL    2500
#define TIMEOUT_AFTER_IDLE_INTERVAL 5000

enum WHEEL_TYPE { TILT, PAN, LEVEL, RED, GREEN, BLUE};
enum WHEEL_MODE { COARSE, FINE };

struct Encoder
{
  uint8_t pinA;
  uint8_t pinB;
  int pinAPrevious;
  int pinBPrevious;
  float pos;
  uint8_t direction;

  uint8_t btnPin;
  bool btnPrevious;
  bool btnCurState;
  bool fine;


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


void initEncoder(struct Encoder* encoder, uint8_t pinA, uint8_t pinB, uint8_t direction, uint8_t btnPin = 0)
{
  encoder->pinA = pinA;
  encoder->pinB = pinB;
  encoder->pos = 0;
  encoder->direction = direction;


  encoder->btnPin = btnPin;

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  encoder->pinAPrevious = digitalRead(pinA);
  encoder->pinBPrevious = digitalRead(pinB);
  if (btnPin != 0)
  {
    encoder->btnPrevious = digitalRead(btnPin);
  }
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

  return encoderMotion;
}

bool updateButton(struct Encoder* encoder)
{
  int current = digitalRead(encoder->btnPin);

  if(encoder->btnPin != current)
  {
    if(current == 1)
    {
      return true;  
    }  
    else
    {
      return false;
    }
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


void sendWheelMove(WHEEL_TYPE type, float ticks, int fine = 3 )
{
  String wheelMsg("/eos/wheel");
  if (type != LEVEL && fine != 3)
  {
    if (fine == 0)
      wheelMsg.concat("/fine");
    else if (fine == 1)
      wheelMsg.concat("/coarse");

  }

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
      
    default:
      return;
      break;
  }


  sendOscMessage(wheelMsg, ticks);
}




void setup() {
  SLIPSerial.begin(115200);
  // This is a hack around an Arduino bug. It was taken from the OSC library
  //examples
#ifdef BOARD_HAS_USB_SERIAL
  while (!SerialUSB);
#else
  while (!Serial);
#endif

  initEncoder(&panWheel, A0, A1, PAN_DIR, 2);
  initEncoder(&tiltWheel, A2, A3, TILT_DIR, 3);
  initEncoder(&levelWheel, A4, A5, LEVEL_DIR);


}

void loop() {
  static String curMsg;
  int size;
  // get the updated state of each encoder
  int32_t panMotion = updateEncoder(&panWheel);
  bool panFine = updateButton(&panWheel);
  int32_t tiltMotion = updateEncoder(&tiltWheel);
  bool tiltFine = updateButton(&tiltWheel);
  int32_t levelMotion = updateEncoder(&levelWheel);
  

  // Scale the result by a scaling factor
  panMotion *= PAN_SCALE;
  tiltMotion *= TILT_SCALE;
  levelMotion *= LEVEL_SCALE;

  if (tiltMotion != 0)
    sendWheelMove(TILT, tiltMotion, tiltFine);

  if (panMotion != 0)
    sendWheelMove(PAN, panMotion, panFine);

  if (levelMotion != 0)
    sendWheelMove(LEVEL, levelMotion);

  size = SLIPSerial.available();
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
