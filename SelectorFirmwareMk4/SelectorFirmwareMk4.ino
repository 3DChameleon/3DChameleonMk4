// November 28, 2024

/* 3DChameleon Mk4.1 Firmware

Copyright 2024 William J. Steele

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the “Software”), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions 
of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.

Single Button Press Commands (count pulses of selector)

#1 - 1
#2 - 2
#3 - 3
#4 - 4
#5 - Home and Load
#6 - Unload Last and Home
#7 - Home
#8 - Next Filament
#9 - Random Filament

Go and edit the top sections of this file to change options and to match your hardware configuration.
There is a section for the screen and/or serial output, along with servo maximum angle on lines 49-52.  
The default pin assignment is for the 3D Chameleon Mk4.1 board, toggled on line 143.
If you are using a different board, you will need to change the pin assignments to match your board.
There are some defaults provided, but the various CNC shield clones have different issues + pinouts.
The AutoClippy is setup on pin 11, if you have a different processor then adjust filamentCutterPin.
It also assumes the servo is 180degress, otherwise adjust line 52 and look at lines 211-220, then test!
*/

#include <SSD1306Ascii.h> //i2C OLED
#include <SSD1306AsciiWire.h> //i2C OLED
#include <SparkFunSX1509.h> // sparkfun i/o expansion board - used for additional filament sensors as well as communications with secondary boards
#include <Wire.h>
#include <SPI.h>
#ifdef ARDUINO_ARCH_ESP32
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define USE_SERIAL 0 // 0 = no serial output, 1 = serial output
#define SERVO_MAX_ANGLE 180
/**
 * Made with Marlin Bitmap Converter
 * https://marlinfw.org/tools/u8glib/converter.html
 *
 * This bitmap from 128x64 pasted image
 */
#pragma once

