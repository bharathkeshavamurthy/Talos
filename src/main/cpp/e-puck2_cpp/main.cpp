#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include "leds.h"
#include "Robot.h"

int main(void) {

    halInit();
    chSysInit();
    mpu_init();

    Robot robot;

    /* Infinite loop. */
    while (1) {
    	robot.ledSeqOn(100);
        chThdSleepMilliseconds(1000);

        robot.ledSeqOff(100);
        chThdSleepMilliseconds(1000);
    }

}
