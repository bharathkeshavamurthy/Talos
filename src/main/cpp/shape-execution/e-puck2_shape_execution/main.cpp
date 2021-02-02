#include <stdio.h>
#include "PositionController.h"
#include "Utility.h"
#if defined(_WIN32) || defined(_WIN64)
    #include "windows.h"
#endif
#include "epuck2.h"

#define NUM_ROBOTS 3

//******//
// MAIN //
//******//
int main(int argc, char *argv[]) {
	Epuck2 robots[NUM_ROBOTS];
	char port[NUM_ROBOTS][20];
    unsigned int i, exitProg=0;
    system("mode 100, 40");
	if(argc == 3) {
		sprintf(&port[0][0], "%s", argv[1]);
		sprintf(&port[1][0], "%s", argv[2]);
	} else{ 	
		for(i=0; i<NUM_ROBOTS; i++) {
			printf("\r\nInsert the robot%d port: ", i);
			scanf("%s", &port[i][0]);
		}
	}
    //enableDebugToFile();
    // Init the communication with the robots
	for(i=0; i<NUM_ROBOTS; i++) {
		std::cerr << "Connecting to robot on port " << port[i] << std::endl;
		if(robots[i].startCommunication(&port[i][0]) < 0) {
			std::cerr << "Cannot establish connection with rob" << (int)i << std::endl;
			return -1;
		}
		std::cerr << "connected!" << std::endl;
	}
    // Init the controller (identify robots).
    startPositionController(robots, NUM_ROBOTS);
    while(!exitProg) {
        positionControlExec();
        printAllOnTerminal();
        if(QKeyPressed()) {
            exitProg = 1;
        }
        if(SpaceKeyPressed()) {
            while(identifyRobotParticles());
        }
    }
    //disableDebugToFile();
	// Put the robots in a default state and close the communication.
    for(i=0; i<NUM_ROBOTS; i++) {
		robots[i].setSpeed(0, 0);
		robots[i].waitForUpdate(5000000);
		robots[i].stopCommunication();
    }
    stopPositionController();
	return 0;
}

