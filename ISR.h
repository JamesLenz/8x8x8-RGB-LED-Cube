#ifndef ISR_H
#define ISR_H

#include "header.h"

//an interrupt when the clock matches
ISR(TIMER1_COMPA_vect) { //Interrupt Service Routine, Timer/Counter1 Compare Match A
  //each output cycle takes ANODE_LEVELS executions and each brightness bit's numerical representation is the length of time it remains on
  //then the 1st bit stays on for 1 cycle (ANODE_LEVELS executions), the 2nd bit stays on for 2 cycles (ANODE_LEVELS * 2 executions), then 4 cycles, and so forth
  //but since we never* reset bamCounter, we must check for the previous bit's cycles, ie: check the 4th bit for (ANODE_LEVELS + ANODE_LEVELS * 2 + ANODE_LEVELS * 4)
  //which is why you see the (x0 default implied), x1, x3, x7, higher resolution would continue x15, x31, x63, x127, or (compareTo = ANODE_LEVELS * ((1 << (bit - 1)) - 1))
  if (bamCounter == ANODE_LEVELS || bamCounter == (ANODE_LEVELS * 3) || bamCounter == (ANODE_LEVELS * 7)) //hardcoded a 4 bit resolution
    bamBit++;
  bamCounter++;

  //Anodes Shift Register
  SPI.transfer(0xFF - (1 << anodeLevel));

  int loc = (anodeLevel * ANODE_LEVELS + (ANODE_LEVELS - 1)) * 3;
  //The other 24 Shift Registers
  for (signed char z = ANODE_LEVELS - 1; z >= 0 ; z--, loc -= 3) {
    SPI.transfer(led[loc + 2][bamBit]);
    SPI.transfer(led[loc + 1][bamBit]);
    SPI.transfer(led[loc][bamBit]);
  }

  //Toggle latch to copy data to the storage register
  PORTD |= latchBit; //Latch pin HIGH
  PORTD &= ~latchBit;//Latch pin LOW

  if (bamCounter >= ANODE_LEVELS * 15) { //4 bit resolution
    bamCounter = 0;
    bamBit = 0;
  }
  anodeLevel = (anodeLevel + 1) % ANODE_LEVELS;
}

#endif

