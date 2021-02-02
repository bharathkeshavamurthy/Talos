/*

File    : uart_e-puck2.h
Author  : Stefano Morgani
Date    : 10 January 2018
REV 1.0

Functions to configure and use the UART communication between the ESP32 and both the main processor (F407) and the programmer (F413).
*/

#ifndef UART_E_PUCK_2_H
#define UART_E_PUCK_2_H

#define UART_TX_BUFF_SIZE 22
#define UART_RX_BUFF_SIZE 104
#define RESPONSE_SIZE 103

#define ECHO_TASK_STACK_SIZE	1024
#define ECHO_TASK_PRIO			10

typedef enum {
	SENSORS_BUFF_EMPTY,
	SENSORS_BUFF_FILLED
} sensors_buffer_state_t;

typedef struct {
	sensors_buffer_state_t state;
	uint8_t* data;
} sensors_buffer_t;

/**
 * @brief Task used to exchange data with the F407 through UART (based on asercom protocol).
 *
 * @param *pvParameter	parameter from the xCreateTask 	
 */
void advsercom_task(void *pvParameter);

/**
 * @brief 	UART initialization.
 *
 */
void uart_init(void);

/**
 * @brief Set the buffer content that will be sent to the F407 the next time.
 */
void uart_set_actuators_state(uint8_t *buff);

/**
 * @brief Return the pointer to the most recent UART data received from the F407.
 */
sensors_buffer_t *uart_get_data_ptr(void);


#endif /* UART_E_PUCK_2_H */