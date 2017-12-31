#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "header.h"
#include "CubeFunctions.h"

void fade() {
  const byte numDots = 8;

  struct Dot {
    byte brightness = 0;
    byte stayFrame = 5;
    byte x = 0, 
         y = 0, 
         z = 0;
    uint16_t color = 0;
    uint16_t finalColor = 0;
    bool alive = false;
    bool fadeIn = true;
  }; Dot dot[numDots];

  clearMatrix();
  animationTimerStart = millis();
  while (millis() - animationTimerStart < animationLength) {
    for(int i = 0; i < numDots; i++) {
      if(dot[i].finalColor > 0) {
        byte r = getRed(dot[i].color);
        byte g = getGreen(dot[i].color);
        byte b = getBlue(dot[i].color);
        byte rf = getRed(dot[i].finalColor);
        byte gf = getGreen(dot[i].finalColor);
        byte bf = getBlue(dot[i].finalColor);
        if(dot[i].fadeIn) {
          if(r < rf) r++;
          if(g < gf) g++;
          if(b < bf) b++;
          if(r == rf && g == gf && b == bf)
            dot[i].fadeIn = false;
        }
        else if(dot[i].stayFrame > 0) {
          dot[i].stayFrame--;
        }
        else {
          if(r) r--;
          if(g) g--;
          if(b) b--;
        }
        dot[i].color = packColor(r, g, b);
      }
      if(dot[i].alive) {
        setLED(dot[i].x, dot[i].y, dot[i].z, getRed(dot[i].color), getGreen(dot[i].color), getBlue(dot[i].color));
      }
      if(dot[i].color == 0)
        dot[i].alive = false;
      if(random(10) == 0 && !dot[i].alive) {
        dot[i].alive = true;
        do {
          dot[i].finalColor = packColor(random(2) ? 8 : 0, random(2) ? 8 : 0, random(2) ? 8 : 0);
        } while(dot[i].finalColor == packColor(8, 8, 8));
        dot[i].color = 0;
        dot[i].stayFrame = 20;
        dot[i].fadeIn = true;
        dot[i].x = random(ANODE_LEVELS);
        dot[i].y = random(ANODE_LEVELS);
        dot[i].z = random(ANODE_LEVELS);
      }
    }
    delay(30);
  }
}

