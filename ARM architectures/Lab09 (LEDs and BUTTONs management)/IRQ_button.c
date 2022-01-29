#include "button.h"
#include "LPC17xx.h"
#include "led.h"

void EINT0_IRQHandler (void) {
	
  /* Getting back to the original configuration when LED8 is ON      */
  LPC_GPIO2 -> FIOCLR = 0x000000FF;
  LED_On(3);
	
  LPC_SC->EXTINT |= (1 << 0);     /* clear pending interrupt         */
}

void EINT1_IRQHandler (void) {
	
  /* Retrieving value of pins 7:0 with a mask                        */
  unsigned int value = LPC_GPIO2->FIOPIN & 0x000000FF;
  
  /* 
     If the most right LED is ON (most right LED is represented through
     FIOPIN[0], i.e. 0x00000001), then perform the rotation and start
     again from the most left LED (i.e. 0x00000080). Else, simply move
     to the right postion (value >> 1)
  */
   if (value == 0x00000001) {
   	LPC_GPIO2 -> FIOCLR = 0x000000FF;
   	LPC_GPIO2 -> FIOSET = 0x00000080;
   } else {
   	LPC_GPIO2 -> FIOCLR = 0x000000FF;
   	LPC_GPIO2 -> FIOSET = value >> 1;
   }
   
  /* clear pending interrupt                                         */
	LPC_SC->EXTINT |= (1 << 1);     
}

void EINT2_IRQHandler (void) {
	
   /* Retrieving value of pins 7:0 with a mask                        */
   unsigned int value = LPC_GPIO2->FIOPIN & 0x000000FF;
   
   /* 
     If the most left LED is ON (most right LED is represented through
     FIOPIN[7], i.e. 0x00000080), then perform the rotation and start
     again from the most right LED (i.e. 0x00000001). Else, simply move
     to the left postion (value << 1)
   */
   if (value == 0x00000080) {
   	LPC_GPIO2 -> FIOCLR = 0x000000FF;
   	LPC_GPIO2 -> FIOSET = 0x00000001;
   } else {
   	LPC_GPIO2 -> FIOCLR = 0x000000FF;
   	LPC_GPIO2 -> FIOSET = value << 1;
   }
  LPC_SC->EXTINT |= (1 << 2);     /* clear pending interrupt         */    
}
