/*
 * Robot.cpp
 *
 *  Created on: Oct 13, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the available functionalities of the e-puck2 robot (which is viewed as a single entity) for acquiring sensor values and triggering actuator operations.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

#include "leds.h"
#include "motors.h"

#include <Robot.h>

Robot::Robot() {
	/* Nothing to do */
}

void Robot::setRightMotorSpeed(int speed) {
	right_motor_set_speed(speed);
}

void Robot::setLeftMotorSpeed(int speed) {
	left_motor_set_speed(speed);
}

void Robot::turnOnLed(int ledNumber) {
	set_led((led_name_t) ledNumber, 1);
}

void Robot::turnOffLed(int ledNumber) {
	set_led((led_name_t) ledNumber, 0);
}

void Robot::setRgbLed(int ledNumber, uint8_t red_value, uint8_t green_value,
		uint8_t blue_value) {
	set_rgb_led((rgb_led_name_t) ledNumber, red_value, green_value, blue_value);
}

void Robot::turnOnBodyLed() {
	set_body_led(1);
}

void Robot::turnOffBodyLed() {
	set_body_led(0);
}

void Robot::turnOnFrontLed() {
	set_front_led(1);
}

void Robot::turnOffFrontLed() {
	set_front_led(0);
}
