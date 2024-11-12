/*--------------------------------------------
File: SegDisp.c
Description:  Segment Display Module
---------------------------------------------*/

#include <stdtypes.h>
#include "mc9s12dg256.h"
#include "SegDisp.h"
#include "Delay_asm.h"

// Prototypes for internal functions

/*---------------------------------------------
Function: initDisp
Description: initializes hardware for the 
             7-segment displays.
-----------------------------------------------*/
unsigned int displays[4];

void initDisp(void) 
{
  int i;
//  byte DDRB @0x0003;  
//  byte DDRP @0x025A;
//  byte DTP @0x0258;
//  byte DTB @0x0001;
  DDRB = 0xFF;
  DDRP = 0x0F;
  PTP = 0x0F;
  PORTB = 0x00;

  for (i = 0; i < 4; i++) 
  {
    displays[i] = 0x00;
  }


}          
/*---------------------------------------------
Function: clearDisp
Description: Clears all displays.
-----------------------------------------------*/
void clearDisp(void) 
{
  int i;
  for (i = 0; i < 4; i++)
    displays[i] = 0x00;
}

/*---------------------------------------------
Function: setCharDisplay
Description: Receives an ASCII character (ch)
             and translates
             it to the corresponding code to 
             display on 7-segment display.  Code
             is stored in appropriate element of
             codes for identified display (dispNum).
-----------------------------------------------*/
void setCharDisplay(char ch, byte dispNum) 
{
  if (dispNum < 0 | dispNum >= 4)
    return;
  switch (ch) {
    case '0':
      displays[dispNum] = 0x3F;
      break;
    case '1':
      displays[dispNum] = 0x06;
      break;
    case '2':
      displays[dispNum] = 0x5B;
      break;
    case '3':
      displays[dispNum] = 0x3F;
      break;
    case '4':
      displays[dispNum] = 0x4F;
      break;
    case '5':
      displays[dispNum] = 0x66;
      break;
    case '6':
      displays[dispNum] = 0x6D;
      break;
    case '7':
      displays[dispNum] = 0x7D;
      break;
    case '8':
      displays[dispNum] = 0x7F;
      break;
    case '9':
      displays[dispNum] = 0x6F;
      break;                 
  }
}

/*---------------------------------------------
Function: segDisp
Description: Displays the codes in the code display table 
             (contains four character codes) on the 4 displays 
             for a period of 100 milliseconds by displaying 
             the characters on the displays for 5 millisecond 
             periods.
-----------------------------------------------*/
void segDisp(void) 
{
  int i;
  for (i = 0; i < 4; i++) {
    PTP = 0x01 << i;
    PORTB = displays[i];
  }
}
