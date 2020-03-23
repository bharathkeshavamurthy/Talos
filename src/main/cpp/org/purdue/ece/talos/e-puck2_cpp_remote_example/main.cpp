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
    sprintf(port, "/dev/rfcomm0");

    if(epuck2.startCommunication(port) < 0) {
        cerr << "Cannot establish connection" << endl;
        return -1;
    }

    while(1) {
        cout << epuck2.getProximity(0) << endl;
        //cout << epuck2.getAccelerometerRaw(0) << endl;
        //cout << epuck2.getAcceleration() << ", " << epuck2.getOrientation() << ", " << epuck2.getInclination() << endl;
        //cout << epuck2.getGyroRaw(0) << endl;
        //cout << epuck2.getMagneticField(0) << ", " << epuck2.getMagneticField(1) << ", " << epuck2.getMagneticField(2) << endl;
        //cout << (int)epuck2.getTemperature() << endl;
        //cout << (int)epuck2.getMicVolume(MIC_RIGHT) << ", " << (int)epuck2.getMicVolume(MIC_LEFT) << ", " << (int)epuck2.getMicVolume(MIC_BACK) << ", " << (int)epuck2.getMicVolume(MIC_FRONT) << endl;
        //cout << epuck2.getMotorSteps(LEFT) << ", " << epuck2.getMotorSteps(RIGHT) << endl;
        //cout << epuck2.getBatteryRaw() << endl;
        //cout << (int)epuck2.getSdState() << endl;
        //cout << (int)epuck2.getGroundProximity(0) << endl;

        //epuck2.setSpeed(300, -300);
        //epuck2.setLed(LED1, 1);
        //epuck2.setRgbLeds(100,0,0, 100,0,0, 100,0,0, 100,0,0);
        //epuck2.setSound(SOUND_UNDERWORLD);
		//#if defined(_WIN32) || defined(_WIN64)
		//	Sleep(100);
		//#else
		//	usleep(100000);
		//#endif
        //epuck2.setSpeed(0, 0);
        //epuck2.setLed(LED1, 0);
        //epuck2.setRgbLeds(0,100,0, 0,100,0, 0,100,0, 0,100,0);
        //epuck2.setSound(SOUND_STOP);
		//#if defined(_WIN32) || defined(_WIN64)
		//	Sleep(100);
		//#else
		//	usleep(100000);
		//#endif

	#if defined(_WIN32) || defined(_WIN64)
       	Sleep(100);
	#else
		struct timespec timespec_req;
		timespec_req.tv_sec = 0;
		timespec_req.tv_nsec = 10*1000*1000;
		nanosleep(&timespec_req, NULL);	
	#endif
    }

    epuck2.stopCommunication();

    return 0;
}
