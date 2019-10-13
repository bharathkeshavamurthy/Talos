/*
 * Controller.h
 *
 *  Created on: Oct 13, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 * 	This entity describes the controller which creates an instance of the e-puck2 bot ${Robot} and initiates various transactions which can be widely grouped into two categories: Acquisition of sensor values and Driving actuators.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#ifdef __cplusplus

extern "C" {

#endif /* __cplusplus */

#include "msgbus/messagebus.h"
#include "parameter/parameter.h"

extern messagebus_t bus;
extern parameter_namespace_t parameter_root;

#ifdef __cplusplus

}

#endif /* __cplusplus */

#endif /* CONTROLLER_H_ */
