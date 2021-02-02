/*

File    : button_E-Puck_2.h
Author  : Eliot Ferragni
Date    : 22 november 2017
REV 1.0

Functions to configure and use the button through interruption
*/


#ifndef BUTTON_E_PUCK2_H
#define BUTTON_E_PUCK2_H

/**
 * @brief return if the button is pressed or not
 *
 * @return		1 if pressed and 0 if not
 */
uint8_t button_is_pressed(void);

/**
 * @brief Init the gpio of the button and the ISR related
 */
void button_init(void);


#endif /* BUTTON_E_PUCK2_H */