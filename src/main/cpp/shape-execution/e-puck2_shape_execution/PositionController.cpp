#include "PositionController.h"
#include "NetworkConnection.h"
#include "Utility.h"
#include "math.h"
#include "MySwisTrackClient.h"
#include "windows.h"

#define OBSTACLE_THR 800    // if one the 3 front proximity sensors value > this value then enable onboard obstacle avoidance, disable it otherwise
#define NUM_ROBOTS_MAX 7
#define TRIANGLE 2

// received from robot
unsigned int proxValue[NUM_ROBOTS_MAX][8];

// sent to robot
Epuck2 *robots;
int lspeed[NUM_ROBOTS_MAX], rspeed[NUM_ROBOTS_MAX];

// position control
unsigned int stable, entered = 0;
unsigned int numRobotsToControl = 0;
MySwisTrackClient swistrackClient;
unsigned char identifyRobotParticlesFlag = 0;
double controllerFreq = 0;
signed int particleOfRobot[NUM_ROBOTS_MAX];

// shape execution
unsigned int p, q, r = 0;;
unsigned int stable0, stable1, stable2, stable3 = 0;
unsigned int faces[NUM_ROBOTS_MAX] = {0};
double x1, y1, x2, y2, x3, y3, x2_ref, y2_ref, x3_ref, y3_ref = 0.0;

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
	// Default orientation of the robots
	faces[0] = 0;
	faces[1] = 0;
	faces[2] = 0;
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
        robots[j].setSpeed(250, 250);
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

void ready_position_data() {
	recv_size = ReceiveTCP(server_reply);
	if(recv_size > 0) {
		updateTime(&currTime, &currTimeF, &currTime64);
		if(diffTimeSeconds(controlFreqUpdateTime64, currTime64) >= 1) {
			controllerFreq = 1000.0 / ( (double) diffTimeMilliseconds(controlFreqUpdateTime64, currTime64) / (double) swistrackClient.getPacketCount(0) );
			swistrackClient.resetPacketCount(0);
			updateTime(&controlFreqUpdateTime, &controlFreqUpdateTimeF, &controlFreqUpdateTime64);
		}
		swistrackClient.processData(server_reply, recv_size);
		if(swistrackClient.getNumParticlesDetected() != numRobotsToControl) {
			
		}
		if(printToFileFlag == 1) {
			printDebugToFile();
		}		
		memset(server_reply, 0x00, 300);
	}
}

