#include "PositionController.h"
#include "NetworkConnection.h"
#include "Utility.h"
#include "MySwisTrackClient.h"

#define OBSTACLE_THR 800    // if one the 3 front proximity sensors value > this value then enable onboard obstacle avoidance, disable it otherwise
#define NUM_ROBOTS_MAX 7

// received from robot
unsigned int proxValue[NUM_ROBOTS_MAX][8];

// sent to robot
Epuck2 *robots;
int lspeed[NUM_ROBOTS_MAX], rspeed[NUM_ROBOTS_MAX];

// position control
unsigned int numRobotsToControl = 0;
MySwisTrackClient swistrackClient;
unsigned char identifyRobotParticlesFlag = 0;
double controllerFreq = 0;
signed int particleOfRobot[NUM_ROBOTS_MAX];

// network
char server_reply[2000];
int recv_size;

// various
SYSTEMTIME recvTime, currTime, controlFreqUpdateTime;
FILETIME recvTimeF, currTimeF, controlFreqUpdateTimeF;
ULONGLONG recvTime64, currTime64, controlFreqUpdateTime64;
unsigned char printToFileFlag=0;
FILE *logFile;

void printOnTerminal(int id) {
    curPos(0,0);
    printf("Robot: id=%4d, port=%s, x=%6.2f, y=%6.2f,, l=%+3d, r=%+3d\r\n", id, robots[id].getPortName(), swistrackClient.xPos[particleOfRobot[id]], swistrackClient.yPos[particleOfRobot[id]], lspeed[id], rspeed[id]);
    printf("Controller freq =%3.0f Hz\r\n", controllerFreq);
    printf("q=quit, space=reinit\r\n");
}

void printAllOnTerminal() {
    unsigned int id=0;
    curPos(0,0);
    for(id=0; id<numRobotsToControl; id++) {
        printf("Robot: id=%4d, port=%s, x=%6.2f, y=%6.2f, l=%+3d, r=%+3d\r\n", id, robots[id].getPortName(), swistrackClient.xPos[particleOfRobot[id]], swistrackClient.yPos[particleOfRobot[id]], lspeed[id], rspeed[id]);
    }
    printf("Controller freq=%3.0f\r\n", controllerFreq);
    printf("q=quit, space=reinit\r\n");
}

void enableDebugToFile() {
    logFile = fopen("logFile.csv", "w");
    fprintf(logFile, "id,robX,robY,l,r\n");
    printToFileFlag=1;
}

void disableDebugToFile() {
    printToFileFlag=0;
    fclose(logFile);
}

void printDebugToFile() {
    unsigned int id=0;

    for(id=0; id<numRobotsToControl; id++) {
        fprintf(logFile, "%d,%6.2f,%6.2f,%+3d,%+3d\n", id, swistrackClient.xPos[particleOfRobot[id]], swistrackClient.yPos[particleOfRobot[id]], lspeed[id], rspeed[id]);
    }

}

