/*
 * Controller.cpp
 *
 *  Created on: Oct 13, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 * 	This entity describes the controller which creates an instance of the e-puck2 bot ${Robot} and initiates various transactions which can be widely grouped into two categories: Acquisition of sensor values and Driving actuators.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

#include "ch.h"
#include "memory_protection.h"
#include "hal.h"
#include "leds.h"
#include "motors.h"
#include "Robot.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Controller.h>

int main(void) {

	/// Hardware Abstraction Layer Initialization
	halInit();
	/// ChibiOS System Initialization
	chSysInit();
	/// Memory Protection Unit Initialization
	mpu_init();

	/// Create an instance of the Robot
	Robot robot;

	/* Set both Motor speeds to 1000 */
	motors_init();
	robot.setRightMotorSpeed(1000);
	robot.setLeftMotorSpeed(1000);

	while (1) {

		/* Turn on the Red LEDs */
		for (int i = LED1; i != NUM_LED; i++) {
			robot.turnOnLed(i);
		}

		/* Turn on the Body LED */
		robot.turnOnBodyLed();

		/* Turn on the Front LED */
		robot.turnOnFrontLed();

		/* Sleep for 2 seconds */
		chThdSleepMilliseconds(2000);

		/* Set the RGB LEDs to RED */
		for (int i = LED2; i < NUM_RGB_LED; i++) {
			robot.setRgbLed(i, 255, 0, 0);
		}
		/* Set the RGB LEDs to GREEN */
		for (int i = LED2; i < NUM_RGB_LED; i++) {
			robot.setRgbLed(i, 0, 255, 0);
		}
		/* Set the RGB LEDs to BLUE */
		for (int i = LED2; i < NUM_RGB_LED; i++) {
			robot.setRgbLed(i, 0, 0, 255);
		}

		/* Turn off the Red LEDs */
		for (int i = LED1; i != NUM_LED; i++) {
			robot.turnOffLed(i);
		}

		/* Turn off the Body LED */
		robot.turnOffBodyLed();

		/* Turn off the Front LED */
		robot.turnOffFrontLed();

		/* Sleep for 2 seconds */
		chThdSleepMilliseconds(2000);

	}

}
