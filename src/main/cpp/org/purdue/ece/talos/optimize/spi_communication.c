/*
 * spi_communication.c
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

/* The header file */
#include "spi_communication.h"

/* Global variable definitions */
int data_pointer = NULL;
int spi_tx_buffer[SPI_MAX_PACKET_SIZE];
int spi_rx_buffer[SPI_MAX_PACKET_SIZE];

static thread_t *spi_thread_pointer;

static THD_WORKING_AREA(spi_thread_working_area, 1024);

/* The SPI communication thread */
static THD_FUNCTION(spi_thread, p) {
	(void)p;
	chRegSetThreadName("SPI Thread");

	/* Internal members */
	int left_motor_speed = 0;
	int right_motor_speed = 0;

	/* Forever */
	while(true) {
		/* Get motor speeds */
		left_motor_speed = left_motor_get_desired_speed();
		right_motor_speed = right_motor_get_desired_speed();
		/* Allocate the Tx and Rx buffers */
		memset(spi_tx_buffer, 0x00, SPI_PACKET_MAX_SIZE);
		memset(spi_rx_buffer, 0x00, SPI_PACKET_MAX_SIZE);
		/* Set the Tx buffer */
		spi_tx_buffer[0] = (right_motor_speed >> 8) & 0xFF;
		spi_tx_buffer[1] = right_motor_speed & 0xFF;
		spi_tx_buffer[2] = (left_motor_speed >> 8) & 0xFF;
		spi_tx_buffer[3] = left_motor_speed & 0xFF;
		/* Start transaction */
		spiSelect(&SPID1);
		spiExchange(&SPID1, spi_tx_buffer, spi_rx_buffer);
		spiUnselect(&SPID1);
#ifdef FOLLOWER /* FOLLOWER */
		/* Extract the received information and set motor speeds */
		right_motor_set_speed((spi_rx_buffer[0] << 8) | spi_rx_buffer[1]);
		left_motor_set_speed((spi_rx_buffer[2] << 8) | spi_rx_buffer[3]);
#endif /* FOLLOWER */
	}
}

/* Start the SPI communication between the Master (STM32F407uC) and the Slave (ESP32) */
void spi_communication_start(void) {
	/* SPI1 max speed is 42MHz. However, the ESP32 radio module allows a max speed of 10MHz. So, a pre-scaler of 1/8 is used. */
	static const SPIConfig spi_config = {
			NULL,
			GPIOA,
			15,
			SPI_CR1_BR_1
	};
	spiStart(&SPID1, &spi_config);
	spi_thread_pointer = chThdCreateStatic(spi_thread_work_area, sizeof(spi_thread_working_area), NORMALPRIO, spi_thread, NULL);
}