int identifyRobotParticles() {

    // move the robots one at a time and see what particle id is moving from swistrack, then couple the
    // robot with that particle id

    int error = 0;
    double xMin[NUM_ROBOTS_MAX], xMax[NUM_ROBOTS_MAX];
	double yMin[NUM_ROBOTS_MAX], yMax[NUM_ROBOTS_MAX];
	double totalDiff = 0.0;
    SYSTEMTIME startTime, currTime;
    FILETIME startTimeF, currTimeF;
    ULONGLONG startTime64, currTime64;
    unsigned int i = 0, j = 0;    

    for(i=0; i<numRobotsToControl; i++) {
        xMin[i] = 2000;
        xMax[i] = 0;
		yMin[i] = 2000;
		yMax[i] = 0;
        particleOfRobot[i] = -1;
		robots[i].setSpeed(0, 0);
		printf("Initial: robot[%d]=%s, particle id=%d\n", i, robots[i].getPortName(), i);
		std::cout << "stop wait = " << (int)robots[i].waitForUpdate(5000000) << std::endl;        
    }

    for(j=0; j<numRobotsToControl; j++) {
        printf("Moving robot %d\n", j);

        // let move the robot to be identified
        robots[j].setSpeed(150, 150);
		std::cout << "speed wait = " << (int)robots[j].waitForUpdate(5000000) << std::endl;

        // get maximum and minimum x position during motion
        updateTime(&startTime, &startTimeF, &startTime64);
        while(1) {
            recv_size = ReceiveTCP(server_reply);
            if(recv_size > 0) {
				//std::cout << "Network data received" << std::endl;
                swistrackClient.processData(server_reply, recv_size);

                for(i=0; i<numRobotsToControl; i++) {
                    if(xMin[i] > swistrackClient.xPos[i] && swistrackClient.xPos[i] > 0) {
                        xMin[i] = swistrackClient.xPos[i];
                        //printf("xMin[%d]=%f\n", i, xMin[i]);
                    }
                    if(xMax[i] < swistrackClient.xPos[i]) {
                        xMax[i] = swistrackClient.xPos[i];
                        //printf("xMax[%d]=%f\n", i, xMax[i]);
                    }
                    if(yMin[i] > swistrackClient.yPos[i] && swistrackClient.yPos[i] > 0) {
                        yMin[i] = swistrackClient.yPos[i];
                        //printf("yMin[%d]=%f\n", i, yMin[i]);
                    }
                    if(yMax[i] < swistrackClient.yPos[i]) {
                        yMax[i] = swistrackClient.yPos[i];
                        //printf("yMax[%d]=%f\n", i, yMax[i]);
                    }					
                }
            }
            updateTime(&currTime, &currTimeF, &currTime64);
            if(diffTimeMilliseconds(startTime64, currTime64) > 1500) { // 1.5 seconds
                break;
            }
        }

        // stop the robot
		std::cout << "Stopping robot " << (int)j << std::endl;
		robots[j].setSpeed(0, 0);
		std::cout << "stop wait = " << (int)robots[j].waitForUpdate(5000000) << std::endl;  

        // the robot is still moving a little so flush the position data before passing to the next robot
        // otherwise there could be some wrong identification
		std::cout << "Flushing network data" << std::endl;
        updateTime(&startTime, &startTimeF, &startTime64);
        while(1) {
            recv_size=ReceiveTCP(server_reply);
            if(recv_size > 0) {
                swistrackClient.processData(server_reply, recv_size);
            }
            updateTime(&currTime, &currTimeF, &currTime64);
            if(diffTimeMilliseconds(startTime64, currTime64) > 500) { // 0.5 seconds
                break;
            }
        }
		std::cout << "Flushed" << std::endl;

		std::cout << "Identifying robot " << (int)j << std::endl;
        for(i=0; i<numRobotsToControl; i++) {
			totalDiff = (xMax[i]-xMin[i]) + (yMax[i]-yMin[i]);
			std::cout << "particle " << (int)i << ", total diff = " << totalDiff << std::endl;
            if(totalDiff > 8.0) {  // be sure that it is actually a robot and not noise
                printf("particle of robot %d = %d (diff=%f)\n", j, i, totalDiff);
                particleOfRobot[j] = i;
				//break;
            }
        }
		
		// Reset extreme values for the next loop.
        for(i=0; i<numRobotsToControl; i++) {
            xMax[i] = 0;
            xMin[i] = 2000;
			yMax[i] = 0;
			yMin[i] = 2000;
        }		

    }

	std::cout << "Results" << std::endl;
    for(i=0; i<numRobotsToControl; i++) {
        printf("robot=%s, particle id=%d\n", robots[i].getPortName(), particleOfRobot[i]);
    }
    return error;

}

void startPositionController(Epuck2 *rob, int numRobots) {

    unsigned int i=0;

    // open the connection with Swistrack to get tracking informations
    OpenTCPConnection(3000, "127.0.0.1");

    numRobotsToControl = numRobots;
	robots = rob;

    while(identifyRobotParticles());

    swistrackClient.setNumParticles(numRobots);

    updateTime(&recvTime, &recvTimeF, &recvTime64);
    updateTime(&controlFreqUpdateTime, &controlFreqUpdateTimeF, &controlFreqUpdateTime64);

    clearTerminal();

}

void stopPositionController() {
    CloseTCPConnection();
}

void positionControlExec() {

    unsigned int i=0;

    recv_size = ReceiveTCP(server_reply);

    if(recv_size > 0) {

        updateTime(&currTime, &currTimeF, &currTime64);

        //printf("packet time: %I64d ms\n", diffTimeMilliseconds(recvTime64, currTime64));
        //updateTime(&recvTime, &recvTimeF, &recvTime64);
        if(diffTimeSeconds(controlFreqUpdateTime64, currTime64) >= 1) {
            controllerFreq = 1000.0/((double)diffTimeMilliseconds(controlFreqUpdateTime64, currTime64)/(double)swistrackClient.getPacketCount(0));  // the frequency for all robots is the same thus request the num of packets for robot id=0
            swistrackClient.resetPacketCount(0);
            updateTime(&controlFreqUpdateTime, &controlFreqUpdateTimeF, &controlFreqUpdateTime64);
        }

        swistrackClient.processData(server_reply, recv_size);

		if(swistrackClient.getNumParticlesDetected() != numRobotsToControl) {
			// Something goes wrong, the robots need to be identified again to know which particles corresponds to which robot...
			//std::cout << "Detection problem, " << (int)swistrackClient.getNumParticlesDetected() << " particles detected instead of " << (int)numRobotsToControl << std::endl;
		}
		
        if(printToFileFlag == 1) {
            printDebugToFile();
        }		
		
        memset(server_reply, 0x00, 300);
    }

	for(i=0; i<numRobotsToControl; i++) {
		proxValue[i][0] = robots[i].getProximity(0);
		proxValue[i][1] = robots[i].getProximity(1);
		proxValue[i][2] = robots[i].getProximity(2);
		proxValue[i][5] = robots[i].getProximity(5);
		proxValue[i][6] = robots[i].getProximity(6);
		proxValue[i][7] = robots[i].getProximity(7);
		lspeed[i] = 500 - proxValue[i][0]*8 - proxValue[i][1]*4 - proxValue[i][2]*2;
		rspeed[i] = 500 - proxValue[i][7]*8 - proxValue[i][6]*4 - proxValue[i][5]*2;
		robots[i].setSpeed(lspeed[i], rspeed[i]);
	}

}