void rain() {
  const byte numDrops = 32;
  byte droplet[numDrops][7] = {}; //x,y,z,r,g,b,active
  signed char lightningDrops[numDrops][3];
  for (byte i = 0; i < 32; i++)
    lightningDrops[i][0] = -1; //not active
  byte li = 0; //lightning index
  unsigned long lMillis = millis();
  boolean lightning = false;
  signed char x, y, z, dx, dz;
  byte temp;
  byte totalDrops = 0;
  for (int i = 0; i < numDrops; i++) {
    droplet[i][0] = random(0, 8); //x
    droplet[i][1] = 0;//y
    droplet[i][2] = random(0, 8); //z
    droplet[i][3] = 0;//r
    droplet[i][4] = 0;//g
    droplet[i][5] = 0;//b
    droplet[i][6] = random(false, true);
    if (droplet[i][6])
      totalDrops++;
  }

  clearMatrix();
  animationTimerStart = millis();
  while (millis() - animationTimerStart < animationLength) {
    for (byte i = 0; i < numDrops; i++) {
      if (droplet[i][6]) {
        if (droplet[i][1] <= 9) {

          if (droplet[i][1] <= 7)
            setLED(droplet[i][0], 7 - droplet[i][1], droplet[i][2], droplet[i][3], droplet[i][4], droplet[i][5]);

          if (droplet[i][1] > 0 && droplet[i][1] <= 8)
            setLED(droplet[i][0], 7 - droplet[i][1] + 1, droplet[i][2], droplet[i][3] / 2, droplet[i][4] / 2, droplet[i][5] / 2);

          if (droplet[i][1] > 1 && droplet[i][1] <= 9)
            setLED(droplet[i][0], 7 - droplet[i][1] + 2, droplet[i][2], droplet[i][3] / 4, droplet[i][4] / 4, droplet[i][5] / 4);

        } else {
          droplet[i][6] = false;
          totalDrops--;
        }
        if (droplet[i][1] > 0) {
          //setLED(droplet[i][0], 7 - droplet[i][1] + 1, droplet[i][2], 0, 0, 0);

          //if (droplet[i][1] > 1)
          //  setLED(droplet[i][0], 7 - droplet[i][1] + 2, droplet[i][2], 0, 0, 0);

          if (droplet[i][1] > 2)
            setLED(droplet[i][0], 7 - droplet[i][1] + 3, droplet[i][2], 0, 0, 0);
        }
        droplet[i][1]++;
      }
    }
    if (random(0, 2) == 0 || totalDrops < 3) {
      for (byte i = 0; i < numDrops; i++) {
        if (!droplet[i][6]) {
          droplet[i][0] = random(0, 8); //x
          droplet[i][1] = 0;//y
          droplet[i][2] = random(0, 8); //z
          temp = 0;//random(0,3);
          droplet[i][3] = (temp == 0) ? 0 : random(0, 4); //r
          droplet[i][4] = (temp == 1) ? 0 : random(0, 4); //g
          droplet[i][5] = (temp == 2) ? 0 : random(4, 4); //b
          droplet[i][6] = true;
          totalDrops++;
          if (random(0, 2) != 0)
            break;
        }
      }
    }
    //lightning
    if (random(0, 150) == 0 || lightning) {
      if (!lightning) {
        lMillis = millis();
        x = random(0, 8);
        z = random(0, 8);
        li = 0;
        y = 0;
        while (y < 8) {
          lightningDrops[li][0] = x;
          lightningDrops[li][1] = y;
          lightningDrops[li][2] = z;
          li++;
          if (random(0, 2) == 0) {
            x += random(-1, 2);
            z += random(-1, 2);
            if (x < 0)
              x = 1;
            else if (x > 7)
              x = 6;
            if (z < 0)
              z = 1;
            else if (z > 7)
              z = 6;

            if (random(0, 2) == 0) {
              temp = random(1, 4);
              dx = random(-1, 2);
              dz = random(-1, 2);
              for (byte i = 0; i < temp; i++) {
                lightningDrops[li][0] = x;
                lightningDrops[li][1] = y;
                lightningDrops[li][2] = z;
                li++;
                x += dx;
                z += dz;
                if (x < 0)
                  x = 1;
                else if (x > 7)
                  x = 6;
                if (z < 0)
                  z = 1;
                else if (z > 7)
                  z = 6;
              }
            }
          }
          y++;
        }
      }
      lightning = true;
      if (random(0, 5) > 2) {
        for (byte i = 0; i < li; i++)
          if (lightningDrops[i][0] != -1)
            setLED(lightningDrops[i][0], 7 - lightningDrops[i][1], lightningDrops[i][2], 8, 8, 8);
      }
      else {
        for (byte i = 0; i < li; i++)
          if (lightningDrops[i][0] != -1)
            setLED(lightningDrops[i][0], 7 - lightningDrops[i][1], lightningDrops[i][2], 0, 0, 0);
      }
      if (millis() - lMillis >= 500) {
        lightning = false;
        for (byte i = 0; i < li; i++) {
          if (lightningDrops[i][0] == -1)
            continue;
          setLED(lightningDrops[i][0], 7 - lightningDrops[i][1], lightningDrops[i][2], 0, 0, 0);
          lightningDrops[i][0] = -1; //not active
        }
      }
    }

    delay(30);
  }
  clearMatrix();
}

