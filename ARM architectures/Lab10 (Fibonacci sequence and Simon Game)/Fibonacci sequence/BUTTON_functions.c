/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           BUTTON_functions.h
** Last modified Date:  2022-01-29
** Last Version:        V1.00
** Descriptions:        contains the definitions of the BUTTON functions.
** 
** Author:              Davide Arcolini
** Contact:             davide.arcolini@studenti.polito.it
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "BUTTON.h"

/*
   BUTTON_init(): it perform four operations.
 
   o Setting pins to work with the function one (01) through the PINSEL4 register
     in the PIN CONNECTED BLOCK (PCB). The function one allows the PINs to work as
     EXTERNAL INTERRUPTS pins, whose direction will be set in the second part of this 
     function.
     PINSEL4 register:
     
     +----+----+----+----+----+----+
     |  P2.10  |  P2.11  |  P2.12  |
     +----+----+----+----+----+----+
       20   21   22   23   24   25
			 
	 Remember that those pins are the one connected to the BUTTONs, through the following
	 mapping:
	 - P2.10 --> INT0 --> PINSEL4[21:20]    -->    DOES NOT MATTER FOR THE EXERCISE
	 - P2.11 --> KEY1 --> PINSEL4[23:22]    -->    SET AS EINT1
	 - P2.12 --> KEY2 --> PINSEL4[25:24]    -->    SET AS EINT2
		
   o Setting pins to work in INPUT direction (0) through the FIODIR register in the
     GENERAL PURPOSE INPUT/OUTPUT (GPIO).
     FIODIR register:
     
     +-------+-------+-------+
     | P2.10 | P2.11 | P2.12 |
     +-------+-------+-------+
         10      11      12      
     	 
   o Setting the edge-sensitivity through the EXTMODE register in the SYSTEM CONTROL
     BLOCK (SCB). Sensitivity can be LEVEL (0) or EDGE (1). 
     EXTMODE register (the External Interrupt Mode Register controls whether each 
     pin is edge- or level-sensitive):
     
     +----------+----------+----------+----------+----------
     | EXTMODE0 | EXTMODE1 | EXTMODE2 | EXTMODE3 | reserved...
     +----------+----------+----------+----------+----------
          0          1          2           3          
     		
     Where the mapping is the following:
     - EXTMODE[0] --> EINT0 
     - EXTMODE[1] --> EINT1
     - EXTMODE[2] --> EINT2
     - EXTMODE[3] --> EINT3
		 
  o Enabling the External Interrupt in the NESTED VECTORED INPUT CONTROL (NVIC).
    It calls a function defined in core_cm3.h which manages the specific
    interrupts in the NVIC.
*/
void BUTTON_init(void) {
	
	/* Setting the PINs to work as external interrupts in input */
	
  LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 1 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 11);    /* PORT2.11 defined as input          */
  
  LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 2 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 12);    /* PORT2.12 defined as input          */
	
	
	/* Setting the edge-sensitivity to the PINs */
	
  LPC_SC->EXTMODE |= 0x00000006;           /* 0x06 = 0000 0110                   */
	
	
	/* Enabling the interrupts */
	
	NVIC_EnableIRQ(EINT2_IRQn);              /* enable irq in NVIC                 */
  NVIC_EnableIRQ(EINT1_IRQn);              /* enable irq in NVIC                 */
}


