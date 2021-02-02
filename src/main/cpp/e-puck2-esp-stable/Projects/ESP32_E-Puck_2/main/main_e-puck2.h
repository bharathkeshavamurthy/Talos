/*

File    : main_e-puck2.h
Author  : Eliot Ferragni
Date    : 12 november 2017
REV 1.0

Firmware to be run on the ESP32 of the e-puck2
*/


#ifndef MAIN_E_PUCK_2_H
#define MAIN_E_PUCK_2_H

#define DELAY_1_TICKS              	1
#define DELAY_10_TICKS              10
#define DELAY_1000_TICKS            1000   

#define CORE_0						0
#define CORE_1						1


///////////////////////////////////////////BLUART DEFINITIONS//////////////////////////////////////////////
#define BLUART_413_BLUETOOTH_CHANNEL_USED		CHANNEL_1

#define BLUART_413_UART_USED					UART_NUM_0
#define BLUART_413_UART_TX_PIN					UART_PIN_NO_CHANGE //default pin U0TXD
#define BLUART_413_UART_RX_PIN					UART_PIN_NO_CHANGE //default pin U0RXD

#define BLUART_413_CONNECTION_STATUS_PIN		GPIO_NUM_0	//0 = bluetooth connected, 1 = bluetooth not connected
#define BLUART_407_CONNECTION_STATUS_PIN		GPIO_NUM_1	// Special case. Not really used. See shared_set_level()


#define BLUART_407_BLUETOOTH_CHANNEL_USED		CHANNEL_2

#define BLUART_407_UART_USED					UART_NUM_1
#define BLUART_407_UART_TX_PIN					GPIO_NUM_17
#define BLUART_407_UART_RX_PIN					GPIO_NUM_34


//////////////////////////////////////////RGB_LED DEFINITIONS//////////////////////////////////////////////
#define RGB_LED2_RED_GPIO		32
#define RGB_LED2_GREEN_GPIO		33
#define RGB_LED2_BLUE_GPIO		25

#define RGB_LED4_RED_GPIO		14
#define RGB_LED4_GREEN_GPIO		27
#define RGB_LED4_BLUE_GPIO		26

#define RGB_LED6_RED_GPIO		22
#define RGB_LED6_GREEN_GPIO		21
#define RGB_LED6_BLUE_GPIO		13

#define RGB_LED8_RED_GPIO		4
#define RGB_LED8_GREEN_GPIO		16
#define RGB_LED8_BLUE_GPIO		15


///////////////////////////////////////////BUTTON DEFINITIONS//////////////////////////////////////////////
#define BUTTON_GPIO		GPIO_NUM_35


#endif /* MAIN_E_PUCK_2_H */