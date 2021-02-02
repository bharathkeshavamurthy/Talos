/*

File    : bluart_e-puck2.h
Author  : Eliot Ferragni
Date    : 6 december 2017
REV 1.0

Functions to init and use a bluetooth-UART translation
*/

#ifndef BLUART_E_PUCK_2_H
#define BLUART_E_PUCK_2_H

#include "driver/uart.h"
#include "rfcomm_e-puck2.h"

#define BLUART_BUFFER_SIZE 						2000
#define BLUART_UART_BUFFER_SIZE					2000 

#define BLUART_413_UART_BAUDRATE				230400
#define BLUART_413_UART_DATABITS				UART_DATA_8_BITS
#define BLUART_413_UART_PARITY					UART_PARITY_DISABLE
#define BLUART_413_UART_STOP_BITS				UART_STOP_BITS_1
#define BLUART_413_UART_FLOWCTRL				UART_HW_FLOWCTRL_DISABLE
#define BLUART_413_UART_FLOWCTRL_THRESHOLD		122

#define BLUART_407_UART_BAUDRATE				115200
#define BLUART_407_UART_DATABITS				UART_DATA_8_BITS
#define BLUART_407_UART_PARITY					UART_PARITY_DISABLE
#define BLUART_407_UART_STOP_BITS				UART_STOP_BITS_1
#define BLUART_407_UART_FLOWCTRL				UART_HW_FLOWCTRL_DISABLE
#define BLUART_407_UART_FLOWCTRL_THRESHOLD		122

typedef enum{
	BLUART_413 = 0,
	BLUART_407,
	NB_BLUART,
} BLUART_NB;

//struct to represent a color in RGB format (0-255)
typedef struct {

	CHANNEL_NB bluetooth_channel;		/*!< bluetooth channel used*/
	uart_port_t uart_port;				/*!< uart port used*/
	int uart_tx_pin;					/*!< uart tx pin used*/	
	int uart_rx_pin;					/*!< uart rx pin used*/

	uart_config_t* uart_config;			/*!< uart config*/
	gpio_config_t* gpio_status_config;	/*!< gpio config*/
	gpio_num_t gpio_status_pin;			/*!< gpio status pin*/

	esp_err_t (*gpio_set_level_func)(gpio_num_t gpio_num, uint32_t level);	/*!< gpio_set_level pointer*/
	void (*uart_to_bluetooth_func)(void *pvParameter);		/*!< Pointer to a task to run*/
	void (*bluetooth_to_uart_func)(void *pvParameter);		/*!< Pointer to a task to run*/

} bluart_config_t;

/*
 *	Init the uart used for the bluart tasks and create the tasks
*/
void bluart_init(void);


#endif /* BLUART_E_PUCK_2_H */