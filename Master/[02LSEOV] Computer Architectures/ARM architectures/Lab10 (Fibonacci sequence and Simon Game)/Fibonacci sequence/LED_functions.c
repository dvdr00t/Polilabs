/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           LED_functions.h
** Last modified Date:  2022-01-29
** Last Version:        V1.00
** Descriptions:        contains the definitions of the LED functions.
** 
** Author:              Davide Arcolini
** Contact:             davide.arcolini@studenti.polito.it
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "LED.h"


/*
* +------------------+
* | GLOBAL VARIABLES |
* +------------------+
*/
unsigned int fibonacci_start = 0;
unsigned int fibonacci_next = 1;

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
	LPC_PINCON->PINSEL4 &= 0xFFFF0000;
	LPC_GPIO2->FIODIR |= 0x000000FF;
}

/*
* Starting from the right (i.e. P2.7 -> LED4) with value 0x80 = 1000 0000
* moves one position to the right based on the passed parameter. If the number is_
*  o 1 --final--> 1000 0000
*  o 2 --final--> 0100 0000
*  o 3 --final--> 0010 0000
*  o 4 --final--> 0001 0000
*  o 5 --final--> 0000 1000
*  o 6 --final--> 0000 0100
*  o 7 --final--> 0000 0010
*  o 8 --final--> 0000 0001
*/
void LED_ON(unsigned int num) {
  LPC_GPIO2->FIOSET |= (0x80 >> (num-1));
}

/*
* Turning OFF all the LEDs.
*/
void LED_clear(void) {
  LPC_GPIO2->FIOCLR = 0x000000FF;
}