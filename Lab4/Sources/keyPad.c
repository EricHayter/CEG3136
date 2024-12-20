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
    char code;
    char ch;
    
    do {
        PORTA = 0x0F; // set all output pins to 0
        while (PORTA == 0x0F) {
            // wait
        }
        code = PORTA; // store code
        delayms(10);  // delay 10ms
        
        while (code == PORTA) {
            // wait until PORTA changes
        }
        
        code = readKeyCode(); // get the key code
        RDK_CODE = code;
        PORTA = 0x0F; // set all output pins to 0
        
        while (PORTA != 0x0F) {
            // wait
        }
        
        delayms(10); // debouncing release of the key
        ch = translate(code); // translate to ASCII code
    } while (code != PORTA); // start again if PORTA has changed
    
    return ch; // return translated character

}
/*-------------------------------------------------
Interrupt: pollReadKey
Description: Checks for a key and if present returns its ASCII
             equivalent; otherwise returns NOKEY.
---------------------------------------------------*/
char pollReadKey() 
{
char pollReadKey() {
    char ch = NOKEY;
    int count = POLLCOUNT;
    PORTA = 0x0f; // set outputs to low
    
    do {
        if (PORTA != 0x0f) {
            delayms(1);  // delay 1 ms
        }
        
        if (PORTA != 0x0f) {
            ch = readKey();
            PRK_CH = ch;
            break;
        }
        
        count--;
    } while (count != 0);
    
    return PRK_CH;
}
}

/*-------------------------------------------------
Interrupt: key_isr
Description: Display interrupt service routine
             that checks keypad every 10 ms.
---------------------------------------------------*/