void sineWave3d() {
  byte y[ANODE_LEVELS][ANODE_LEVELS] = {}; //zero the array
  float rads = 0;
  float oscRate = 0.008; //oscillation rate
  float t = 0; //time variable, 0-2pi
  float r = 0, g = 0, b = 15, rate = 1; //red, green, blue, and color change rate
  clearMatrix();
  animationTimerStart = millis();
  while (millis() - animationTimerStart < animationLength) {
    for (byte x = 0; x < ANODE_LEVELS; x++) {
      for (byte z = 0; z < ANODE_LEVELS; z++) {
        rads = ((float)sqrt(((float)x - 3.5) * ((float)x - 3.5) + ((float)z - 3.5) * ((float)z - 3.5)) / 3.5) * 2 + t;
        t += oscRate;
        while (t > TWO_PI)
          t -= TWO_PI;
        while (rads > TWO_PI)
          rads -= TWO_PI;
        setLED(x, y[x][z] , z, 0, 0, 0);
        y[x][z] = (sin(rads) * 3.999) + 4; //use 3.999 otherwise you can end up with 8 and go out of bounds
        setLED(x, y[x][z], z, r, g, b);
      }
    }

    if (r >= 14.999 && g < 14.999 && b == 0)      //red -> yellow
      g = ((g + rate) > 15) ? 15 : (g + rate);
    else if (r > 0 && g >= 14.999 && b == 0)      //yellow -> green
      r = ((r - rate) < 0) ? 0 : (r - rate);
    else if (r == 0 && g >= 14.999 && b < 14.999) //green -> cyan
      b = ((b + rate) > 15) ? 15 : (b + rate);
    else if (r == 0 && g > 0 && b >= 14.999)      //cyan -> blue
      g = ((g - rate) < 0) ? 0 : (g - rate);
    else if (r < 14.999 && g == 0 && b >= 14.999) //blue -> magenta
      r = ((r + rate) > 15) ? 15 : (r + rate);
    else if (r >= 14.999 && g == 0 && b > 0)      //magenta -> red
      b = ((b - rate) < 0) ? 0 : (b - rate);
    delay(50);
  }
  clearMatrix();
}

void helix() {
  float rads = 0; //radians, indicates angle while drawing the helix
  float t = 0;    //time variable, allows helix to move in a direction
  int i = 0;
  float radius = 3.999;
  float radiusRate = 0.04;
  signed char dir = 1;
  const byte listSize = 28; //I think I half-assed this, 28 may to be the # before the helix looped around to the beginning...
  int heldLocation[listSize] = {}; //0000000zzzyyyxxx (bitmasks x=0x007, y=0x038, z=0x1C0)
  int heldColor[listSize] = {};    //0000bbbbggggrrrr (bitmasks r=0x00F, g=0x0F0, b=0xF00)
  unsigned long lastUpdate = millis();  //holds time of last frame or drawing cycle
  float r = 0, g = 0, b = 15, rate = 1; //holds the starting color and the rate at which the color changes
  clearMatrix();
  animationTimerStart = millis();
  while (millis() - animationTimerStart < animationLength) {
    byte x = (sin(rads) * radius) + 4; //
    byte y = (cos(rads) * radius) + 4; //
    if (millis() - lastUpdate >= 100) {
      for (int j = 0; j < listSize; j++) {
        setLED(heldLocation[j] & 0x07, (heldLocation[j] & 0x38) >> 3, (heldLocation[j] & 0x1C0) >> 6, 0, 0, 0);
        heldLocation[j] += (1 << 6);
        setLED(heldLocation[j] & 0x07, (heldLocation[j] & 0x38) >> 3, (heldLocation[j] & 0x1C0) >> 6, heldColor[j] & 0xF, (heldColor[j] & 0xF0) >> 4, (heldColor[j] & 0xF00) >> 8);
      }
      lastUpdate = millis();
    }
    setLED(heldLocation[i] & 0x07, (heldLocation[i] & 0x38) >> 3, (heldLocation[i] & 0x1C0) >> 6, 0, 0, 0); //fixes lingering LEDs in a certain situation I didn't think about long even to really figure out what was happening
    heldLocation[i] = x + (y << 3);
    heldColor[i] = (byte)r + ((byte)g << 4) + ((byte)b << 8);
    setLED(x, y, 0, r, g, b);
    i = (i + 1) % listSize;
    rads += 0.2;
    if (rads >= TWO_PI)
      rads -= TWO_PI; //bring it back into the correct domain

    if (dir == 1) {
      radius += radiusRate;
      if (radius > 3.999) {
        radius = 3.999;
        dir = -dir;
      }
    } else {
      radius -= radiusRate;
      if (radius < 0) {
        radius = 0;
        dir = -dir;
      }
    }

    if (r >= 14.999 && g < 14.999 && b == 0)      //red -> yellow
      g = ((g + rate) > 15) ? 15 : (g + rate);
    else if (r > 0 && g >= 14.999 && b == 0)      //yellow -> green
      r = ((r - rate) < 0) ? 0 : (r - rate);
    else if (r == 0 && g >= 14.999 && b < 14.999) //green -> cyan
      b = ((b + rate) > 15) ? 15 : (b + rate);
    else if (r == 0 && g > 0 && b >= 14.999)      //cyan -> blue
      g = ((g - rate) < 0) ? 0 : (g - rate);
    else if (r < 14.999 && g == 0 && b >= 14.999) //blue -> magenta
      r = ((r + rate) > 15) ? 15 : (r + rate);
    else if (r >= 14.999 && g == 0 && b > 0)      //magenta -> red
      b = ((b - rate) < 0) ? 0 : (b - rate);

    delay(25);
  }
  clearMatrix();
}

