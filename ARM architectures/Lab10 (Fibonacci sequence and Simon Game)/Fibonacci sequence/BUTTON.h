/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           BUTTON.h
** Last modified Date:  2022-01-29
** Last Version:        V1.00
** Descriptions:        contains the prototypes of the BUTTON functions.
** 
** Author:              Davide Arcolini
** Contact:             davide.arcolini@studenti.polito.it
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

/**
* @brief Initialize the BUTTONs
* @param void
*/
void BUTTON_init(void);

/**
* @brief Manages the External Interrupt 1
* @param void
*/
void EINT1_IRQHandler(void);

/**
* @brief Manages the External Interrupt 2
* @param void
*/
void EINT2_IRQHandler(void);
