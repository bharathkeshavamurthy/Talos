/*
 * legos.c
 *
 *  Created on: Feb 05, 2020
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the STM32F407 side of the "Bot-Group Formation/Dispersion/Re-formation" application within the Talos initiative.
 *
 *  Copyright (c) 2020. All Rights Reserved.
 */

/* The Includes */
#include "legos.h"

/* Stack Smashing Detect Memory Address */
#define STACK_CHK_GUARD 0xe2dee396

/* Message Bus Global Variables */
messagebus_t bus;
MUTEX_DECL(bus_lock); // @suppress("Field cannot be resolved")
CONDVAR_DECL(bus_condvar);

/* Parameter Declarations for ${parameter_root} parent and "aseba" child */
parameter_namespace_t parameter_root, aseba_ns;

/* Load the parameter tree from the Flash Memory on to the STM32F407 uC */
static bool load_config(void) {
	extern uint32_t _config_start;
	return config_load(&parameter_root, &_config_start);
}

/* Run Trigger */
void main(void) {
	/* System Initializations Begin */

	/* Hardware Abstraction Layer Initialization */
	halInit();
	/* ChibiOS System Initialization */
	chSysInit();
	/* Memory Protection Unit Initialization */
	mpu_init();
	/* Message Bus Initialization */
	messagebus_init(&bus, &bus_lock, &bus_condvar);
	/* ${parameter_root} Name-space Declaration */
	parameter_namespace_declare(&parameter_root, NULL, NULL);

	/* System Initializations End */

	/* Bot-group Motion and Topology Dynamics Initialization Begin */

	/* Initialize the Motors in accordance with the bot-group motion dynamics */
	motors_init();
	/* Initialize the Proximity Sensors in accordance with the bot-group topology dynamics */
	proximity_start();
	/* TODO: Save the Status of the Proximity Sensors here... */
	/* Initialize the Serial Peripheral Interface for Master/Slave communication with the ESP32 radio IC */
	spi_communication_start();

	/* Bot-group Motion and Topology Dynamics Initialization End */

	/* Aseba CAN System Initialization Begin */

	/* Use the previously create ${parameter_root} as the parent for this "aseba" parameter */
	parameter_namespace_declare(&aseba_ns,  &parameter_root, "aseba");
	aseba_declare_parameters(&aseba_ns);
	/* Load the parameter tree from the Flash memory */
	load_config();
	aseba_vm_init();
	aseba_can_start(&vm_state);

	/* Aseba CAN System Initialization End */
}

/* Stack Smashing Check Guard */
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

/* Stack Smashing Detected! - Halt the system */
void __stack_chk_fail(void) {
	chSysHalt("Stack Smashing Detected!");
}
