/*
 * spi_communication.h
 *
 *  Created on: Nov 9, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the SPI communication mechanism at the Master (STM32F407uC), with the Slave (ESP32 radio module).
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

/* This entity has been derived from the SPI communication routines of the STM32F407 */

#ifndef SPI_COMMUNICATION_H_
#define SPI_COMMUNICATION_H_

/* Includes */
#include <ch.h>
#include <hal.h>
#include <stdint.h>
#include <stdlib.h>

#include "motors.h"

/* Macros */
#define SPI_DELAY 5000
#define SPI_MAX_PACKET_SIZE 4

/* Function Declaration to start the SPI communication between the Master and the Slave */
void spi_communication_start(void);

#endif /* SPI_COMMUNICATION_H_ */
