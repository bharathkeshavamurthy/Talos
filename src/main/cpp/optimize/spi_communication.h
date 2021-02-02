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
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "usbcfg.h"
#include "motors.h"
#include "chprintf.h"
#include "talos_leds.h"

/* Macros */
#ifndef LEADER /* LEADER */
#define FOLLOWER
#endif /* LEADER */

#define SPI_DELAY 5000
#define SPI_MAX_PACKET_SIZE 1024

/* Function Declaration to start the SPI communication between the Master and the Slave */
void spi_communication_start(void);

/* Function Declaration to enable setting the RGB LED values */
void talos_spi_rgb_setting_enable(void);

/* Function Declaration to disable setting the RGB LED values */
void talos_spi_rgb_setting_disable(void);

/* Function Declaration to check if the RGB LED value setting operation is enabled */
uint8_t talos_spi_rgb_setting_is_enabled(void);

/* Function Declaration to suspend SPI communication */
void talos_spi_comm_suspend(void);

/* Function Declaration to resume SPI communication */
void talos_spi_comm_resume(void);

#endif /* SPI_COMMUNICATION_H_ */