void positionControlExec() {
	
    unsigned int i, j = 0;
	int m = 0;
	ready_position_data();
	p = particleOfRobot[0];
	q = particleOfRobot[1];
	r = particleOfRobot[2];
	
#ifdef LINE
	if ( (swistrackClient.yPos[r] - swistrackClient.yPos[particleOfRobot[p]]) >= 2.0 ) {
		robots[0].setSpeed(-200, -200);
	}
	else if ( (swistrackClient.yPos[p] - swistrackClient.yPos[r]) >= 2.0 ) {
		robots[0].setSpeed(200, 200);
	}
	else {
		robots[0].setSpeed(0, 0);
	}
	if ( (swistrackClient.yPos[r] - swistrackClient.yPos[q]) >= 2.0 ) {
		robots[1].setSpeed(-200, -200);
	}
	else if ( (swistrackClient.yPos[q] - swistrackClient.yPos[r]) >= 2.0 ) {
		robots[1].setSpeed(200, 200);
	}
	else {
		robots[1].setSpeed(0, 0);
	}
#endif

#ifdef TRIANGLE
	x1 = swistrackClient.xPos[r];
	y1 = swistrackClient.yPos[r];
	x2 = swistrackClient.xPos[p];
	y2 = swistrackClient.yPos[p];
	x3 = swistrackClient.xPos[q];
	y3 = swistrackClient.yPos[q];
	if ( robots[2].getDistanceMillimeters() >= 90 && y1 < 400.0 ) {
		faces[2] = 0;
		x2_ref = x1 - 100.0;
		y2_ref = y1 + ( sqrt(3) * 100.0 );
		x3_ref = x1 + 100.0;
		y3_ref = y1 + ( sqrt(3) * 100.0 );
		if ( abs(y2_ref - y2) >= 90.0 ) {
			stable0 = 0;
		}
		if ( abs(y3_ref - y3) >= 90.0 ) {
			stable1 = 0;
		}
		if ( abs(x2_ref - x2) >= 90.0 ) {
			stable2 = 0;
		}
		if ( abs(x3_ref - x3) >= 90.0 ) {
			stable3 = 0;
		}
		if ( stable0 == 0 && stable2 == 0 && faces[0] != 0 ) {
			m = (faces[0] >= 1) ? (4 - faces[0]) : 0;
			while (1) {
				rotation_angle0 += (robots[0].getGyroRaw(2) - 0.0) * (250.0 / 32768.0) * (3.14 / 180.0);
				if ( abs(rotation_angle0) >= ( m * 15.0 ) ) {
					robots[0].setSpeed(0, 0);
					rotation_angle0 = 0.0;
					faces[0] = 0;
					m = 0;
					break;
				}
				else {
					robots[0].setSpeed(-200, 200);
				}
				Sleep(100);
			}
			return;
		}
		if ( ( stable0 == 0 ) && ( stable2 == 0 ) && ( (y2_ref - y2) >= 2.0 ) ) {
			robots[0].setSpeed(-200, -200);
			return;
		}
		else if ( ( stable0 == 0 ) && ( stable2 == 0 ) && ( (y2 - y2_ref) >= 2.0 ) ) {
			robots[0].setSpeed(200, 200);
			return;
		}
		else if ( ( stable0 == 0 ) ) {
			robots[0].setSpeed(0, 0);
			stable0 = 1;
		}
		else {
		
		}
		if ( stable1 == 0 && stable3 == 0 && faces[1] != 0 ) {	
			m = (faces[1] >= 1) ? (4 - faces[0]) : 0;
			while (1) {
				rotation_angle1 += (robots[1].getGyroRaw(2) - 0.0) * (250.0 / 32768.0) * (3.14 / 180.0);
				if ( abs(rotation_angle1) >= ( m * 15.0 ) ) {
					robots[1].setSpeed(0, 0);
					rotation_angle1 = 0.0;
					faces[1] = 0;
					m = 0;
					break;
				}
				else {
					robots[1].setSpeed(-210, 210);
				}
				Sleep(100);
			}
			return;
		}
		if ( ( stable1 == 0 ) && ( stable3 == 0 ) && ( (y3_ref - y3) >= 2.0 ) ) {
			robots[1].setSpeed(-200, -200);
			return;
		}
		else if ( ( stable1 == 0 ) && ( stable3 == 0 ) && ( (y3 - y3_ref) >= 2.0 ) ) {
			robots[1].setSpeed(200, 200);
			return;
		}
		else if ( stable1 == 0 ) {
			robots[1].setSpeed(0, 0);
			stable1 = 1;
		}
		else {
			
		}
		if ( ( stable0 == 1 ) && ( stable2 == 0 ) && ( (x2 - x2_ref) >= 2.0 ) ) {
			if ( faces[0] != 1 ) {
				while (1) {
					rotation_angle0 += (robots[0].getGyroRaw(2) - 0.0) * (250.0 / 32768.0) * (3.14 / 180.0);
					if ( abs(rotation_angle0) >= 15.0 ) {
						robots[0].setSpeed(0, 0);
						rotation_angle0 = 0.0;
						faces[0] = 1;
						break;
					}
					else {
						robots[0].setSpeed(-210, 210);
					}
					Sleep(100);
				}
				return;
			}
			if (faces[0] == 1) {
				robots[0].setSpeed(200, 200);
			}
			return;
		}
		else if ( ( stable0 == 1 ) && ( stable2 == 0 ) && ( (x2_ref - x2) >= 2.0 ) ) {
			if ( faces[0] != 3 ) {
				while (1) {
					rotation_angle0 += (robots[0].getGyroRaw(2) - 0.0) * (250.0 / 32768.0) * (3.14 / 180.0);
					if ( abs(rotation_angle0) >= 15.0 ) {
						robots[0].setSpeed(0, 0);
						rotation_angle0 = 0.0;
						faces[0] = 3;
						break;
					}
					else {
						robots[0].setSpeed(210, -210);
					}
					Sleep(100);
				}
				return;
			}
			if (faces[0] == 3) {
				robots[0].setSpeed(200, 200);
			}
			return;
		}
		else if ( ( stable0 == 1 ) && ( stable2 == 0 ) && ( abs(x2_ref - x2) < 2.0 ) ) {
			m = (faces[0] >= 1) ? (4 - faces[0]) : 0;
			if ( faces[0] != 0 ) {
				while (1) {
					rotation_angle0 += (robots[0].getGyroRaw(2) - 0.0) * (250.0 / 32768.0) * (3.14 / 180.0);
					if ( abs(rotation_angle0) >= ( m * 15.0 ) ) {
						robots[0].setSpeed(0, 0);
						rotation_angle0 = 0.0;
						faces[0] = 0;
						m = 0;
						stable2 = 1;
						break;
					}
					else {
						robots[0].setSpeed(-210, 210);
					}
					Sleep(100);
				}
			}
			return;
		} 
		else {
			
		}
		if ( ( stable1 == 1 ) && ( stable3 == 0 ) && ( (x3 - x3_ref) >= 2.0 ) ) {
			if ( faces[1] != 1 ) {
				while (1) {
					rotation_angle1 += (robots[1].getGyroRaw(2) - 0.0) * (250.0 / 32768.0) * (3.14 / 180.0);
					if ( abs(rotation_angle1) >= 15.0 ) {
						robots[1].setSpeed(0, 0);
						rotation_angle1 = 0.0;
						faces[1] = 1;
						break;
					}
					else {
						robots[1].setSpeed(-210, 210);
					}
					Sleep(100);
				}
				return;
			}
			if (faces[1] == 1) {
				robots[1].setSpeed(200, 200);
			}
			return;
		}
		else if ( ( stable1 == 1 ) && ( stable3 == 0 ) && ( (x3_ref - x3) >= 2.0 ) ) {
			if ( faces[1] != 3 ) {
				while (1) {
					rotation_angle1 += (robots[1].getGyroRaw(2) - 0.0) * (250.0 / 32768.0) * (3.14 / 180.0);
					if ( abs(rotation_angle1) >= 15.0 ) {
						robots[1].setSpeed(0, 0);
						rotation_angle1 = 0.0;
						faces[1] = 3;
						break;
					}
					else {
						robots[1].setSpeed(210, -210);
					}
					Sleep(100);
				}
				return;
			}
			if (faces[1] == 3) {
				robots[1].setSpeed(200, 200);
			}
			return;
		}
		else if ( ( stable1 == 1 ) && ( stable3 == 0 ) && ( abs(x3_ref - x3) < 2.0 ) ) {
			m = (faces[1] >= 1) ? (4 - faces[0]) : 0;
			if ( faces[1] != 0 ) {
				while (1) {
					rotation_angle1 += (robots[1].getGyroRaw(2) - 0.0) * (250.0 / 32768.0) * (3.14 / 180.0);
					if ( abs(rotation_angle1) >= ( m * 15.0 ) ) {
						robots[1].setSpeed(0, 0);
						rotation_angle1 = 0.0;
						faces[1] = 0;
						m = 0;
						stable3 = 1;
						break;
					}
					else {
						robots[1].setSpeed(-210, 210);
					}
					Sleep(100);
				}
			}
			return;
		} 
		else {
			
		}
	}
#endif

}

