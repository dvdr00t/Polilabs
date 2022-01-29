/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           LED.h
** Last modified Date:  2022-01-29
** Last Version:        V1.00
** Descriptions:        contains the prototypes of the LED functions.
** 
** Author:              Davide Arcolini
** Contact:             davide.arcolini@studenti.polito.it
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

/**
* @brief Initialize the LEDs
* @param void
*/
void LED_init(void);

/**
* @brief Turn ON the specific LED
* @param the number of the LED to be activated. NB: LED4 -> 1, ..., LED11 -> 8
*/
void LED_ON(unsigned int num);

/**
* @brief Turn OFF all the LEDs
* @param void
*/
void LED_clear(void);
