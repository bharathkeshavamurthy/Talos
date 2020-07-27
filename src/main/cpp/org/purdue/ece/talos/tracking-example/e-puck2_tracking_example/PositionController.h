#if defined(_WIN32) || defined(_WIN64)
    #include "windows.h"
#endif
#include "epuck2.h"

#ifndef POSITION_CONTROLLER_H_
#define POSITION_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

// Open the file used to print debugging data.
void enableDebugToFile();

// Close the file used to print debugging data.
void disableDebugToFile();

// Print data to file in csv format. It is useful to debug the controller and implement new algorithms offline.
void printDebugToFile();

// Tell which particle number corresponds to which robot and change the addresses sequence accordingly
int identifyRobotParticles();

// Execute the position controller that will lead the robot to the target point.
void positionControlExec();

// Initialize the controller.
void startPositionController(Epuck2 rob[], int numRobots);

// Stop the controller.
void stopPositionController();

// Print the information (such as position, orientation, target, ...) to a terminal window.
void printOnTerminal(int id);
void printAllOnTerminal();
void ready_position_data();

#ifdef __cplusplus
}
#endif

#endif // POSITION_CONTROLLER_H_



