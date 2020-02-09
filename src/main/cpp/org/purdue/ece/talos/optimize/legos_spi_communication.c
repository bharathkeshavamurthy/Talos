/*
 * legos_spi_communication.c
 *
 *  Created on: Feb 7, 2020
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the SPI communication mechanism at the Master (STM32F407uC), with the Slave (ESP32 radio module) for the "Bot-group Formation/Dispersion/Re-formation" application within the Talos initiative.
 *
 *  Copyright (c) 2020. All Rights Reserved.
 */

/* The Includes */
#include "legos_spi_communication.h"

/* ODD_ONE_OUT */
#ifdef ODD_ONE_OUT
	// Inter-Process Communication (IPC) Bus members
	messagebus_t bus;
	MUTEX_DECL(bus_lock); // @suppress("Field cannot be resolved")
	CONDVAR_DECL(bus_condvar);
#endif
/* ODD_ONE_OUT */

/* Global variable definitions */
uint8_t *data_pointer = NULL;
uint8_t talos_spi_suspend_flag = 0;
uint8_t spi_tx_buffer[SPI_MAX_PACKET_SIZE];
uint8_t spi_rx_buffer[SPI_MAX_PACKET_SIZE];
uint16_t *initial_proximity_sensor_values[8];

static thread_t *spi_thread_pointer;

static THD_WORKING_AREA(spi_thread_working_area, 1024);

/* The SPI communication thread */
static THD_FUNCTION(spi_thread, p) {
	(void)p;
	systime_t time;
	volatile uint32_t delay = 0;
	chRegSetThreadName("SPI Thread");

	/* Internal members */
	int motion_dynamics_flag = 0;
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

		/* Allocate the Tx and Rx buffers for transacting motor operations */
		memset(spi_tx_buffer, 0x00, SPI_MAX_PACKET_SIZE);
		memset(spi_rx_buffer, 0x00, SPI_MAX_PACKET_SIZE);
		/* Execute the transaction */
		spiSelect(&SPID1);
		spiExchange(&SPID1, 1, spi_tx_buffer, spi_rx_buffer);
		spiUnselect(&SPID1);
		/* Extract the received information */
		motion_dynamics_flag = spi_rx_buffer[0];
		/* Set motor speeds */
		switch (motion_dynamics_flag) {
		/* Stationary */
		case 0: {
			right_motor_set_speed(0);
			left_motor_set_speed(0);
		}
		break;

		/* Formation */
		case 1: {
			right_motor_set_speed(1000);
			left_motor_set_speed(1000);
		}
		break;

		/* Dispersion */
		case 2: {
#ifndef LEADER
			right_motor_set_speed(-1000);
			left_motor_set_speed(-1000);
			/* Sleep for 2 seconds */
			chThdSleepUntilWindowed(time, time + MS2ST(2000));
			right_motor_set_speed(0);
			left_motor_set_speed(0);
#endif
		}
		break;

		/* Re-formation */
		case 3: {
/* ODD_ONE_OUT */
#ifdef ODD_ONE_OUT
			right_motor_set_speed(100);
			left_motor_set_speed(100);
			while (true) {
				proximity_msg_t proximity_msg;
				messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");
				messagebus_topic_wait(prox_topic, &proximity_msg, sizeof(proximity_msg));
				for(int i=0; i<8; i++) {
					if (get_calibrated_prox(i) >= (initial_proximity_sensor_values[i] * 2)) {
						continue;
					}
				}
				break;
			}
			right_motor_set_speed(0);
			left_motor_set_speed(0);
/* ODD_ONE_OUT */
#else
			right_motor_set_speed(0);
			left_motor_set_speed(0);
#endif
		}
		break;

		/* Default case: Stay stationary */
		default: {
			right_motor_set_speed(right_motor_speed);
			left_motor_set_speed(left_motor_speed);
			break;
		}

		}

		/* A pause in between transactions */
		for(delay=0; delay<SPI_DELAY * 2; delay++) {
			__NOP();
		}

		/* Sleep for 50 ms */
		chThdSleepUntilWindowed(time, time + MS2ST(50));
	}
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
void spi_communication_start(uint16_t *proximity_sensor_values) {
/* ODD_ONE_OUT */
#ifdef ODD_ONE_OUT
	// Initialize the IPC Message Bus and Store the initial Proximity Sensor values */
	messagebus_init(&bus, &bus_lock, &bus_condvar);
	memcpy(initial_proximity_sensor_values, proximity_sensor_values, 8);
#endif
/* ODD_ONE_OUT */
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
