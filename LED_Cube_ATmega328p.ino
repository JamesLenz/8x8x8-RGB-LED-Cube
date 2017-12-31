//My board's pin breakouts
//5V, GND, A5,  A4,  2,    3,        4,        6
//5V, GND, SCL, SDA, INT0, INT1/PWM, latchPin, PWM

//includes
#include "header.h"
#include "Animations.h"

//functions
void setup() {
  //Enable Serial for debugging
  Serial.begin(115200);

  //set up the ADC, only used for FFT
  //ADCSRA &= ~PS_128;  // remove bits set by Arduino library
  //ADCSRA |= PS_32;    // set our own prescaler to 32

  //set pinmodes
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  //Reset shift registers to prevent all the LEDs going on, but you will still see a flash
  shiftOut(dataPin, clockPin, MSBFIRST, B11111111);   //All 1's mean off for the anode register
  for (byte i = 0; i < 24; i++)
    shiftOut(dataPin, clockPin, MSBFIRST, B00000000); //All 0s mean off for the other 24 registers
  //Toggle latch to copy data to the storage register
  PORTD |= latchBit; //Latch pin HIGH
  PORTD &= ~latchBit;//Latch pin LOW

  //This will tell me if there are any unnexpected resets happening, strictly for debugging purposes
  //blinking an LED
  for (byte i = 0; i < 2; i++) {
    digitalWrite(13, true);
    delay(200);
    digitalWrite(13, false);
    delay(200);
  }

  //SPI setup
  SPI.setBitOrder(MSBFIRST);          //Most Significant Bit First
  SPI.setDataMode(SPI_MODE0);         // Mode 0 Rising edge of data, keep clock low
  SPI.setClockDivider(SPI_CLOCK_DIV2);//Run the data in at 16MHz/2 = 8MHz

  //Interupt Service Routine and timer setup
  noInterrupts();// kill interrupts until everybody is set up
  //We use Timer 1 to refresh the cube, its the only 16 bit timer
  TCCR1A = B00000000;//Register A all 0's since we're not toggling any pins
  TCCR1B = B00001011;//bit 3 set for CTC mode, will call interrupt on counter match, bits 0 and 1 set to divide clock by 64, so 16MHz/64=250kHz
  TIMSK1 = B00000010;//bit 1 set to call the interrupt on an OCR1A match
  OCR1A  = (unsigned int)((250000UL / (REFRESH_RATE * ANODE_LEVELS)) - 1UL);//our clock runs at 250kHz, which is 1/250kHz = 4us

  SPI.begin(); //start up the SPI library
  interrupts();//start interrupts, this lets the multiplexing start

  //if analog input pin 0 is unconnected, random analog noise will cause the call to randomSeed() to generate
  //different seed numbers each time the sketch runs. randomSeed() will then shuffle the random function.
  pinMode(A0, INPUT);
  randomSeed(analogRead(A0));
}

void loop() {
  byte chosenAnimation = random(0, 4);
  if (chosenAnimation == lastAnimation)
    chosenAnimation = (chosenAnimation + 1) % 4; //prevent animations from happening more than once in a row
  lastAnimation = chosenAnimation;
  switch (chosenAnimation) {
    case 0:
      helix();
      break;
    case 1:
      sineWave3d();
      break;
    case 2:
      rain();
      break;
    case 3:
      fade();
      break;
    default:
      break;
  }
}

