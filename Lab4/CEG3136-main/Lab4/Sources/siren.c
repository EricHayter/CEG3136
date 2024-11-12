/*------------------------------------------------
File: siren.c

Description: The Siren module.
-------------------------------------------------*/
#include "mc9s12dg256.h"
 

#define ONE_MS 750 // got a related value from delay.c

int count; // count the number of interrupts
void initSiren()
{
  // set PIN 5 to output compare
  TIOS |= 0b00100000;
  count = 0;
}

#define HIGH 1
#define LOW 0
int levelTC5;  // level on TC5
void turnOnSiren()
{
  // set HIGH on onput compare
  TCTL1 |= 0b00001100;
  // force pin 5 to HIGH
  CFORC = 0b00100000;
  // toggle on comparison
  TCTL1 &= 0b11110111; 
  TC5 = TCNT + ONE_MS * 200;
  TIE |= 0b00100000;
  count = 0;
}

void turnOffSiren()
{
  // disable interrupt for port 5
  TIE  &= 0b11011111;
  // set output port to 0
  TCTL1 |= 0b00001000;
  TCTL1 &= 0b00000100;
  CFORC = 0b00100000;
}


// ISR Call 
void interrupt VectorNumber_Vtimch5 oc5_isr(void) 
{
  TC5 += ONE_MS;
  count++;
  if (count >= 200) {
    count = 0;
    
  }

}



