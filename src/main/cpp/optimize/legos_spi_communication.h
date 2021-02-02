/*
 * legos_spi_communication.h
 *
 *  Created on: Feb 7, 2020
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the SPI communication mechanism at the Master (STM32F407uC), with the Slave (ESP32 radio module) for the "Bot-group Formation/Dispersion/Re-formation" application within the Talos initiative.
 *
 *  Copyright (c) 2020. All Rights Reserved.
 */

/* LEGOS_SPI_COMMUNICATION_H_ */
#ifndef LEGOS_SPI_COMMUNICATION_H_
#define LEGOS_SPI_COMMUNICATION_H_

/* The Includes */
#include <ch.h>
#include <hal.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "usbcfg.h"
#include "motors.h"
#include "chprintf.h"
#include "sensors/proximity.h"
#include "msgbus/messagebus.h"

#define SPI_DELAY 5000
#define SPI_MAX_PACKET_SIZE 1024

/* Function Declaration to start the SPI communication between the Master and the Slave */
void spi_communication_start(uint16_t proximity_sensor_values);

/* Function Declaration to suspend SPI communication */
void talos_spi_comm_suspend(void);

/* Function Declaration to resume SPI communication */
void talos_spi_comm_resume(void);

#endif
/* LEGOS_SPI_COMMUNICATION_H_ */
