/*
 * talos.h
 *
 *  Created on: Oct 16, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the overall control mechanism for the e-puck2 robot and serves as the starting point for the execution of the firmware on-board the STM32F407 uC.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

#ifndef TALOS_H_
#define TALOS_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* The includes */
#include "camera/dcmi_camera.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"
#include "ch.h"
#include "chprintf.h"
#include "hal.h"
#include "shell.h"
#include "aseba_vm/aseba_node.h"
#include "aseba_vm/skel_user.h"
#include "aseba_vm/aseba_can_interface.h"
#include "aseba_vm/aseba_bridge.h"
#include "audio/audio_thread.h"
#include "audio/play_melody.h"
#include "audio/play_sound_file.h"
#include "audio/microphone.h"
#include "camera/camera.h"
#include "epuck1x/Asercom.h"
#include "epuck1x/Asercom2.h"
#include "epuck1x/a_d/advance_ad_scan/e_acc.h"
#include "epuck1x/motor_led/advance_one_timer/e_led.h"
#include "epuck1x/utility/utility.h"
#include "sensors/battery_level.h"
#include "sensors/ground.h"
#include "sensors/imu.h"
#include "sensors/mpu9250.h"
#include "sensors/proximity.h"
#include "sensors/VL53L0X/VL53L0X.h"
#include "behaviors.h"
#include "button.h"
#include "cmd.h"
#include "config_flash_storage.h"
#include "exti.h"
#include "fat.h"
#include "i2c_bus.h"
#include "ir_remote.h"
#include "leds.h"
#include "memory_protection.h"
#include "motors.h"
#include "sdio.h"
#include "selector.h"
#include "serial_comm.h"
#include "spi_communication.h"
#include "usbcfg.h"
#include "communication.h"
#include "uc_usage.h"

extern messagebus_t bus;
extern parameter_namespace_t parameter_root;

/* Obstacle Avoidance feature */
void obstacle_avoidance(void);

/* Rotation using the Gyro feature */
void rotation_using_gyro(void);

/* Cliff Fall Avoidance feature */
void cliff_fall_avoidance(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TALOS_H_ */
