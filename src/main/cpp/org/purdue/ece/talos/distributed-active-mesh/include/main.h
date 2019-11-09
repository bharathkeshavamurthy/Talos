/*
 * main.h
 *
 *  Created on: Nov 8, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the design of a fully-distributed active mesh within the Talos initiative.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

#ifndef INCLUDE_MAIN_H_
#define INCLUDE_MAIN_H_

/* Macros */
#ifndef MESH_SET_ROOT /* MESH_SET_ROOT */
#define MESH_SET_NODE
#endif /* MESH_SET_ROOT */

#define CORE_0 0
#define CORE_1 1
#define PIN_NUM_CS 5
#define PIN_NUM_CLK 18
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define BUFFER_SIZE 1024
#define SPI_TASK_PRIORITY 5
#define MAX_BUFFER_SIZE 38400
#define SPI_TASK_STACK_SIZE 4096
#define SPI_PACKET_MAX_SIZE 4092

#define DELAY(ticks) (ticks)

/* Constant Declarations */
const int EVENT_DATA_BUFFER_FILL_NEXT = BIT0;
const int EVENT_DATA_BUFFER_FILLED = BIT1;

static const int DEFAULT_RSSI = -120;
static const uint8_t MESH_ID[6] = {0x77, 0x77, 0x77, 0x77, 0x77, 0x77};

/* Variable Declarations */
static int mesh_layer = -1;
static uint8_t last_layer = 0;
static mesh_addr_t mesh_parent_addr;
static EventGroupHandle_t spi_event_group;
static spi_slave_transaction_t spi_slave_transaction;

static spi_bus_config_t spi_bus_config = {
		.miso_io_num = PIN_NUM_MISO,
		.mosi_io_num = PIN_NUM_MOSI,
		.sclk_io_num = PIN_NUM_CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1
};

static spi_slave_interface_config_t spi_slave_interface_config = {
		.mode = 0,
		.spics_io_num = PIN_NUM_CS,
		.queue_size = 2,
		.flags = 0
};

/* Data Buffer State Enumeration */
typedef enum {
	DATA_BUFFER_EMPTY = 0,
	DATA_BUFFER_FULL = 1
} data_buffer_state_t;

/* Data Buffer Structure */
typedef struct {
	data_buffer_state_t data_buffer_state;
	uint8_t *data; // @suppress("Type cannot be resolved")
} data_buffer_t;

/* Function Declaration for Handling Parent Scans in the Mesh */
void mesh_scan_handler(int count);

/* Initialize the SPI communication between the STM32F407uC and the ESP32 radio module */
void spi_init(void);

/* SPI communication handler between the STM32F407uC and the ESP32 radio module */
void spi_task(void);

/* Get the most recent data received from the STM32F407uC */
data_buffer_t* spi_get_data(void);

#endif /* INCLUDE_MAIN_H_ */
