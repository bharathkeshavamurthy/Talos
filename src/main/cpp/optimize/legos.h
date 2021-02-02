/*
 * legos.h
 *
 *  Created on: Feb 05, 2020
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the STM32F407 side of the "Bot-Group Formation/Dispersion/Re-formation" application within the Talos initiative.
 *
 *  Copyright (c) 2020. All Rights Reserved.
 */

/* LEGOS_H_ */
#ifndef LEGOS_H_
#define LEGOS_H_

/* __cplusplus */
#ifdef __cplusplus
extern "C" {
#endif
/* __cplusplus */

/* The Includes */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "cmd.h"
#include "fat.h"
#include "exti.h"
#include "sdio.h"
#include "shell.h"
#include "button.h"
#include "motors.h"
#include "usbcfg.h"
#include "i2c_bus.h"
#include "uc_usage.h"
#include "chprintf.h"
#include "selector.h"
#include "behaviors.h"
#include "ir_remote.h"
#include "talos_leds.h"
#include "sensors/imu.h"
#include "serial_comm.h"
#include "communication.h"
#include "camera/camera.h"
#include "sensors/ground.h"
#include "sensors/mpu9250.h"
#include "epuck1x/Asercom.h"
#include "audio/microphone.h"
#include "epuck1x/Asercom2.h"
#include "audio/play_melody.h"
#include "sensors/proximity.h"
#include "msgbus/messagebus.h"
#include "memory_protection.h"
#include "spi_communication.h"
#include "audio/audio_thread.h"
#include "camera/dcmi_camera.h"
#include "aseba_vm/skel_user.h"
#include "parameter/parameter.h"
#include "aseba_vm/aseba_node.h"
#include "config_flash_storage.h"
#include "audio/play_sound_file.h"
#include "aseba_vm/aseba_bridge.h"
#include "sensors/battery_level.h"
#include "epuck1x/utility/utility.h"
#include "sensors/VL53L0X/VL53L0X.h"
#include "aseba_vm/aseba_can_interface.h"
#include "epuck1x/a_d/advance_ad_scan/e_acc.h"
#include "epuck1x/motor_led/advance_one_timer/e_led.h"

/* Variable Declarations */
extern messagebus_t bus;
extern parameter_namespace_t parameter_root;

/* __cplusplus */
#ifdef __cplusplus
}
#endif
/* __cplusplus */

#endif
/* LEGOS_H_ */
