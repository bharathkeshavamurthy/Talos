/*
 * robot.h
 *
 *  Created on: Oct 13, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the available functionalities of the e-puck2 robot (which is viewed as a single entity) for acquiring sensor values and triggering actuator operations.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#ifdef __cplusplus

class Robot {

public:

	Robot();

	/// The Motors
	void setRightMotorSpeed(int speed);
	void setLeftMotorSpeed(int speed);

	/// The Red LEDs
	void turnOnLed(int ledNumber);
	void turnOffLed(int ledNumber);

	/// The RGB LEDs
	void setRgbLed(int ledNumber, uint8_t red_value, uint8_t green_value,
			uint8_t blue_value);

	/// The Body LED
	void turnOnBodyLed();
	void turnOffBodyLed();

	/// The Front LED
	void turnOnFrontLed();
	void turnOffFrontLed();

};

#endif /* __cplusplus */

#endif /* ROBOT_H_ */
