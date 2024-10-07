;----------------------------------------------------------------------
; File: Keypad.asm
; Author:

; Description:
;  This contains the code for reading the
;  16-key keypad attached to Port A
;  See the schematic of the connection in the
;  design document.
;
;  The following subroutines are provided by the module
;
; char pollReadKey(): to poll keypad for a keypress
;                 Checks keypad for 2 ms for a keypress, and
;                 returns NOKEY if no keypress is found, otherwise
;                 the value returned will correspond to the
;                 ASCII code for the key, i.e. 0-9, *, # and A-D
; void initkey(): Initialises Port A for the keypad
;
; char readKey(): to read the key on the keypad
;                 The value returned will correspond to the
;                 ASCII code for the key, i.e. 0-9, *, # and A-D
;---------------------------------------------------------------------

; Include header files
 include "sections.inc"
 include "reg9s12.inc"  ; Defines EQU's for Peripheral Ports

**************EQUATES**********


;-----Conversion table
NUMKEYS	EQU	16	; Number of keys on the keypad
BADCODE 	EQU	$FF 	; returned of translation is unsuccessful
NOKEY		EQU 	$00   ; No key pressed during poll period
POLLCOUNT	EQU	1     ; Number of loops to create 1 ms poll time

 SWITCH globalConst  ; Constant data
 
 ; defnitions for structure cnvTbl_struct
 OFFSET 0
cnvTbl_code ds.b 1
cnvTbl_ascii  ds.b 1
cnvTbl_struct_len EQU *

; Conversion Table
cnvTbl  dc.b %11101110,'1'
	dc.b %11101101,'2'
	dc.b %11101011,'3'
	dc.b %11100111,'a'
	dc.b %11011110,'4'
	dc.b %11011101,'5'
	dc.b %11011011,'6'
	dc.b %11010111,'b'
	dc.b %10111110,'7'
	dc.b %10111101,'8'
	dc.b %10111011,'9'
	dc.b %10110111,'c'
	dc.b %01111110,'*'
	dc.b %01111101,'0'
	dc.b %01111011,'#'
	dc.b %01110111,'d'



 SWITCH code_section  ; place in code section
;-----------------------------------------------------------	
; Subroutine: initKeyPad
;
; Description: 
; 	Initiliases PORT A
;-----------------------------------------------------------	
initKeyPad:
	movb #$F0, DDRA
	bset PUCR, $01
	rts

;-----------------------------------------------------------    
; Subroutine: ch <- pollReadKey
; Parameters: none
; Local variable:
; Returns
;       ch: NOKEY when no key pressed,
;       otherwise, ASCII Code in accumulator B

; Description:
;  Loops for a period of 2ms, checking to see if
;  key is pressed. Calls readKey to read key if keypress 
;  detected (and debounced) on Port A and get ASCII code for
;  key pressed.
;-----------------------------------------------------------
; Stack Usage
	OFFSET 0  ; to setup offset into stack

pollReadKey: 

   pshx
   ldy POLLCOUNT
   movb #$0F, PORTA
   
prk_mainloop:
prk_if1:
   lda PORTA
   cmpa #$0F
   beq endloops
   ldx #3000
   ldd #1
   jsr delayms
prk_if2:
   lda PORTA
   cmpa #$0F
   beq endloops
   jsr readKey

endloops:
   dey
   bne prk_mainloop
   pulx

   rts

;-----------------------------------------------------------	
; Subroutine: ch <- readKey
; Arguments: none
; Local variable: 
;	ch - ASCII Code in accumulator B

; Description:
;  Main subroutine that reads a code from the
;  keyboard using the subroutine readKeybrd.  The
;  code is then translated with the subroutine
;  translate to get the corresponding ASCII code.
;-----------------------------------------------------------	
; Stack Usage
; OFFSET 0  ; to setup offset into stack
COMPARE_KEY DS.B 1
A_STORE DS.B 1

readKey:
	psha

rk_loop:
	movb #$0F, PORTA	; init port A
	ldd #10		; delay for 10 ms
	jsr delayms
	ldab PORTA			; save the current value of PORTA into B
	cmpb #$0F			; check if we have input
	beq rk_loop			; if we don't have input
		; call our function to delay
	jsr readKeyboard


	jsr translate

	pula

 rts		           ;  return(ch); 


;-----------------------------------------------------------	
; Subroutine: ch <- readKey
; Arguments: none
; Local variable: 
;	ch - ASCII Code in accumulator B

; Description:
;  Main subroutine that reads a code from the
;  keyboard using the subroutine readKeybrd.  The
;  code is then translated with the subroutine
;  translate to get the corresponding ASCII code.
;-----------------------------------------------------------

readKeyboard:
	movb #%11101111, PORTA

if1: ldab PORTA
	cmpb #%11101111
	bne endloop
	movb #%11011111, PORTA

if2: ldab PORTA
	cmpb #%11011111
	bne endloop
	movb #%10111111, PORTA

if3: ldab PORTA
	cmpb #%10111111
	bne endloop
	movb #%01111111, PORTA
	
endloop:
	ldab PORTA
	rts
	
;-----------------------------------------------------------	
; Subroutine:  ch <- translate(code)
; Arguments
;	code - in Acc B - code read from keypad port
; Returns
;	ch - saved on stack but returned in Acc B - ASCII code
; Local Variables
;    	ptr - in register X - pointer to the table
;	count - counter for loop in accumulator A
; Description:
;   Translates the code by using the conversion table
;   searching for the code.  If not found, then BADCODE
;   is returned.
;-----------------------------------------------------------	
; Stack Usage:
   OFFSET 0
TR_CH DS.B 1  ; for ch 
TR_PR_A DS.B 1 ; preserved regiters A
TR_PR_X DS.B 1 ; preserved regiters X
TR_RA DS.W 1 ; return address

translate: psha
	pshx	; preserve registers
	leas -1,SP 		    ; byte chascii;
	ldx #cnvTbl		    ; ptr = cnvTbl;
	clra			    ; ix = 0;
	movb #BADCODE,TR_CH,SP ; ch = BADCODE;

TR_loop 			    ; do {

	; IF code = [ptr]
	cmpb cnvTbl_code,X  	    ;     if(code == ptr->code)
	bne TR_endif
				    ;     {
	movb cnvTbl_ascii,X,TR_CH,SP ;        ch <- [ptr+1]
	bra TR_endwh  		    ;         break;
TR_endif  			    ;     }
				    ;     else {	
	leax cnvTbl_struct_len,X    ;           ptr++;
	inca ; increment count      ;           ix++;
                                    ;     }	
	cmpa #NUMKEYS               ;} WHILE count < NUMKEYS
	blo TR_LOOP	
tr_endwh ; ENDWHILE

	pulb ; move ch to Acc B
	; restore registres
	pulx
	pula
	rts

