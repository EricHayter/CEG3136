/*-----------------------------------------------------------
    File: keyPad.c
    Description: Module for reading the keypad using interrupts
                 and timer channel 4.
	fall 2020
-------------------------------------------------------------*/

#include "mc9s12dg256.h"
#include "keyPad.h"


// Global variables
char currentKey;
char prevKey;


// Local Function Prototypes


/*---------------------------------------------
Function: initKeyPad
Description: initializes hardware for the 
             KeyPad Module.
-----------------------------------------------*/
void initKeyPad(void) 
{
  DDRA = 0b11110000;
  PUCR |= 0b00000001;
  
  TIOS |= 0b00010000;
  TIE |= 0b00000001;
  TC0 = TCNT + 7500;
}

/*-------------------------------------------------
Interrupt: readKey
Description: Waits for a key and returns its ASCII
             equivalent.
---------------------------------------------------*/
char readKey() 
{
  char code;
  char ch;
  
  
 
    PORTA = 0x0F;
    while(PORTA ==0x0f){
      
    }
    if(prevKey!=currentKey){
      return NOKEY;
    } 
    
    
    switch(currentKey){
    
      case 0b00001111: return NOKEY;
      case 0b11101110: return '1';
      case 0b11101101: return '2';
      case 0b11101011: return '3';
      case 0b11100111: return 'a';
      case 0b11011110: return '4';
      case 0b11011101: return '5';
      case 0b11011011: return '6';
      case 0b11010111: return 'b';
      case 0b10111110: return '7';
      case 0b10111101: return '8';
      case 0b10111011: return '9';
      case 0b10110111: return 'c';
      case 0b01111110: return '*';
      case 0b01111101: return '0';
      case 0b01111011: return '#';
      case 0b01110111: return 'd';
      default: return BADCODE;
    }
     
  
}

/*-------------------------------------------------
Interrupt: pollReadKey
Description: Checks for a key and if present returns its ASCII
             equivalent; otherwise returns NOKEY.
---------------------------------------------------*/
char pollReadKey() 
{
  char ch;
  ch = NOKEY;
  readKey();
  PORTA = 0x0f;
  if(PORTA != 0X0F){
    delayms(1);
  }
  if(PORTA != 0X0F){
    ch = readKey();
  }
  
  return ch;
}

/*-------------------------------------------------
Interrupt: key_isr
Description: Display interrupt service routine
             that checks keypad every 10 ms.
---------------------------------------------------*/

void interrupt VectorNumber_Vtimch5 key_isr(void){

  prevKey = currentKey;
  currentKey = PORTA;
  
  TC0 += 7500; 
  
}
  
