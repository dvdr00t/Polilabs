/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT buttons
 * Author:  Davide Arcolini
 * Contact: davide.arcolini@studenti.polito.it
 * Date:    2022-01-29
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.h"              /* LPC17xx definitions                      */
#include "led.h"						     	/* header file containing the prototypes    */
#include "button.h"               /* header file containing the prototypes    */

/*----------------------------------------------------------------------------
  Main Program
	
	It contains the main() program which calls the initialization of the LEDs
	and the BUTTONs. In particular, the aim of the laboratory is to define an
	external interrupt that switches LEDs one position left or one position 
	right.
 *----------------------------------------------------------------------------*/
int main (void) {
	
	/*----------------------------------------------------------------------------
  LED_init()
	
	It performs two operations:
		o Set the pins (P2.0 to P2.7, the ones corresponding to the LEDs, in
	    according with the User Manual) to work with the DEFAULT function, i.e.
	    GPIO Port2.x. Default function is selected when the corresponding bits 
	    are set to 00.
	  o Set the pins to work in OUTPUT direction (those are LEDs). 
	*----------------------------------------------------------------------------*/
	LED_init();                           /* LED Initialization                 */
	
	
	/*----------------------------------------------------------------------------
  BUTTON_init()
	
	It performs four operations:
		o Set the pins (P2.10 to P2.12, the ones corresponding to INT0, KEY1 and 
	    KEY2, in according with the User Manual) to work with the function one, 
	    i.e. EINT0, EINT1, EINT2. Function one is selected when the corresponding 
			bits are set to 01.
	  o Set the pins to work in INPUT direction (those are BUTTONs). 
		o Set the edge sensitivity of the external interrupts.
		o Enabling the external interrupts in the NVIC.
	*----------------------------------------------------------------------------*/
	BUTTON_init();						            /* BUTTON Initialization              */


	LED_On(3);
	
	/*----------------------------------------------------------------------------
  From now on, waiting for EXTERNAL INTERRUPTS to occur in order to:
	  o KEY1 -> move active LED to the right
    o KEY2 -> move active LED to the left		
		o INT0 -> restore initial conditions (LED8 ON)
	*----------------------------------------------------------------------------*/
  while (1);                            /* Loop forever                       */	
}
