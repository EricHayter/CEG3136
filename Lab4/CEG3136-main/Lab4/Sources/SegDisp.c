/*--------------------------------------------
File: SegDisp.c
Description:  Segment Display Module
              Uses Timer Channel 1
---------------------------------------------*/

#include "mc9s12dg256.h"
#include "SegDisp.h"

#define FIFTY_MS 37500
#define NUMDISPS 4

// Global Variables
int displays[4];


/*---------------------------------------------
Function: initDisp
Description: initializes hardware for the 
             7-segment displays.
-----------------------------------------------*/


void initDisp(void) 
{
  // Sets up Port B and P to control displays
  DDRB = 0xFF;  // Set output direction for PORT B
  DDRP |= 0x0F;  // Set output direction for PORT P bits 0 to 3
  PTP |= 0x0F;   // Disables all displays
  clearDisp();  // Clears all displays 

	TIOS |= 0b00000010; // Set ouput compare for TC1
	TIE |= 0b00000010; // Enable interrupt on TC1
	
	TC1 = TCNT + FIFTY_MS; // enables timeout on channel 1	
}

/*---------------------------------------------
Function: clearDisp
Description: Clears all displays.
-----------------------------------------------*/
void clearDisp(void) 
{
  int i;
  for (i = 0; i < 4; i++)
    displays[i] = 0;
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
      displays[dispNum] = 0b00111111;
      break;
    case '1':
      displays[dispNum] = 0b00000110;
      break;
    case '2':
      displays[dispNum] = 0b01011011;
      break;
    case '3':
      displays[dispNum] = 0b01001111;
      break;
    case '4':
      displays[dispNum] = 0b01100110;
      break;
    case '5':
      displays[dispNum] = 0b01101101;
      break;
    case '6':
      displays[dispNum] = 0b01111101;
      break;
    case '7':
      displays[dispNum] = 0b00000111;
      break;
    case '8':
      displays[dispNum] = 0b01111111;
      break;
    case '9':
      displays[dispNum] = 0b01101111;
      break;
    case '':
      displays[dispNum] = 0b00000000;      
      break;
  }
}


/*---------------------------------------------
Function: turnOnDP
Description: Turns on the decimal point of 2nd
             display from the left.
-----------------------------------------------*/
void turnOnDP(int dNum) 
{
  displays[dNum] = displays[dNum] | 0x80;  // sets bit 7  
}

/*---------------------------------------------
Function: turnOffDP
Description: Turns off the decimal point of 2nd
             display from the left.
-----------------------------------------------*/
void turnOffDP(int dNum) 
{
  displays[dNum] = displays[dNum] & 0x80;  // resets bit 7  
}


/*-------------------------------------------------
Interrupt: disp_isr
Description: Display interrupt service routine that
             to update displays every 50 ms.
---------------------------------------------------*/
void interrupt VectorNumber_Vtimch1 disp_isr(void)
{
  static byte dNum = 0;  // preserve between invocations
  byte enable;
  
  PORTB = displays[dNum];
  enable = PTP;  // get current values
  enable &= 0xF0; // erase lower four bits
  PTP = enable | ~(0x1 << dNum); // set lower for bits
  dNum++;
  dNum = dNum%NUMDISPS;
	// Set up next interrupt (also clears the interrupt)
	TC1 = TC1 + FIFTY_MS;
}


