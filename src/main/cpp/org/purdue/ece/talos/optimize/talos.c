/*
 * talos.c
 *
 *  Created on: Oct 16, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the overall control mechanism for the e-puck2 robot and serves as the starting point for the execution of the firmware on-board the STM32F407 uC.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

/* C-variant */

/* This file has been derived from the provided e-puck2 main controller firmware */

#include <math.h>
#include <talos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHELL_WA_SIZE THD_WORKING_AREA_SIZE(2048)

// Inter-Process Communication (IPC) Bus members
messagebus_t bus;
MUTEX_DECL(bus_lock); // @suppress("Field cannot be resolved")
CONDVAR_DECL(bus_condvar);

parameter_namespace_t parameter_root, aseba_ns;

// Load the parameter tree from the Flash Storage on-board the STM32F407 uC
static bool load_config(void) {
	extern uint32_t _config_start;
	return config_load(&parameter_root, &_config_start);
}

/* The Obstacle Avoidance feature */
void obstacle_avoidance() {
	// System Time member
	systime_t time;

	// Turn on the Body LED
	set_body_led(1);

	// Turn on the Front LED
	set_front_led(1);

	unsigned int red_leds_state = 0;

	unsigned int rgb_leds_state = 0;

	// Calibrate the IR Proximity Sensors
	calibrate_ir();

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
		uint16_t prox_inputs[8];
		messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus,
				"/proximity");
		messagebus_topic_wait(prox_topic, &prox_values, sizeof(prox_values));
		for (unsigned int sensor_index = 0; sensor_index < 8; sensor_index++) {
			prox_inputs[sensor_index] = get_calibrated_prox(sensor_index);
		}

		// Configure the Motors according to the processed IR Proximity Sensors' values
		right_motor_set_speed(
				(int16_t) ((MOTOR_SPEED_LIMIT / 2) - (prox_inputs[7] * 8)
						- (prox_inputs[6] * 4) - (prox_inputs[5] * 2)));
		left_motor_set_speed(
				(int16_t) ((MOTOR_SPEED_LIMIT / 2) - (prox_inputs[0] * 8)
						- (prox_inputs[1] * 4) - (prox_inputs[2] * 2)));

		// Sleep thread for 100 ms
		chThdSleepUntilWindowed(time, time + MS2ST(100));
	}
}

/* The Rotation using the Gyro feature */
void rotation_using_gyro() {
	// System Time member
	systime_t time;

	// 0 = CLOCKWISE and 1 = ANTI-CLOCKWISE
	unsigned int rotation_state = 0;

	float rotation_angle = 0.0;

	unsigned int red_leds_state = 0;

	unsigned int rgb_leds_state = 0;

	// Infinite Loop
	while (1) {
		// Get current System Time
		time = chVTGetSystemTime();

		// Play the SEVEN NATION ARMY Melody
		playMelody(SEVEN_NATION_ARMY, ML_SIMPLE_PLAY, NULL);

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

		if (rotation_state == 0) {
			// Start rotating the bot CLOCKWISE
			right_motor_set_speed(200);
			left_motor_set_speed(-200);
		} else {
			// Switch to ANTI-CLOCKWISE
			left_motor_set_speed(200);
			right_motor_set_speed(-200);
		}

		//Find the relevant IPC Message Bus Topic for the Inertial Motion Unit (Gyro, Accelerometer, and Magnetometer), Subscribe to it, and Process the shared information
		imu_msg_t imu_values;
		messagebus_topic_t *imu_topic = messagebus_find_topic_blocking(&bus,
				"/imu");
		messagebus_topic_wait(imu_topic, &imu_values, sizeof(imu_values));
		rotation_angle += get_gyro_rate(2) * getDiffTimeMsAndReset() * 0.001;
		if (fabs(rotation_angle) >= 1.8 * M_PI) {
			// Wait for some small perceivable amount of time and Switch
			chThdSleepUntilWindowed(time, time + MS2ST(2000));
			rotation_angle = 0.0;
			rotation_state = (rotation_state == 0) ? 1 : 0;
		}

		// Sleep thread for 100 ms
		chThdSleepUntilWindowed(time, time + MS2ST(100));
	}
}

/* The Cliff Fall Avoidance feature */
// I need the Ground Sensor Module extension for this feature...
void cliff_fall_avoidance() {
	// The Sensors' extracted values array
	int ground_sensor_values[5];

	// Turn on the Body LED
	set_body_led(1);

	// Turn on the Front LED
	set_front_led(1);

	// The on-surface calibration
	int on_surface_information[5];
	for (int sensor_index = 0; sensor_index < 5; sensor_index++) {
		on_surface_information[sensor_index] = get_ground_prox(sensor_index);
	}

	while (1) {
		// Play the PIRATES_OF_THE_CARIBBEAN Melody
		playMelody(PIRATES_OF_THE_CARIBBEAN, ML_SIMPLE_PLAY, NULL);

		/* Forward Movement Code */

		/* Forward Movement Code */

		// Extract the proximity values from the ground sensors
		for (int sensor_index = 0; sensor_index < 5; sensor_index++) {
			ground_sensor_values[sensor_index] = get_ground_prox(sensor_index);
		}

		if (ground_sensor_values[0] > on_surface_information[0]
				|| ground_sensor_values[1] > on_surface_information[1]
				|| ground_sensor_values[2] > on_surface_information[2]) {
			// Turn off the Body LED
			set_body_led(0);
			// Turn off the Front LED
			set_front_led(0);

			/* Rotation code using the Gyro to prevent falling */

			/* Rotation code using the Gyro to prevent falling */
		}
	}
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
 * 		g. IR Proximity Sensors
 * 		h. Battery Level Sensor
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
// Post-Initialization, this Controller triggers the required feature operation of the e-puck2 robot.
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
	spi_communication_start();
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

	/* Feature operations listed here */

#ifdef LEADER /* LEADER */
	obstacle_avoidance();
#endif /* LEADER */

	// rotation_using_gyro();
	// cliff_fall_avoidance();

	/* Feature operations list ends here */
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

/* Stack Check Fail Handler */
void __stack_chk_fail(void) {
	chSysHalt("Stack smashing detected");
}