const unsigned char bitmap_logo[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,B00001100,B00100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111100,B00001110,B00110000,B00011100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B00011111,B01111000,B00011111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B10011111,B11111000,B00011111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B11011111,B11110110,B00011111,B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B10000000,B00000110,B00011111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B11111100,B01111101,B11111111,B11110010,B00111111,B11110000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B11111111,B00001111,B11111111,B11111110,B00111111,B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B00111111,B11111111,B11111111,B00111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01111110,B00111111,B11111111,B11111111,B01111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01111101,B11111111,B11111111,B11111111,B01111111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01111011,B11111111,B11111111,B11111110,B11111111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01100111,B11101111,B11111111,B11111111,B11111111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000111,B10001111,B11110111,B11111111,B11111111,B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001111,B10111111,B11111011,B11111111,B11111101,B11111110,B00011111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001111,B10111111,B11111001,B11111111,B11111111,B11111100,B00001111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001111,B01111111,B11111101,B11111111,B11111111,B11111001,B11110111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000111,B11111111,B11111110,B11111111,B11111111,B11111011,B11110011,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000010,B11111111,B11111110,B11111111,B11111111,B11111011,B10111011,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00111100,B11111111,B11111111,B01111111,B11111111,B11111011,B11110011,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111101,B11111111,B11111111,B01111111,B11111111,B11111001,B11110111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111101,B11111111,B11111111,B10111111,B11111101,B11111100,B11101111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00111101,B11111111,B11111111,B10000000,B00000000,B11111111,B11011111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011001,B11111111,B11111111,B10000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001011,B11111111,B11111111,B01100000,B00000000,B01111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000001,B11111111,B11111110,B11111000,B00000000,B00111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011010,B01111111,B11111101,B11111110,B00000000,B00001111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111011,B11100111,B11111011,B11111110,B00000000,B00000111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B11111011,B11111100,B01110000,B01111111,B00000000,B00000001,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111011,B11111111,B00000000,B01111111,B10000000,B00000000,B01111100,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011011,B11111111,B11000000,B11111111,B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001011,B11111111,B11100001,B11111111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00111011,B11111111,B11100001,B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111011,B11111111,B11100011,B11111111,B11110011,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B11111011,B11111111,B11100000,B00000001,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111011,B11111111,B11100000,B00000000,B01111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000011,B11111111,B11100000,B00000000,B00011110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000101,B11111111,B11100000,B00000000,B00000010,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001101,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011111,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011110,B11111111,B10000000,B00000111,B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000001,B11111111,B01110000,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000011,B01111111,B11111111,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000011,B11101111,B11111111,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000111,B10011111,B11111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00011111,B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01011111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01101111,B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01110111,B11111111,B10000000,B00000000,B01100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000011,B11111111,B11111111,B11111111,B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00001110,B11111111,B11111111,B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00001110,B01111111,B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00001100,B11011111,B11111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11011001,B11111111,B11110000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00011001,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000001,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET  -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C
SSD1306AsciiWire oled;

// define sparkfun io expansion
const byte SX1509_ADDRESS = 0x3E; // SX1509 I2C address
#define SX1509_FILAMENT_0 0
#define SX1509_FILAMENT_1 1
#define SX1509_FILAMENT_2 2
#define SX1509_FILAMENT_3 3
#define SX1509_OUTPUT 4
SX1509 io;                        // Create an SX1509 object to be used throughout

// This "#if 1" means setup pins for the 3d Chameleon PCB (official)
// If you want to use a CNC Shield, change to "#if 0" and check below
#if 1
// defines pins numbers - 3D Chameleon Board

#define extEnable 0
#define extStep 1
#define extDir 2

#define selEnable A0
#define selStep A1
#define selDir A2

#define trigger A3
#define s_limit A4
#define filament A5
#define filamentCutterPin 11


#elif defined(ARDUINO_AVR_UNO)
// Arduino Uno -- CNC Shield V3 - double-check your board's pinout
// A4988 in base X (ext) 
// A4988 in base Y (sel) 
// X.STEP /DR (ext)
// Y.STEP /DR (sel)
// END STOPS Z+ (clippy)
// CoolEn (trigger)
// Resume (s_limit)
// Hold (filament)
// Alternate settings: (shared with screen, but can be used)
// DA (s_limit) is A4
// CL (filament) is A5

#define extEnable 8
#define extStep 2
#define extDir 5

#define selEnable 8
#define selStep 3
#define selDir 6

#define trigger A3
// #define s_limit A4        // pcb labelled DA
// #define filament A5       // pcb labelled CL
#define s_limit A2           // pcb labelled Resume
#define filament A1          // pcb labelled Hold
#define filamentCutterPin 11


#elif defined(ARDUINO_AVR_NANO)
// Nano CNC Shield (aka CNC Shield V4), HW-702 v0.0.0 - ensure follow guide below to correct microstepping pcb traces if needed!
// see https://www.instructables.com/Fix-Cloned-Arduino-NANO-CNC-Shield/#:~:text=Lines%2047%2C48%20%26%2049-,need%20replacing%20with,-%3A

#define extEnable 8
#define extStep 5
#define extDir 2

#define selEnable 8
#define selStep 6
#define selDir 3

#define trigger A3           // pcb labelled Coolant ENable
#define s_limit A2           // pcb labelled Resume
#define filament A1          // pcb labelled Hold
#define filamentCutterPin 11 // pcb labelled Z+/- endstop




#else
#error "UNSUPPORTED BOARD - Add your board definition above this!"

#endif

// This defines servo sweep start and end values, scaled to servo max angle,
// and attempts to use step direction (later on line 841 + 857). Was 136-180.
// Change these values if desired, but probably not necessary unless reversed servo
#define SERVO_START_ANGLE_AS_180_SERVO 136 / (180 / SERVO_MAX_ANGLE)
#define SERVO_END_ANGLE_AS_180_SERVO 180 / (180 / SERVO_MAX_ANGLE)
#if SERVO_START_ANGLE_AS_180_SERVO > SERVO_END_ANGLE_AS_180_SERVO
  #define SERVO_STEP -1
#else
  #define SERVO_STEP 1
#endif

const int counterclockwise = HIGH;
const int clockwise = !counterclockwise;

const int stepsPerRev = 200;
const int microSteps = 16;
const int speedDelay = 170;

const int defaultBackoff = 10;

Servo filamentCutter;  // create servo object to control a servo
int cutterPos = 0;    // variable to store the servo position
bool reverseServo = true;

int currentExtruder = -1;
int nextExtruder = 0;
int lastExtruder = -1;
int tempExtruder = -1;

int seenCommand = 0;
int prevCommand = 0;

int loaderMode = 2;  //(0= direct drive, 1=loader/unloader, 2=loader/unloader with press)

long triggerTime = 300;
long pulseTime = (triggerTime / 2);

long distance = 10;

long unloadDistance = stepsPerRev * microSteps * distance;  // this is 10 revs - about 10"
long loadDistance   = unloadDistance * 1.1;           // this is 11 revs - about 11"

int address = 0;
byte value;

long idleCount = 0;
bool logoActive = false;
bool T0Loaded = false;
bool T1Loaded = false;
bool T2Loaded = false;
bool T3Loaded = false;


bool displayEnabled = false;
bool ioEnabled = false;
//int sensorEnabled = 0;

long randomNumber = 0;

void setup()
{
  if (USE_SERIAL) {
    Serial.begin(115200);
    delay(500);
    Serial.println("Serial Enabled");
  }

  Wire.begin(); //start i2C  
	Wire.setClock(400000L); //set clock

  // set up IO expander
  if (io.begin(SX1509_ADDRESS) == true)
  {
    io.pinMode(SX1509_FILAMENT_0, INPUT_PULLUP);
    io.pinMode(SX1509_FILAMENT_1, INPUT_PULLUP);
    io.pinMode(SX1509_FILAMENT_2, INPUT_PULLUP);
    io.pinMode(SX1509_FILAMENT_3, INPUT_PULLUP);
    io.pinMode(SX1509_OUTPUT, OUTPUT);
    ioEnabled = true;
  }
  if (USE_SERIAL) {
    Serial.print("IO Expander: ");
    Serial.println(ioEnabled);
  }

  // enable OLED display
  oled.begin(&Adafruit128x64, OLED_I2C_ADDRESS);

  // wait for it to start up
  delay(50);
    

  // welcome screen
  oled.setFont(Adafruit5x7);
  oled.clear(); //clear display
	oled.println("");
  oled.println("       Welcome!"); //print a welcome message  
  oled.println("");
  oled.println("    3DChameleon Mk4"); //print a welcome message
  oled.println("");
  oled.println("         Pro");
  delay(3000);

  displayText(0, "       Ready!");
  
  seenCommand = 0;

  // Sets the two pins as Outputs
  pinMode(extEnable, OUTPUT);
  pinMode(extStep, OUTPUT);
  pinMode(extDir, OUTPUT);

  pinMode(selEnable, OUTPUT);
  pinMode(selStep, OUTPUT);
  pinMode(selDir, OUTPUT);

  // set up the button
  pinMode(trigger, INPUT_PULLUP);  // selector

  // a little override here... we're using the two inputs as I2C instead
  pinMode(s_limit, OUTPUT);    
  pinMode(filament, OUTPUT); 

  // lock the selector by energizing it
  digitalWrite(selEnable, HIGH);

  // make sure filament isn't blocked by gillotine
  connectGillotine();
  cutFilament();
  disconnectGillotine();
  
  prevCommand = 0;

}

int lastLoop = 0;

void loop()
{
  static uint32_t lastTime = 0;

  seenCommand = 0;
  idleCount++;

  // process button press
  if (digitalRead(trigger) == 0)
  {
    if (USE_SERIAL) {
      Serial.println("Button Depressed");
    }
    idleCount = 0;
    logoActive = false;
    unsigned long nextPulse;
    unsigned long pulseCount = 0;
    unsigned long commandCount = 0;

    // keep counting (and pulsing) until button is released
    while (digitalRead(trigger) == 0)
    {
      if(pulseCount<pulseTime)
      {
        pulseCount++;
        displayCommand(pulseCount);
        if(pulseCount>1) vibrateMotor();
      }
      delay(400);  // each pulse is 400+ milliseconds apart 
    }
    if (USE_SERIAL) {
      Serial.println("Button Released");
    }
    processCommand(pulseCount); // ok... execute whatever command was caught (by pulse count)
    pulseCount = 0;
  }

  // updates IO block, duh!  No really, grabs the state of the sparkfun gpio expansion
  updateIOBlock();

  // each loop adds 50ms delay, so that gets added AFTER the command is processed before the next one can start
  delay(50);
}

// read the sparkfun SX1509 io
void updateIOBlock()
{
    if(ioEnabled)
    {
      T0Loaded = io.digitalRead(SX1509_FILAMENT_0);
      T1Loaded = io.digitalRead(SX1509_FILAMENT_1);
      T2Loaded = io.digitalRead(SX1509_FILAMENT_2);
      T3Loaded = io.digitalRead(SX1509_FILAMENT_3);
    }
}

// display command for each button pulse
void displayCommand(long commandCount)
{

  switch(commandCount)
  {
  case 2:
    displayText(25, "    Switch to T0");
    break;
  case 3:
    displayText(25, "    Switch to T1");
    break;
  case 4:
    displayText(25, "    Switch to T2");
    break;
  case 5:
    displayText(25, "    Switch to T3");
    break;
  case 6:
    displayText(25, "    Home/Load T0");
    break;
  case 7:
    displayText(28, "    Unload/Home");
    break;
  case 8:
    displayText(50, "        Home");
    break;  
  case 9:
    displayText(50, "        Next");
    break;
  case 10:
    displayText(40, "       Random");
    break;
  default:
    displayText(30, "     No Command");
    break;
  }
}

// execute the pulse count command
void processCommand(long commandCount)
{

  // select case for commands
  switch (commandCount)
  {
  case 2: // unload current, switch to #0, load
    displayText(30, "     T0 Selected");
    currentExtruder = 0;
    processMoves();
    displayText(35, "      Idle - T0");
    break;

  case 3: // unload current, switch to #1, load
    displayText(30, "     T1 Selected");
    currentExtruder = 1;
    processMoves();
    displayText(35, "      Idle - T1");
    break;

  case 4: // unload current, switch to #3, load
    displayText(30, "     T2 Selected");
    currentExtruder = 2;
    processMoves();
    displayText(35, "      Idle - T2");
    break;

  case 5: // unload current, switch to #4, load
    displayText(30, "     T3 Selected");
    currentExtruder = 3;
    processMoves();
    displayText(35, "      Idle - T3");
    break;

  case 6: //home and reload #1
    displayText(40, "      Homing...");
    homeSelector();
    displayText(15, "   Press to Load T0");
    gotoExtruder(0, 0);
    if(loaderMode>0)rotateExtruder(clockwise, loadDistance);
    if(loaderMode>0)gotoExtruder(0, 1);
    currentExtruder = 0;
    lastExtruder = 0;
    displayText(35, "      Idle - T0");
    break;
    
  case 7: // unload current and rehome selector
    displayText(30, "     Cutting...");
    connectGillotine();
    cutFilament();
    switch(lastExtruder)
    {
      case 0:
        displayText(10, "  Press to Unload T0");
        break;
      case 1:
        displayText(10, "  Press to Unload T1");
        break;
      case 2:
        displayText(10, "  Press to Unload T2");
        break;
      case 3:
        displayText(10, "  Press to Unload T3");
        break;
    } 
    if(loaderMode>0)gotoExtruder((lastExtruder==3?2:lastExtruder+1),lastExtruder);
    if(lastExtruder<2)
    {
      if(loaderMode>0)rotateExtruder(counterclockwise, unloadDistance);
    }
    else
    {
      if(loaderMode>0)rotateExtruder(clockwise, unloadDistance);
    }
    disconnectGillotine();
    displayText(50, "        Idle");
    break;

  case 8:  
    displayText(40, "     Homing...");
    homeSelector();
    displayText(50, "        Idle");
    break;

  case 9: // move to next available filament
    displayText(30, "     Cutting...");
    connectGillotine();
    cutFilament();
    displayText(30, "     Next Tool");
    currentExtruder++;
    if(currentExtruder==4)currentExtruder=0;
    processMoves();
    displayText(35, "        Idle");
    break;

  case 10: // move to a random filament
    displayText(30, "     Cutting...");
    connectGillotine();
    cutFilament();
    displayText(30, "    Random Tool");

    // select a random number
    randomNumber = random(0,2) + 1;

    // skip ahead that many tools
    for(long i=0; i<randomNumber; i++)
    {
      currentExtruder++;
      if(currentExtruder==4)currentExtruder=0;
    }
    processMoves();
    displayText(50, "        Idle");
    break;
    
  default:
    displayText(47, "       Clear");
    delay(200);

    displayText(50, "        Idle");
    break;
  }
}

// just the routine to update the OLED
void displayText(int offset, String str)
{
  if (USE_SERIAL) {
    Serial.print(offset);
    Serial.print(": ");
    Serial.println(str);
  } 
  //if(displayEnabled){
    oled.clear();
    oled.println("");
    oled.println("   3DChameleon Mk4"); //print a welcome message
    oled.println("");
    oled.println("");
    oled.println(str);
    oled.println("");
    oled.println("");
    if(ioEnabled)
    {
      oled.print("     ");
      if(T0Loaded)
      {
        oled.print("0  ");
      }
      else
      {
        oled.print("+  ");
      }
      if(T1Loaded)
      {
        oled.print("1  ");
      }
      else
      {
        oled.print("+  ");
      }
      if(T2Loaded)
      {
        oled.print("2  ");
      }
      else
      {
        oled.print("+  ");
      }
      if(T3Loaded)
      {
        oled.println("3");
      }
      else
      {
        oled.println("+");
      }
    }
    else
    {
      oled.println("     -  -  -  -");
    }
  //  }
}

// real work is here
void processMoves()
{

  // make sure we have a real extruder selected
  if(lastExtruder>-1)
  {

    // if so, we need to cut the filament
    displayText(30, "     Cutting...");
    connectGillotine();
    cutFilament();

    // ok... then wait for the 2nd button press to unload it
    switch(lastExtruder)
    {
      case 0:
        displayText(10, "  Press to Unload T0");
        break;
      case 1:
        displayText(10, "  Press to Unload T1");
        break;
      case 2:
        displayText(10, "  Press to Unload T2");
        break;
      case 3:
        displayText(10, "  Press to Unload T3");
        break;
    } 

    // roll over to first if on last
    if( loaderMode>0 ) gotoExtruder( ( lastExtruder==3 ? 2 : (lastExtruder+1)), lastExtruder);

    // this determines which direction to move the motor, 0-1 : counterclockwise, 2-3 : clockwise
    if(lastExtruder<2)
    {
      if(loaderMode>0)rotateExtruder(counterclockwise, unloadDistance);
    }
    else
    {
      if(loaderMode>0)rotateExtruder(clockwise, unloadDistance);
    }
  }
  else
  {
    lastExtruder = 0;
  }
  disconnectGillotine();
  
  // tell it to actually execute that command now
  gotoExtruder(lastExtruder, currentExtruder);

  // ok... filament unloaded, time to load the new... so tell the user
  switch(currentExtruder)
  {
    case 0:
      displayText(15, "   Press to Load T0");
      break;
    case 1:
      displayText(15, "   Press to Load T1");
      break;
    case 2:
      displayText(15, "   Press to Load T2");
      break;
    case 3:
      displayText(15, "   Press to Load T3");
      break;
  }

  // same (but inversed) logic for motor direction
  if(currentExtruder<2)
  {
    if(loaderMode>0)rotateExtruder(clockwise, loadDistance);
  }
  else
  {
    if(loaderMode>0)rotateExtruder(counterclockwise, loadDistance);
  }

  // if we're loading, then load it now
  if(loaderMode>0)gotoExtruder(currentExtruder, (currentExtruder==3?2:currentExtruder+1));

  // everybody remember where we parked!
  lastExtruder = currentExtruder;
}


// this function simply moves from the currentCog to the targetCog is the best way
void gotoExtruder(int currentCog, int targetCog)
{
 
  int newCog = targetCog - currentCog;

  // ok... which way
  int newDirection = counterclockwise;
  if(newCog<0)
  {
    // we need to move the other way
    newDirection = clockwise;

    //and since we know we went too far... let's go the other way in steps as well
    newCog = currentCog - targetCog;
  }

  // if we're already on the current cog, then do nothing
  if(newCog > 0)
  {    
    // advance tool targetCog times
    for(int i=0; i<newCog; i++)
    {
      rotateSelector(newDirection, (stepsPerRev / 4) * microSteps);
    }
  }
}

// move the extruder motor in a specific direction for a specific distance (unless it's a "until button is not pressed")
void rotateExtruder(bool direction, long moveDistance)
{
  // note to bill:  make this acecelerate so it's very fast!!!
  
  digitalWrite(extEnable, LOW);  // lock the motor
  digitalWrite(extDir, direction); // Enables the motor to move in a particular direction
  const int fastSpeed = speedDelay/2; // double time

  // this is depricated right now... might bring it back in the future
  if(loaderMode==1)
  {

    // Makes 50 pulses for making one full cycle rotation
    for (long x = 0; x < (moveDistance-1); x++)
    {
      // this is how we pulse the motor to get it to step
      digitalWrite(extStep, HIGH);
      delayMicroseconds(fastSpeed);
      digitalWrite(extStep, LOW);
      delayMicroseconds(fastSpeed);
    }

  }

  if(loaderMode==2)
  {

   // keep waiting until button is pressed
   while (digitalRead(trigger) != 0)
    {
      delay(50);
    }
    
    // Move while button is pressed
    while (digitalRead(trigger) == 0)
    {

      // this is how we pulse the motor to get it to step
      digitalWrite(extStep, HIGH);
      delayMicroseconds(fastSpeed);
      digitalWrite(extStep, LOW);
      delayMicroseconds(fastSpeed);
    }
  }
  // ok, done pressing button, so make sure we're not energized (high is no, low is yes)
  digitalWrite(extEnable, HIGH);
}

// similar to extruder, but only stepping 50 (of 200) at a time
void rotateSelector(bool direction, int moveDistance)
{

  // while we are at it... can we make this faster using the magic you invented above?
  
  digitalWrite(selEnable, LOW); // lock the selector
  digitalWrite(selDir, direction); // Enables the motor to move in a particular direction

    // Makes 50 pulses for making one full cycle rotation
    for (int x = 0; x < (moveDistance-1); x++)
    {
      digitalWrite(selStep, HIGH);
      delayMicroseconds(speedDelay);
      digitalWrite(selStep, LOW);
      delayMicroseconds(speedDelay);
    }
}

// this cycles the servo between two positions
void cutFilament() {
  digitalWrite(selEnable, LOW); // disable stepper so we have power!
  if(reverseServo==false)
  {
    openGillotine();
    closeGillotine();
  }
  else
  {
    closeGillotine();
    openGillotine();
  }
  digitalWrite(selEnable, HIGH);
}

// enable the servo
void connectGillotine()
{
  filamentCutter.attach(filamentCutterPin);
}

// disable the servo - so it doesn't chatter when not in use
void disconnectGillotine()
{
  filamentCutter.detach();
}

// cycle servo from 135 and 180
void openGillotine()
{
  #if SERVO_STEP <= -1
    for (int pos = SERVO_START_ANGLE_AS_180_SERVO; pos >= SERVO_END_ANGLE_AS_180_SERVO; pos += SERVO_STEP) {
  #else
    for (int pos = SERVO_START_ANGLE_AS_180_SERVO; pos <= SERVO_END_ANGLE_AS_180_SERVO; pos += SERVO_STEP) { // goes from 0 degrees to 180 degrees
  #endif
    // in steps of 1 degree
    filamentCutter.write(pos);              // tell servo to go to position in variable 'pos'
    delayMicroseconds(25000);                       // waits 15ms for the servo to reach the position
  }
  //filamentCutter.write(3.5);       // tell servo to go to position in variable 'pos'
  delay(50);                       // waits 15ms for the servo to reach the position
}

// reverse cycle servo from 180 back to 135
void closeGillotine()
{
#if SERVO_STEP <= -1
  for (int pos = SERVO_END_ANGLE_AS_180_SERVO; pos <= SERVO_START_ANGLE_AS_180_SERVO; pos -= SERVO_STEP) {
#else
  for (int pos = SERVO_END_ANGLE_AS_180_SERVO; pos >= SERVO_START_ANGLE_AS_180_SERVO; pos -= SERVO_STEP) { // goes from 180 degrees to 0 degrees
#endif
    filamentCutter.write(pos);              // tell servo to go to position in variable 'pos'
    delayMicroseconds(25000);                       // waits 15ms for the servo to reach the position
  }
  delay(50);                       // waits 15ms for the servo to reach the position
}

// rotate the selector clockwise too far from 4, so it'll grind on the bump stop
void homeSelector()
{
  // rotate counter clockwise to hard stop
  rotateSelector(clockwise, stepsPerRev * microSteps);

  // move just slightly to extruder 1 (this backs off a little from the hard stop)
  rotateSelector(counterclockwise, defaultBackoff * microSteps);

 currentExtruder = 0;
 lastExtruder = -2;

}

// buzz buzz buzz
void vibrateMotor()
{
  // oscillate selector 1 time
  rotateSelector(clockwise, 2 * 16);
  rotateSelector(!clockwise, 2 * 16);
}
