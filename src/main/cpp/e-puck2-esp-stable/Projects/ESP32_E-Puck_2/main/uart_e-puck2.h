/*

File    : uart_e-puck2.h
Author  : Stefano Morgani
Date    : 10 January 2018
REV 1.0

Functions to configure and use the UART communication between the ESP32 and both the main processor (F407) and the programmer (F413).
*/

#ifndef UART_E_PUCK_2_H
#define UART_E_PUCK_2_H

#define BUF_SIZE (1024)

#define ECHO_TASK_STACK_SIZE	1024
#define ECHO_TASK_PRIO			10

/**
 * @brief Echo from UART2 (F407) to UART0(F413)/UART2(F407) without flow control.
 *
 * @param *pvParameter	parameter from the xCreateTask 	
 */
void echo_task(void *pvParameter);

/**
 * @brief 	UART initialization.
 *
 */
void uart_init(void);

#endif /* UART_E_PUCK_2_H */