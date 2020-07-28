#include <chrono>
#include <thread>
#include <iostream>
#include <stdio.h>
#include "epuck2.h"
#if defined(_WIN32) || defined(_WIN64)
	#include "windows.h"
#endif

using namespace std;

int main()
{
	
    Epuck2 epuck2;
    char port[20] = {0};
    sprintf(port, "com10");
	
	//float rotation_angle = 0.0;

    if(epuck2.startCommunication(port) < 0) {
        cerr << "Cannot establish connection" << endl;
        return -1;
    }

    while(1) {
		
		/*
		rotation_angle += (epuck2.getGyroRaw(2) - 0.0) * (250.0 / 32768.0) * (3.14 / 180.0);
		cout << rotation_angle << endl;
		if (rotation_angle >= 15.0) {
			epuck2.setSpeed(0, 0);
		}
		else {
			epuck2.setSpeed(-50, 50);
		}
		*/
		
		/* Sensor tests */
		cout << "ToF: " << epuck2.getDistanceMillimeters() << endl;
        cout << "Proximity-0: " << epuck2.getProximity(0) << endl;
        cout << "Accelerometer Raw-0: " << epuck2.getAccelerometerRaw(0) << endl;
        cout << "Acceleration: " << epuck2.getAcceleration() << ", Orientation: " << epuck2.getOrientation() << ", Inclination: " << epuck2.getInclination() << endl;
        cout << "Heading: " << epuck2.getHeading() << ", IMU Mag Value X: " << epuck2.getImuMagValueX() << endl;
        cout << "Temperature: " << (int)epuck2.getTemperature() << endl;
        cout << "Mic Volume Right: " << (int)epuck2.getMicVolume(MIC_RIGHT) << ", Mic Volume Left: " << (int)epuck2.getMicVolume(MIC_LEFT) << ", Mic Volume Back: " << (int)epuck2.getMicVolume(MIC_BACK) << ", Mic Volume Front: " << (int)epuck2.getMicVolume(MIC_FRONT) << endl;
        cout << "Motor Steps Left: " << epuck2.getMotorSteps(LEFT) << ", Motor Steps Right: " << epuck2.getMotorSteps(RIGHT) << endl;
        cout << "Battery Raw: " << epuck2.getBatteryRaw() << endl;
        cout << "SD State: " << (int)epuck2.getSdState() << endl;
        cout << "Ground Proximity-0: " << (int)epuck2.getGroundProximity(0) << endl;
		/* Sensor tests */
		
		/* Actuator tests */
        epuck2.setSpeed(300, -300);
        epuck2.setLed(LED1, 1);
        epuck2.setRgbLeds(100,0,0, 100,0,0, 100,0,0, 100,0,0);
        epuck2.setSound(SOUND_UNDERWORLD);
		/* Actuator tests */
		
		
		#if defined(_WIN32) || defined(_WIN64)
			Sleep(100);
		#else
			usleep(100000);
		#endif
		
    }

    epuck2.stopCommunication();

    return 0;
	
}
