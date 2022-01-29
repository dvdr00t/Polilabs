/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_led.c
** Last modified Date:  2022-01-29
** Last Version:        V1.00
** Descriptions:        Atomic led init functions
** Correlated files:    
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "led.h"

/*----------------------------------------------------------------------------
  Function that initializes LEDs and switch them on/off
 *----------------------------------------------------------------------------*/

/*
   LED_init(): it perform two operations.
 
   o Setting pins to work with the default function (00) through the PINSEL4 register
	   in the PIN CONNECTED BLOCK (PCB). The default function allows the PINs to work as
		 GPIO pins, whose direction will be set in the second part of this function.
     PINSEL4 register:
     
		 +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
		 |  P2.0 |  P2.1 |  P2.2 |  P2.3 |  P2.4 |  P2.5 |  P2.6 |  P2.7 |
		 +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
		   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
			 
		Remember that those pins are the one connected to the LEDs, through the following
		mapping:
		- P2.0 --> LED11 --> PINSEL4[1:0]
		- P2.1 --> LED10 --> PINSEL4[3:2]
		- P2.2 --> LED09 --> PINSEL4[5:4]
		- P2.3 --> LED08 --> PINSEL4[7:6]
		- P2.4 --> LED07 --> PINSEL4[9:8]
		- P2.5 --> LED06 --> PINSEL4[11:10]
		- P2.6 --> LED05 --> PINSEL4[13:12]
		- P2.7 --> LED04 --> PINSEL4[15:14]
		
	 o Setting pins to work in OUTPUT direction (1) through the FIODIR register in the
	   GENERAL PURPOSE INPUT/OUTPUT (GPIO).
		 FIODIR register:
		 
		 +------+------+------+------+------+------+------+------+
		 | P2.0 | P2.1 | P2.2 | P2.3 | P2.4 | P2.5 | P2.6 | P2.7 |
		 +------+------+------+------+------+------+------+------+
		     0      1      2      3      4      5      6      7   
*/
void LED_init(void) {
	LPC_PINCON->PINSEL4 &= 0xFFFF0000;	// PIN mode GPIO: P2.0-P2.7 are set to zero (0x0000)
	                                    // The other values are left as they were (0xFFFF)
	
	LPC_GPIO2->FIODIR |= 0x000000FF;		// P2.0-P2.7 on PORT2 defined as Output
	                                    // Setting FIODIR[7:0] to 1 (Output)
}

void LED_deinit(void) {
  LPC_GPIO2->FIODIR &= 0xFFFFFF00;
}

void led4and11_ON(void) {
	LPC_GPIO2->FIOSET = 0x00000081;
}

void all_LED_off(void) {
	LPC_GPIO2->FIOCLR = 0x000000FF;
}

void led4_OFF(void) {
	LPC_GPIO2->FIOCLR = 0x00000001;
}

void ledEvenON_OddOFF(void) {
	LPC_GPIO2->FIOCLR = 0x000000AA;
	LPC_GPIO2->FIOSET = 0x00000055;
}

void LED_On(unsigned int num) {
	unsigned int value = 1 << num;
	LPC_GPIO2->FIOSET = value;
}

void LED_Off(unsigned int num) {
	unsigned int value = 1 << num;
	LPC_GPIO2->FIOCLR = value;
}

/*----------------------------------------------------------------------------
  Functions that light up four LEDs at a time
 *----------------------------------------------------------------------------*/

void first_four_LED_On(void)
{
  LPC_GPIO2->FIOSET = 0x000000F0;
}

void last_four_LED_On(void)
{
	LPC_GPIO2->FIOSET = 0x000000F;
}

