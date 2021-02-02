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
uint8_t *data_pointer = NULL;
uint8_t talos_spi_suspend_flag = 0;
uint8_t talos_rgb_setting_enabled = 0;
uint8_t spi_tx_buffer[SPI_MAX_PACKET_SIZE];
uint8_t spi_rx_buffer[SPI_MAX_PACKET_SIZE];

static thread_t *spi_thread_pointer;

static THD_WORKING_AREA(spi_thread_working_area, 1024);

/* The SPI communication thread */
static THD_FUNCTION(spi_thread, p) {
	(void)p;
	systime_t time;
	volatile uint32_t delay = 0;
	chRegSetThreadName("SPI Thread");

	/* Internal members */
	int left_motor_speed = 0;
	int right_motor_speed = 0;

	/* Forever */
	while(true) {
		time = chVTGetSystemTime();

		/* Check for suspension */
		chSysLock();
		if(talos_spi_suspend_flag == 1) {
			chSchGoSleepS(CH_STATE_SUSPENDED);
		}
		chSysUnlock();

#ifdef LEADER /* LEADER */
		/* Allocate the Tx and Rx buffers for transacting RGB LED operations */
		memset(spi_tx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		memset(spi_rx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		if(talos_rgb_setting_enabled == 1) {
			talos_get_all_rgb_state(&spi_tx_buffer[0]);
		} else {
			memset(spi_tx_buffer, 101, 12);
		}
		/* Execute the transaction */
		spiSelect(&SPID1);
		spiExchange(&SPID1, 12, spi_tx_buffer, spi_rx_buffer);
		spiUnselect(&SPID1);
		/* A pause in between transactions */
		for(delay=0; delay<SPI_DELAY * 2; delay++) {
			__NOP();
		}
#endif /* LEADER */

#ifdef LEADER /*LEADER */
		/* Allocate the Tx and Rx buffers for transacting motor operations */
		memset(spi_tx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		memset(spi_rx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		/* Get motor speeds */
		left_motor_speed = left_motor_get_desired_speed();
		right_motor_speed = right_motor_get_desired_speed();
		/* Prepare the Tx buffer */
		spi_tx_buffer[0] = (abs(right_motor_speed) >> 8) & 0xFF;
		spi_tx_buffer[1] = abs(right_motor_speed) & 0xFF;
		spi_tx_buffer[2] = (right_motor_speed >= 0) ? 1 : 0;
		spi_tx_buffer[3] = (abs(left_motor_speed) >> 8) & 0xFF;
		spi_tx_buffer[4] = abs(left_motor_speed) & 0xFF;
		spi_tx_buffer[5] = (left_motor_speed >= 0) ? 1 : 0;
		/* Execute the transaction */
		spiSelect(&SPID1);
		spiExchange(&SPID1, 6, spi_tx_buffer, spi_rx_buffer);
		spiUnselect(&SPID1);
		/* A pause in between transactions */
		for(delay=0; delay<SPI_DELAY * 2; delay++) {
			__NOP();
		}
#endif /* LEADER */

#ifdef FOLLOWER /* FOLLOWER */
		/* Allocate the Tx and Rx buffers for transacting motor operations */
		memset(spi_tx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		memset(spi_rx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		/* Execute the transaction */
		spiSelect(&SPID1);
		spiExchange(&SPID1, 6, spi_tx_buffer, spi_rx_buffer);
		spiUnselect(&SPID1);
		/* Extract the received information */
		right_motor_speed = (spi_rx_buffer[0] << 8) | spi_rx_buffer[1];
		right_motor_speed = (spi_tx_buffer[2] == 1) ? right_motor_speed : (-1 * right_motor_speed);
		left_motor_speed = (spi_rx_buffer[3] << 8) | spi_rx_buffer[4];
		left_motor_speed = (spi_rx_buffer[5] == 1) ? left_motor_speed : (-1 * left_motor_speed);
		/* Set motor speeds */
		right_motor_set_speed(right_motor_speed);
		left_motor_set_speed(left_motor_speed);
		/* A pause in between transactions */
		for(delay=0; delay<SPI_DELAY * 2; delay++) {
			__NOP();
		}
#endif /* FOLLOWER */

		/* Sleep for 50 ms */
		chThdSleepUntilWindowed(time, time + MS2ST(50));
	}
}

/* Enable setting the RGB LED values */
void talos_spi_rgb_setting_enable(void) {
	talos_rgb_setting_enabled = 1;
}

/* Disable setting the RGB LED values */
void talos_spi_rgb_setting_disable(void) {
	talos_rgb_setting_enabled = 0;
}

/* Check if the RGB LED value setting operation is enabled */
uint8_t talos_spi_rgb_setting_is_enabled(void) {
	return talos_rgb_setting_enabled;
}

/* Suspend SPI communication */
void talos_spi_comm_suspend(void) {
	if(talos_spi_suspend_flag == 0) {
		talos_spi_suspend_flag = 1;
		while(spi_thread_pointer->p_state != CH_STATE_SUSPENDED) { // @suppress("Field cannot be resolved")
			chThdSleepMilliseconds(2);
		}
	}
}

/* Resume SPI communication */
void talos_spi_comm_resume(void) {
	if(talos_spi_suspend_flag == 1) {
		chSysLock();
		chSchWakeupS(spi_thread_pointer, MSG_OK);
		talos_spi_suspend_flag = 0;
		chSysUnlock();
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
	spi_thread_pointer = chThdCreateStatic(spi_thread_working_area, sizeof(spi_thread_working_area), NORMALPRIO, spi_thread, NULL);
}
