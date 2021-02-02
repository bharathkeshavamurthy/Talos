/*

File    : socket_e-puck2.h
Author  : Stefano Morgani
Date    : 22 March 2018
REV 1.0

Functions to configure and use the socket to exchange data through WiFi.
*/

#ifndef SOCKET_E_PUCK_2_H
#define SOCKET_E_PUCK_2_H

#define ACTUATORS_BUFF_LEN 21

#define SOCKET_TASK_STACK_SIZE	2048
#define SOCKET_TASK_PRIO		5

/**
 * @brief 	TCP communication handling between the PC and ESP32
 *
 * @param *pvParameter	parameter from the xCreateTask 	
 */
void socket_task(void *pvParameter);

/**
 * @brief 	Socket initialization.
 *
 */
void socket_init(void);

/**
 * @brief 	Set the connected bit.
 *
 */
void socket_set_event_connected(void);

/**
 * @brief 	Set the disconnected bit.
 *
 */
void socket_set_event_disconnected(void);

#endif /* SOCKET_E_PUCK_2_H */
