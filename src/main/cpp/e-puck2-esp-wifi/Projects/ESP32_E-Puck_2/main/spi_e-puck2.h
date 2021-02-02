/*

File    : spi_e-puck2.h
Author  : Stefano Morgani
Date    : 10 January 2018
REV 1.0

Functions to configure and use the SPI communication between the main processor (F407) and the ESP32. 
*/

#ifndef SPI_E_PUCK_2_H
#define SPI_E_PUCK_2_H

#define BUF_SIZE (1024)

#define SPI_TASK_STACK_SIZE	4096
#define SPI_TASK_PRIO		5

typedef enum {
	IMG_BUFF_EMPTY=0,
	IMG_BUFF_FILLED=1
} image_buffer_state_t;

typedef struct {
	image_buffer_state_t state;
	uint8_t* data;
} image_buffer_t;

/**
 * @brief 	SPI communication handling between the F407 and ESP32
 *
 * @param *pvParameter	parameter from the xCreateTask 	
 */
void spi_task(void *pvParameter);

/**
 * @brief 	SPI initialization (VSPI port).
 *
 */
void spi_init(void);

/**
 * @brief Return the pointer to the most recent SPI data received from the STM32.
 *
 * @param wait_buff If 1 then this function blocks until the next image buffer is filled.
 */
image_buffer_t* spi_get_data_ptr(unsigned char wait_buff);

#endif /* SPI_E_PUCK_2_H */
