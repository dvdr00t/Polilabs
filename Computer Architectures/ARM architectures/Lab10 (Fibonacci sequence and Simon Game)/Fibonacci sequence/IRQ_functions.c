/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_functions.h
** Last modified Date:  2022-01-29
** Last Version:        V1.00
** Descriptions:        contains the definitions of the INTERRUPTS handlers.
** 
** Author:              Davide Arcolini
** Contact:             davide.arcolini@studenti.polito.it
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "LED.h"
#include "BUTTON.h"

/*
* Computes the next number of the Fibonacci sequence. Eventually, if the result is still in the range
* of the available LEDs (i.e. if it is lower than, or equal to 8), it proceed with turning ON the 
* corresponding LED. Otherwise, it reset the pending interrupt and exit. 
*/
void EINT1_IRQHandler(void) {
	
	extern unsigned int fibonacci_start;
	extern unsigned int fibonacci_next;
	unsigned int tmp;
	
	if (fibonacci_next <= 8) {
		LED_clear();
		LED_ON(fibonacci_next);
	
		tmp = fibonacci_start + fibonacci_next;
		fibonacci_start = fibonacci_next;
		fibonacci_next = tmp;
	}
	LPC_SC->EXTINT |= (1 << 1);     /* clear pending interrupt         */
}

/*
* Computes the previuous number of the Fibonacci sequence. Eventually, if the result is still in the range
* of the available LEDs (i.e. if it is greater than, or equal to 1), it proceed with turning ON the 
* corresponding LED. Otherwise, it reset the pending interrupt and exit. 
*/
void EINT2_IRQHandler(void) {
	extern unsigned int fibonacci_start;
	extern unsigned int fibonacci_next;
	unsigned int tmp;
	
	tmp = fibonacci_next - fibonacci_start;
	fibonacci_next = fibonacci_start;
	fibonacci_start = tmp;
	
	if (fibonacci_next >= 1) {
		LED_clear();
		LED_ON(fibonacci_start);
	}
	LPC_SC->EXTINT |= (1 << 2);     /* clear pending interrupt         */
}