void pulseBall() {
  const float outerDensity = 0.50;
  int outerLitLEDs[300] = {};
  int size = 0;
  float radius = 3;
  float radiusExpansionRate = ANODE_LEVELS / 2.0;
  clearMatrix();
  while (millis() - animationTimerStart < animationLength) {
    for (int i = 0; i < (ANODE_LEVELS * ANODE_LEVELS * ANODE_LEVELS) * outerDensity; ++i) {
      if (radius < ANODE_LEVELS >> 1)
        radius += radiusExpansionRate;
      if (radius > ANODE_LEVELS >> 1)
        radius = ANODE_LEVELS >> 1;
      byte x = (ANODE_LEVELS >> 1) + (random((int)(-radius * 1000), (int)(radius * 1000)) / 1000.0);
      byte y = (ANODE_LEVELS >> 1) + (random((int)(-radius * 1000), (int)(radius * 1000)) / 1000.0);
      byte z = (ANODE_LEVELS >> 1) + (random((int)(-radius * 1000), (int)(radius * 1000)) / 1000.0);

      int j = 0;
      for (j = 0; j < size; ++j) {
        if ((outerLitLEDs[j] & 0x1C0) >> 6 == x && (outerLitLEDs[j] & 0x38) >> 3 == y && outerLitLEDs[j] & 0x7 == z)
          break;
      }
      if (j == size) {
        size++;
        outerLitLEDs[size] = (x << 6) + (y << 3) + z;
      }
    }
    for (int i = 0; i < size; i++) {
      setLEDcolor((outerLitLEDs[i] & 0x1C0) >> 6, (outerLitLEDs[i] % 0x38) >> 3, outerLitLEDs[i] & 0x7, RED, 8);
    }
    while (1);
  }
  clearMatrix();
}

/*#define WINDOW 0
  #define LIN_OUT 1
  #define FFT_N 64
  byte oldVal_[4][8];
  void fftAnimation(){
  byte ADCexecutionTime = 30;
  int sampleRate = 6000; //can measure 0 through sampleRate/2 hertz
  int sampleDelay = (1000000L / (long)sampleRate) - ADCexecutionTime;

  //start sampling
  for(int i = 0; i < FFT_N; i++)
  {
    fft_input[i << 1] = analogRead(A5) - 512;
    fft_input[(i << 1) + 1] = 0;
    delayMicroseconds(sampleDelay);
  }
  //these 2 functions take 2.8ms to complete with FFT_N = 128
  fft_reorder(); // reorder the data before doing the fft
  fft_run(); // process the data in the fft

  for(int i = 0; i < (FFT_N >> 1); i++)
  {
    byte val = ((unsigned int)sq(fft_input[i << 1]) + (unsigned int)sq(fft_input[(i << 1) + 1])) >> 2;
    if(val > 7)
      val = 7;
    for(byte y = 0; y <= oldVal_[i % 4][i >> 2]; y++)
      setLED((i % 4) << 1, y, (i / 4), 0, 0, 0);
    for(byte y = 0; y <= val; y++)
    {
      //setLED((i % 4) << 1, y, (i / 4) << 1, 15, 0, 0);
      setLEDcolor((i % 4) << 1, y, (i / 4), (y < 4) ? 0 : ((y < 6) ? 1 : 2), 15);
    }
    oldVal_[i % 4][i >> 2] = val;
  }
  }*/

#endif

