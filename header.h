#ifndef HEADER_H
#define HEADER_H

//includes
//#include <FFT.h>
#include <SPI.h>

//some nop defines
#define _NOP_ "nop\n"
#define NOP    __asm__ __volatile__ (_NOP_)

//
#define ANODE_LEVELS 8   //currently only supported up to 8 anode layers, untested anything lower than 8 but should technically work...maybe with a tweak or two
#define REFRESH_RATE 800 //800 refresh rate / 15 cycles for 4-bit color = ~53.3hz per full cube cycle, lowest you should go is ~700

//color codes
#define RED   2
#define GREEN 1
#define BLUE  0

//color packing/unpacking
#define packColor(r, g, b)    ((r & 0xF) | ((g & 0xF) << 4) | ((b & 0xF) << 8))
#define getRed(packedColor)    (packedColor       & 0xF)
#define getGreen(packedColor) ((packedColor >> 4) & 0xF)
#define getBlue(packedColor)  ((packedColor >> 8) & 0xF)

//preset colors, 'br' is brightness (0-15)
#define C_BLACK(br)   packColor(0,  0,  0 )
#define C_WHITE(br)   packColor(br, br, br)
#define C_RED(br)     packColor(br, 0,  0 )
#define C_ORANGE(br)  packColor(br, br >> 1, 0)
#define C_YELLOW(br)  packColor(br, br, 0 )
#define C_LIME(br)    packColor(br >> 1, br, 0)
#define C_GREEN(br)   packColor(0,  br, 0 )
#define C_SPRING(br)  packColor(0, br, br >> 1)
#define C_CYAN(br)    packColor(0,  br, br)
#define C_DODGER(br)  packColor(0, br >> 1, br)
#define C_BLUE(br)    packColor(0,  0,  br)
#define C_INDIGO(br)  packColor(br >> 1, 0, br)
#define C_MAGENTA(br) packColor(br, 0,  br)
#define C_PINK(br)    packColor(br, 0, br >> 1)

//color structure, unused ):
struct Color {
  byte red   = 0;
  byte green = 0;
  byte blue  = 0;
};

// Define various ADC prescalers (slight loss in precision)
const unsigned char PS_16  = (1 << ADPS2);
const unsigned char PS_32  = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64  = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //default

//pin constants
const byte dataPin  = 11;       //THIS PIN NUMBER CANNOT BE CHANGED, IT IS HARDWARE DEDICATED TO SPI
const byte clockPin = 13;       //THIS PIN NUMBER CANNOT BE CHANGED, IT IS HARDWARE DEDICATED TO SPI
const byte latchPin = 4;        //this one can be any pin
const byte latchBit = (1 << 4); //SS (RCK), pin 4 (which is latchPin) is port D4, or B00010000

//interrupt variables
volatile byte anodeLevel = 0;               //used inside ISR
volatile byte bamCounter = 0;               //used inside ISR
volatile byte bamBit     = 0;               //used inside ISR
volatile byte led[sq(ANODE_LEVELS) * 3][4]; //used inside ISR, holds each led info [location][brightness], each bit is an LED

//animation variables
const unsigned long animationLength = 1000 * (30 /*seconds*/); // x second(s) per animation
unsigned long animationTimerStart   = 0; //keeps track of when an animation started
byte lastAnimation                  = 0; //previous animation

//CubeFunctions.h
void setLED        (byte x, byte y, byte z, byte r, byte g, byte b);
void setLEDcolor   (byte x, byte y, byte z, byte c, byte b);
uint16_t getLED    (byte x, byte y, byte z);
byte getLEDcolor   (byte x, byte y, byte z, byte c);
void setMatrix     (byte r, byte g, byte b);
void clearMatrix   ();
void setPlane      (byte axis, byte layer, byte r, byte g, byte b);
void clearLayer    (byte layer);
void setRowX       (byte row, byte layer, byte r, byte g, byte b);
void setRowZ       (byte row, byte layer, byte r, byte g, byte b);
void setColumn     (byte x, byte z, byte r, byte g, byte b);
void setRefreshRate(int updatesPerSecond);
uint16_t blend     (byte r1, byte g1, byte b1, byte r2, byte g2, byte b2, byte p);
uint16_t blendp    (uint16_t c1, uint16_t c2, byte p);

#endif
