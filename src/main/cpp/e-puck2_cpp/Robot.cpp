#include "Robot.h"

#include "leds.h"

Robot::Robot() {
}

void Robot::turnOnLed(int ledNum) {
	set_led((led_name_t)ledNum, 1);
}

void Robot::turnOffLed(int ledNum) {
	set_led((led_name_t)ledNum, 0);
}

void Robot::ledSeqOn(int pause_ms) {
	for (int n : leds) {
		set_led((led_name_t)n, 1);
		chThdSleepMilliseconds(pause_ms);
	}
}

void Robot::ledSeqOff(int pause_ms) {
	for (int n : leds) {
		set_led((led_name_t)n, 0);
		chThdSleepMilliseconds(pause_ms);
	}
}
