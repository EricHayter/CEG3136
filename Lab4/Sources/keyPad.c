/*-----------------------------------------------------------
    File: keyPad.c
    Description: Module for reading the keypad using interrupts
                 and timer channel 4.
	fall 2020
-------------------------------------------------------------*/

#include "mc9s12dg256.h"
#include "keyPad.h"


// Global variables


// Local Function Prototypes


/*---------------------------------------------
Function: initKeyPad
Description: initializes hardware for the 
             KeyPad Module.
-----------------------------------------------*/
void initKeyPad(void) 
{

}

/*-------------------------------------------------
Interrupt: readKey
Description: Waits for a key and returns its ASCII
             equivalent.
---------------------------------------------------*/
char readKey() 
{

}
/*-------------------------------------------------
Interrupt: pollReadKey
Description: Checks for a key and if present returns its ASCII
             equivalent; otherwise returns NOKEY.
---------------------------------------------------*/
char pollReadKey() 
{

}

/*-------------------------------------------------
Interrupt: key_isr
Description: Display interrupt service routine
             that checks keypad every 10 ms.
---------------------------------------------------*/
