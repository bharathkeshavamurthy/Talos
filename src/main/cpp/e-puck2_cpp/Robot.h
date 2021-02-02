#ifndef ROBOT_H
#define ROBOT_H

#ifdef __cplusplus
class Robot {
	public:
		Robot();
		void turnOnLed(int ledNum);
		void turnOffLed(int ledNum);
		void ledSeqOn(int pause_ms);
		void ledSeqOff(int pause_ms);

	private:
		int leds[4] = {0, 1, 2, 3};
};
#endif

#endif /*ROBOT_H*/
