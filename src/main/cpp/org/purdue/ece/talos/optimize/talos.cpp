/*
 * talos.cpp
 *
 *  Created on: Oct 16, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the overall control mechanism for the e-puck2 robot and serves as the starting point for the execution of the firmware on-board the STM32F407 uC.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

/* This file has been derived from the provided e-puck2 main controller firmware */

#include <math.h>
#include <vector>
#include <talos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHELL_WA_SIZE THD_WORKING_AREA_SIZE(2048)

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

parameter_namespace_t parameter_root, aseba_ns;

// Load the parameter tree from the Flash Storage on-board the STM32F407 uC
static bool load_config(void) {
	extern uint32_t _config_start;
	return config_load(&parameter_root, &_config_start);
}

/* Upon start-up, the following systems are initialized:
 * 	1. Hardware Abstraction Layer (HAL) Drivers
 * 	2. ChibiOS Kernel
 * 	3. Memory Protection and Management Unit (MPU)
 * 	4. The Inter-Process Communication (IPC) Message Bus
 * 	5. The on-board peripherals:
 * 		a. LEDs {front_led, body_led, red_leds, and rgb_leds}
 * 		b. Motors
 * 		c. USB
 * 		d. Microphones
 * 		e. Serial Peripheral Interface (SPI)
 * 		f. Digital Camera Interface (DCMI)
 * 		g. Proximity Sensors
 * 		h. Battery Level Sensors
 * 		i. IR Remote Receiver
 * 		j. Digital to Analog Converter (DAC)
 * 		k. Ground Sensor
 * 		l. I2C and the PO8030/PO6030/OV7670 Camera
 * 		m. External Interrupt Processing Services
 * 		n. Inertial Motion Unit (IMU): Accelerometer, Gyroscope, and Magnetometer
 * 		o. Time-Of-Flight Sensor (TOF) VL53L0X
 * 		p. Micro-SD Card (FAT) I/O operations
 * 	6. ASEBA CAN
 */
// Post-Initialization, this Controller triggers the Obstacle-Avoidance operation of the e-puck2 robot.
int main(void) {
	/* System Initialization Begin */

	// HAL Initialization
	halInit();

	// ChibiOS Initialization
	chSysInit();

	// Memory protection Unit Initialization
	mpu_init();

	// Initialize the IPC Message Bus
	messagebus_init(&bus, &bus_lock, &bus_condvar);

	parameter_namespace_declare(&parameter_root, NULL, NULL);

	// Initialize the on-board peripherals
	clear_leds();
	set_body_led(0);
	set_front_led(0);
	usb_start();
	dcmi_start();
	cam_start();
	motors_init();
	proximity_start();
	battery_level_start();
	dac_start();
	exti_start();
	imu_start();
	ir_remote_start();
	spi_comm_start();
	VL53L0X_start();
	serial_start();
	mic_start(NULL);
	sdio_start();
	playMelodyStart();
	playSoundFileStart();
	ground_start();
	behaviors_start();

	// Initialise Aseba system
	parameter_namespace_declare(&aseba_ns, &parameter_root, "aseba");
	aseba_declare_parameters(&aseba_ns);
	// Load parameter tree from flash
	load_config();
	// Start Aseba CAN
	aseba_vm_init();
	aseba_can_start(&vmState);

	/* System Initialization End */

	/* Obstacle Avoidance Feature */

	// System Time member
	systime_t time;

	// Turn on the Body LED
	set_body_led(1);

	// Turn on the Front LED
	set_front_led(1);

	unsigned int red_leds_state = 0;

	unsigned int rgb_leds_state = 0;

	// Infinite Loop
	while (1) {
		// Get current System Time
		time = chVTGetSystemTime();

		// Play the MISSION_IMPOSSIBLE Melody
		playMelody(IMPOSSIBLE_MISSION, ML_SIMPLE_PLAY, NULL);

		// Play with the Red LEDs
		red_leds_state = (red_leds_state == 1) ? 0 : 1;
		for (unsigned int led = LED1; led != NUM_LED; led++) {
			set_led((led_name_t) led, red_leds_state);
		}

		// Play with the RGB LEDs
		switch (rgb_leds_state) {
		case 0:
			for (unsigned int rgb_led = LED2; rgb_led != NUM_RGB_LED;
					rgb_led++) {
				set_rgb_led((rgb_led_name_t) rgb_led, 10, 0, 0);
			}
			rgb_leds_state = 1;
			break;
		case 1:
			for (unsigned int rgb_led = LED2; rgb_led != NUM_RGB_LED;
					rgb_led++) {
				set_rgb_led((rgb_led_name_t) rgb_led, 0, 10, 0);
			}
			rgb_leds_state = 2;
			break;
		case 2:
			for (unsigned int rgb_led = LED2; rgb_led != NUM_RGB_LED;
					rgb_led++) {
				set_rgb_led((rgb_led_name_t) rgb_led, 0, 0, 10);
			}
			rgb_leds_state = 3;
			break;
		case 3:
			for (unsigned int rgb_led = LED2; rgb_led != NUM_RGB_LED;
					rgb_led++) {
				set_rgb_led((rgb_led_name_t) rgb_led, 10, 10, 0);
			}
			rgb_leds_state = 4;
			break;
		case 4:
			for (unsigned int rgb_led = LED2; rgb_led != NUM_RGB_LED;
					rgb_led++) {
				set_rgb_led((rgb_led_name_t) rgb_led, 0, 10, 10);
			}
			rgb_leds_state = 5;
			break;
		case 5:
			for (unsigned int rgb_led = LED2; rgb_led != NUM_RGB_LED;
					rgb_led++) {
				set_rgb_led((rgb_led_name_t) rgb_led, 10, 0, 10);
			}
			rgb_leds_state = 6;
			break;
		case 6:
			for (unsigned int rgb_led = LED2; rgb_led != NUM_RGB_LED;
					rgb_led++) {
				set_rgb_led((rgb_led_name_t) rgb_led, 10, 0, 10);
			}
			rgb_leds_state = 0;
			break;
		}

		// Find the IPC Message Bus Topic to subscribe to in order to receive the IR Proximity Sensors' values
		proximity_msg_t prox_values;
		std::vector<uint16_t> prox_inputs;
		messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus,
				"/proximity");
		messagebus_topic_wait(prox_topic, &prox_values, sizeof(prox_values));
		for (unsigned int sensor_index = 0; sensor_index < 8; sensor_index++) {
			prox_inputs.push_back(get_calibrated_prox(sensor_index));
		}

		// Configure the Motors according to the processed IR Proximity Sensors' values
		right_motor_set_speed(
				(int16_t) ((MOTOR_SPEED_LIMIT / 2) - (prox_inputs[7] * 8)
						- (prox_inputs[6] * 4) - (prox_inputs[5] * 2)));
		left_motor_set_speed(
				(int16_t) ((MOTOR_SPEED_LIMIT / 2) - (prox_inputs[0] * 8)
						- (prox_inputs[1] * 4) - (prox_inputs[2] * 2)));

		// Refresh @ 100 Hz
		chThdSleepUntilWindowed(time, time + MS2ST(10));
	}
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void) {
	chSysHalt("Stack smashing detected");
}
