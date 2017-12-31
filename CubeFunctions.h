#ifndef CUBEFUNCTIONS_H
#define CUBEFUNCTIONS_H

#include "header.h"

//writes a color's brightness values to the led array
#define writeColorToMatrix(ledLoc, bit, val) {    \
  bitWrite(led[ledLoc][0], bit, bitRead(val, 0)); \
  bitWrite(led[ledLoc][1], bit, bitRead(val, 1)); \
  bitWrite(led[ledLoc][2], bit, bitRead(val, 2)); \
  bitWrite(led[ledLoc][3], bit, bitRead(val, 3)); \
}

/*
 * setLED : sets an LED at position <x,y,z> to the RGB color <r,g,b>
 * 
 * @param x,y,z : x,y,z position in matrix
 * @param r,g,b : red,green,blue 4-bit color of LED (accepts 0-15 for each color)
 * @return void
 * 
 * NOTE : will stop program and blink red LED <0,0,0> if out of bounds position error occurs
 */
void setLED(byte x, byte y, byte z, byte r, byte g, byte b) {
  if(x >= ANODE_LEVELS || y >= ANODE_LEVELS || z >= ANODE_LEVELS) { //position error has occurred
    //clearMatrix();
    while(true) {
      setLEDcolor(0, 0, 0, RED, 15);
      delay(1000);
      setLEDcolor(0, 0, 0, RED, 0);
      delay(1000);
    }
  }
  //this function does not check if color is out of bounds, it simply ignores bits 5-8
  
  int locB = (y * ANODE_LEVELS * 3) + (z * 3) + ((x * 3    ) >> 3);
  int locG = (y * ANODE_LEVELS * 3) + (z * 3) + ((x * 3 + 1) >> 3);
  int locR = (y * ANODE_LEVELS * 3) + (z * 3) + ((x * 3 + 2) >> 3);

  writeColorToMatrix(locB, (x * 3    ) % ANODE_LEVELS, b);
  writeColorToMatrix(locG, (x * 3 + 1) % ANODE_LEVELS, g);
  writeColorToMatrix(locR, (x * 3 + 2) % ANODE_LEVELS, r);
}

/*
 * getLED : sets an LED at position <x,y,z> to the RGB color <r,g,b>
 * 
 * @param x,y,z     : x,y,z position in matrix
 * @return uint16_t : a packed color containined red, green, blue values
 */
uint16_t getLED(byte x, byte y, byte z) {
  int locB = (y * ANODE_LEVELS * 3) + (z * 3) + ((x * 3    ) >> 3);
  int locG = (y * ANODE_LEVELS * 3) + (z * 3) + ((x * 3 + 1) >> 3);
  int locR = (y * ANODE_LEVELS * 3) + (z * 3) + ((x * 3 + 2) >> 3);
  
  byte bvB = _BV((x * 3    ) % ANODE_LEVELS); //bit value #, just temp names to save execution time
  byte bvG = _BV((x * 3 + 1) % ANODE_LEVELS);
  byte bvR = _BV((x * 3 + 2) % ANODE_LEVELS);
  
  return
  (( !!(led[locB][0] & bvB) ) << 11) +
  (( !!(led[locB][1] & bvB) ) << 10) +
  (( !!(led[locB][2] & bvB) ) <<  9) +
  (( !!(led[locB][3] & bvB) ) <<  8) +
  (( !!(led[locG][0] & bvG) ) <<  7) +
  (( !!(led[locG][1] & bvG) ) <<  6) +
  (( !!(led[locG][2] & bvG) ) <<  5) +
  (( !!(led[locG][3] & bvG) ) <<  4) +
  (( !!(led[locR][0] & bvR) ) <<  3) +
  (( !!(led[locR][1] & bvR) ) <<  2) +
  (( !!(led[locR][2] & bvR) ) <<  1) +
  (( !!(led[locR][3] & bvR) )      );
}

/*
 * getLEDcolor : gets an LED's color 'c' value at position <x,y,z> 
 * 
 * @param x,y,z : x,y,z position in matrix
 * @param c     : the color to get the value of, color codes defined in header
 * @return byte
 */
byte getLEDcolor(byte x, byte y, byte z, byte c) {
  int loc = (y * ANODE_LEVELS * 3) + (z * 3) + ((x * 3 + c) >> 3);
  
  byte bv = _BV((x * 3 + c) % ANODE_LEVELS); //bit value #, just temp names to save execution time
  
  return
  (( !!(led[loc][0] & bv) ) <<  3) +
  (( !!(led[loc][1] & bv) ) <<  2) +
  (( !!(led[loc][2] & bv) ) <<  1) +
  (( !!(led[loc][3] & bv) )      );
}

//set a single LED's color, does not update the other colors in that LED, its faster than calling setLED
//no out of bounds position detection
void setLEDcolor(byte x, byte y, byte z, byte c, byte b) {
  int loc = (y * ANODE_LEVELS * 3) + (z * 3) + ((x * 3 + c) >> 3);
  
  writeColorToMatrix(loc, (x * 3 + c) % ANODE_LEVELS, b);
}

//set the cube refresh rate
//To calculate full color space cycle for the cube, refresh == (refresh rate) / (cycles for BAM == 15)
//Example: setting the refresh rate to 800 gives 800 / 15 == 53.3 full refreshes per second, which is about as low as you should go
void setRefreshRate(int updatesPerSecond) {
  OCR1A  = (unsigned int)((250000UL / (updatesPerSecond * ANODE_LEVELS)) - 1UL);
}

