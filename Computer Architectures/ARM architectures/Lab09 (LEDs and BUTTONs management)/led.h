/*
* +--------------------------------------------------------+
* |                   LAB09 - LEDs                         |
* | Header file containing the prototypes of the functions |
* |       that will be used in the exercises               |
* +--------------------------------------------------------+
*/

/*
Powering on the LEDs 4 and 11 acting on the FIOSET register.
*/
void led4and11_ON(void);

/*
Powering off the LED 4 acting on the FIOCLR register.
*/
void led4_OFF(void);

/*
Powering off the LED even acting on the FIOCLR register.
*/
void ledEvenON_OddOFF(void);

/*
Powering on the LED corresponding to the parameter passed.
*/
void LED_On(unsigned int num);

/*
Powering off the LED corresponding to the parameter passed.
*/
void LED_Off(unsigned int num);

/*
Initialization functions
*/
void all_LED_off(void);
void LED_deinit(void);
void LED_init(void);
void first_four_LED_On(void);
void last_four_LED_On(void);