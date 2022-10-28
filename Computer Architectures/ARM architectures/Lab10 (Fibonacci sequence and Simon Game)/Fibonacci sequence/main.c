/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2022-01-29
** Last Version:        V1.00
** Descriptions:        main function is defined here.
** 
** Author:              Davide Arcolini
** Contact:             davide.arcolini@studenti.polito.it
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include <stdio.h>
#include "LED.h"
#include "BUTTON.h"

/*
*  Main function. It performs three operations:
*    o Initializing the LEDs through the LED_init() function.
*    o Initializing the BUTTONs through the BUTTON_init() function.
*    o Waiting infinitely for the external interrupts raised by the buttons.
*/
int main(void) {
	LED_init();        /* LED initialization                 */
	BUTTON_init();     /* BUTTON initialization              */
	while(1);          /* Infinite loop                      */
}