void setMatrix(byte r, byte g, byte b) {
  for (byte i = 0; i < ANODE_LEVELS; i++)
    for (byte j = 0; j < ANODE_LEVELS; j++)
      for (byte k = 0; k < ANODE_LEVELS; k++)
        setLED(i, j, k, r, g, b);
}

void clearMatrix() {
  for (int i = 0; i < ANODE_LEVELS * ANODE_LEVELS * 3; ++i) {
    led[i][0] = led[i][1] = led[i][2] = led[i][3] = 0;
    //memset((void*)(led), 0, sizeof(led));
  }
}

//axis (x=0, y=1, z=2), layer (0 - 7), red, green, blue
void setPlane(byte axis, byte layer, byte r, byte g, byte b) {
  for (byte x = (axis < 2) ? 0 : layer; x < (axis < 2) ? ANODE_LEVELS : (layer + 1); x++)
    for (byte y = (axis != 1) ? 0 : layer; y < (axis != 1) ? ANODE_LEVELS : (layer + 1); y++)
      for (byte z = (axis > 0) ? 0 : layer; z < (axis > 0) ? ANODE_LEVELS : (layer + 1); z++)
        setLED(x, y, z, r, g, b);
}

//no out of bounds layer protection
void clearLayer(byte layer) {
  for (int i = ANODE_LEVELS * 3 * layer; i < ANODE_LEVELS * 3 * (layer + 1); ++i) {
    led[i][0] = led[i][1] = led[i][2] = led[i][3] = 0;
  }
}

//no out of bounds row or layer protection
void setRowX(byte row, byte layer, byte r, byte g, byte b) {
  int locB = (layer * ANODE_LEVELS * 3) + ((row * 3    ) >> 3);
  int locG = (layer * ANODE_LEVELS * 3) + ((row * 3 + 1) >> 3);
  int locR = (layer * ANODE_LEVELS * 3) + ((row * 3 + 2) >> 3);
  for (byte z = 0; z < ANODE_LEVELS; ++z) {
    writeColorToMatrix(locB, (row * 3    ) % ANODE_LEVELS, b);
    writeColorToMatrix(locG, (row * 3 + 1) % ANODE_LEVELS, g);
    writeColorToMatrix(locR, (row * 3 + 2) % ANODE_LEVELS, r);

    locB += 3;
    locG += 3;
    locR += 3;
  }
}

//no out of bounds row or layer protection
void setRowZ(byte row, byte layer, byte r, byte g, byte b) {
  int locB = (layer * ANODE_LEVELS * 3) + (row * 3);
  int locG = locB;//(layer * ANODE_LEVELS * 3) + (row * 3);
  int locR = locB;//(layer * ANODE_LEVELS * 3) + (row * 3);
  int j = 0;
  for (byte x = 0; x < ANODE_LEVELS; ++x) {
    j = ((x * 3    ) >> 3);
    writeColorToMatrix(locB + j, (x * 3    ) % ANODE_LEVELS, b);
    j = ((x * 3 + 1) >> 3);
    writeColorToMatrix(locG + j, (x * 3 + 1) % ANODE_LEVELS, g);
    j = ((x * 3 + 2) >> 3);
    writeColorToMatrix(locR + j, (x * 3 + 2) % ANODE_LEVELS, r);
  }
}

//no out of bounds row or depth protection
void setColumn(byte x, byte z, byte r, byte g, byte b) {
  int locB = (z * 3) + ((x * 3    ) >> 3);
  int locG = (z * 3) + ((x * 3 + 1) >> 3);
  int locR = (z * 3) + ((x * 3 + 2) >> 3);
  for (byte y = 0; y < ANODE_LEVELS; ++y) {
    writeColorToMatrix(locB, (x * 3    ) % ANODE_LEVELS, b);
    writeColorToMatrix(locG, (x * 3 + 1) % ANODE_LEVELS, g);
    writeColorToMatrix(locR, (x * 3 + 2) % ANODE_LEVELS, r);

    locB += ANODE_LEVELS * 3;
    locG += ANODE_LEVELS * 3;
    locR += ANODE_LEVELS * 3;
  }
}

//takes 2 colors (r1,g1,b1) and (r2,g2,b2) and returns a new packed color 'p' percent between color 1 and color 2
//returns a packed color, use getRed, getGreen, getBlue to get each color component
//no out of bounds or invalid parameter protection
uint16_t blend(byte r1, byte g1, byte b1, byte r2, byte g2, byte b2, byte p) {
  return (((int)((r1 * (100 - p)) + (r2 * p)) / 100)     ) +
         (((int)((g1 * (100 - p)) + (g2 * p)) / 100) << 4) +
         (((int)((b1 * (100 - p)) + (b2 * p)) / 100) << 8);
}

//blends 2 packed colors, p is percentage between the 2 colors with 0=c1 and 100=c2
uint16_t blendp(uint16_t c1, uint16_t c2, byte p) {
  return (((int)((getRed(c1)   * (100 - p)) + (getRed(c2)   * p)) / 100)     ) +
         (((int)((getGreen(c1) * (100 - p)) + (getGreen(c2) * p)) / 100) << 4) +
         (((int)((getBlue(c1)  * (100 - p)) + (getBlue(c2)  * p)) / 100) << 8);
}

#endif
