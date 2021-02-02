/*

File    : spi_e-puck2.c
Author  : Stefano Morgani
Date    : 10 January 2018
REV 1.0

Functions to configure and use the SPI communication between the main processor (F407) and the ESP32. 
*/
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "spi_e-puck2.h"
#include "driver/spi_slave.h"
#include "button_e-puck2.h"
#include "main_e-puck2.h"
#include "rgb_led_e-puck2.h"
//#include "socket_e-puck2.h"
//#include "utility.h"

// Hardware VSPI pins.
#define PIN_NUM_MOSI 23
#define PIN_NUM_MISO 19
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5

#define MAX_BUFF_SIZE 38400 // For the image.
#define SPI_PACKET_MAX_SIZE 4092

const int EVT_IMG_BUFF_FILL_NEXT = BIT0;
const int EVT_IMG_BUFF_FILLED = BIT1;

uint8_t* spi_tx_buff;
uint8_t* spi_rx_buff;
image_buffer_t* image_buff1;
image_buffer_t* image_buff2;
image_buffer_t* image_buff_last;
image_buffer_t* image_buff_curr;

static EventGroupHandle_t spi_event_group;
static spi_slave_transaction_t transaction;

// Configuration for the SPI bus.
static spi_bus_config_t buscfg = {
    .miso_io_num = PIN_NUM_MISO,
    .mosi_io_num = PIN_NUM_MOSI,
    .sclk_io_num = PIN_NUM_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1
};

// Configuration for the SPI slave interface.
static spi_slave_interface_config_t slvcfg = {
    .mode = 0,							// SPI mode0: CPOL=0, CPHA=0.
    .spics_io_num = PIN_NUM_CS,			// CS pin.
    .queue_size = 2,					// We want to be able to queue 3 transactions at a time.
    .flags = 0,
    //.post_setup_cb=my_post_setup_cb,
    //.post_trans_cb=my_post_trans_cb
};

image_buffer_t* spi_get_data_ptr(void) {
	// Wait for the next buffer to be filled in case it isn't.
	if(image_buff_curr->state == IMG_BUFF_EMPTY) {
		xEventGroupWaitBits(spi_event_group, EVT_IMG_BUFF_FILLED, true, false, portMAX_DELAY);
	}
	xEventGroupClearBits(spi_event_group, EVT_IMG_BUFF_FILLED); // This bit remain set if the buffer was already filled, so clear it.
	
	image_buff_last = image_buff_curr;
	if(image_buff_curr == image_buff1) {
		image_buff_curr = image_buff2;
	} else {
		image_buff_curr = image_buff1;
	}
	image_buff_curr->state = IMG_BUFF_EMPTY;
	
	xEventGroupSetBits(spi_event_group, EVT_IMG_BUFF_FILL_NEXT); // Tell the SPI task to fill the next buffer.
	
	return image_buff_last;

}

void spi_task(void *pvParameter) {
	esp_err_t ret;
	uint16_t numPackets = 0;
	uint32_t remainingBytes = 0;	
	unsigned int packetId = 0;
	uint8_t spi_state = 0;
	
	memset(spi_rx_buff, 0x01, SPI_PACKET_MAX_SIZE);
	memset(spi_tx_buff, 0x01, SPI_PACKET_MAX_SIZE);

	memset(&transaction, 0, sizeof(transaction));
	transaction.rx_buffer = spi_rx_buff;
	transaction.tx_buffer = spi_tx_buff;
	transaction.length = SPI_PACKET_MAX_SIZE*8; // Always set to maximum bytes expected and then read "trans_len" to know how many bytes the master actually sent.
	transaction.user=(void*)0;	// Optional user parameter for the callback.
	
#if 0	
// Test single transactions with the STM32.
	for(;;) {
		rgb_set_intensity(LED2, RED_LED, 0, 0);
		rgb_set_intensity(LED2, BLUE_LED, 0, 0);
		rgb_set_intensity(LED2, GREEN_LED, 0, 0);	
	
		/*
		// Small size (header) transaction testing.
		transaction.rx_buffer = spi_rx_buff; 	// Otherwise the last image segment will be corrupted.
		transaction.trans_len = 0; 				// This value tell us how many bytes will be transferred in the next transaction, so clear it now.	
		ret = spi_slave_transmit(VSPI_HOST, &transaction, portMAX_DELAY);
		assert(ret==ESP_OK);				
		if(transaction.trans_len == 12*8) { // Check the correct number of bytes are received.
			rgb_set_intensity(LED2, RED_LED, 0, 0);
			rgb_set_intensity(LED2, BLUE_LED, 0, 0);
			rgb_set_intensity(LED2, GREEN_LED, 100, 0);		
		} else {
			rgb_set_intensity(LED2, RED_LED, 100, 0);
			rgb_set_intensity(LED2, BLUE_LED, 0, 0);
			rgb_set_intensity(LED2, GREEN_LED, 0, 0);			
		}		
		*/
		
		// Big size (image segments) transaction testing.
		transaction.rx_buffer = &(image_buff_curr->data[0]);
		transaction.trans_len = 0;
		ret = spi_slave_transmit(VSPI_HOST, &transaction, portMAX_DELAY);
		assert(ret==ESP_OK);
		if(transaction.trans_len == 0) {
			continue;
		} else if(transaction.trans_len == SPI_PACKET_MAX_SIZE*8) { // Check the correct number of bytes are received.			
			//printf("%d)%d\r\n", image_buff_curr->data[0], transaction.trans_len);			
			rgb_set_intensity(LED2, RED_LED, 0, 0);
			rgb_set_intensity(LED2, BLUE_LED, 0, 0);
			rgb_set_intensity(LED2, GREEN_LED, 100, 0);			
		} else {	
			printf("%d)%d\r\n", image_buff_curr->data[0], transaction.trans_len);
			rgb_set_intensity(LED2, RED_LED, 100, 0);
			rgb_set_intensity(LED2, BLUE_LED, 0, 0);
			rgb_set_intensity(LED2, GREEN_LED, 0, 0);				
			//break;
		}
		
		//vTaskDelay(100 / portTICK_PERIOD_MS);
	}
#endif	

	for(;;) {

		spi_tx_buff[0] = button_is_pressed(); 	// Button status to send to F407.
		spi_tx_buff[1] = 0xB7; 					// Get image.		
		
		switch(spi_state) {
			case 0: // Prepare to read header.
				transaction.rx_buffer = spi_rx_buff; 	// Otherwise the last image segment will be corrupted.
				transaction.trans_len = 0; 				// This value tell us how many bytes will be transferred in the next transaction, so clear it now.	
				spi_state = 1;
				break;
				
			case 1: // Reading header.									
				ret = spi_slave_transmit(VSPI_HOST, &transaction, portMAX_DELAY);
				assert(ret==ESP_OK);
				if(transaction.trans_len == 0) {
					break;
				} else if(transaction.trans_len == 12*8) {
					rgb_update_all(spi_rx_buff);
					spi_state = 2;
				} else {
					spi_state = 0;
				}
				break;
				
			case 2: // Prepare to read all image but the last section.
				numPackets = MAX_BUFF_SIZE/SPI_PACKET_MAX_SIZE;
				packetId = 0;
				spi_state = 3;
				break;
				
			case 3: // Reading all image but the last section.
				transaction.rx_buffer = &(image_buff_curr->data[packetId*SPI_PACKET_MAX_SIZE]);
				transaction.trans_len = 0; 
				ret = spi_slave_transmit(VSPI_HOST, &transaction, portMAX_DELAY);
				assert(ret==ESP_OK);
				if(transaction.trans_len == 0) {
					break;
				} else if(transaction.trans_len == SPI_PACKET_MAX_SIZE*8) {
					packetId++;
					if(packetId == numPackets) {
						spi_state = 4;
					}
				} else {
					spi_state = 0;
				}
				break;				
				
			case 4: // Prepare to read last section of the image.
				remainingBytes = MAX_BUFF_SIZE%SPI_PACKET_MAX_SIZE;
				transaction.rx_buffer = &(image_buff_curr->data[packetId*SPI_PACKET_MAX_SIZE]);
				transaction.trans_len = 0;
				spi_state = 5;
				break;
				
			case 5: // Reading the last section.
				ret = spi_slave_transmit(VSPI_HOST, &transaction, portMAX_DELAY);
				assert(ret==ESP_OK);
				if(transaction.trans_len == 0) {
					break;
				} else if(transaction.trans_len == remainingBytes*8) {
					image_buff_curr->state = IMG_BUFF_FILLED;
					spi_state = 6;
					xEventGroupSetBits(spi_event_group, EVT_IMG_BUFF_FILLED);
				} else {
					spi_state = 0;
				}				
				break;				
				
			case 6: // Waiting next request.
				//xEventGroupWaitBits(spi_event_group, EVT_IMG_BUFF_FILL_NEXT, true, false, portMAX_DELAY);
				spi_state = 0;
				break;
		}
		
	}	
	
}

void spi_init(void) {
	esp_err_t ret;

	spi_tx_buff = (uint8_t*) heap_caps_malloc(SPI_PACKET_MAX_SIZE, MALLOC_CAP_DMA);
	if(spi_tx_buff == NULL) {
		printf("cannot allocate spi tx buff\r\n");
		return;
	} else {
		printf("spi tx buff allocated\r\n");
	}
	spi_rx_buff = (uint8_t*) heap_caps_malloc(SPI_PACKET_MAX_SIZE, MALLOC_CAP_DMA);
	if(spi_rx_buff == NULL) {
		printf("cannot allocate spi rx buff\r\n");
		return;
	} else {
		printf("spi rx buff allocated\r\n");
	}
	image_buff1 = (image_buffer_t*) heap_caps_calloc(1, sizeof(image_buffer_t), MALLOC_CAP_DMA);	
	image_buff1->data = (uint8_t*) heap_caps_calloc(MAX_BUFF_SIZE, 1, MALLOC_CAP_DMA);
	if(image_buff1->data == NULL) {
		printf("cannot allocate image buff1\r\n");
		return;
	} else {
		printf("image buff1 allocated\r\n");
	}

	image_buff2 = (image_buffer_t*) heap_caps_calloc(1, sizeof(image_buffer_t), MALLOC_CAP_DMA);	
	image_buff2->data = (uint8_t*) heap_caps_calloc(MAX_BUFF_SIZE, 1, MALLOC_CAP_DMA);
	if(image_buff2->data == NULL) {
		printf("cannot allocate image buff2\r\n");
		return;
	} else {
		printf("image buff2 allocated\r\n");
	}	

	image_buff_last = image_buff1;
	image_buff_curr = image_buff2;
	image_buff_curr->state = IMG_BUFF_EMPTY;
	
	spi_event_group = xEventGroupCreate();
	
    // Enable pull-ups on SPI lines so we don't detect rogue pulses when no master is connected.
    gpio_set_pull_mode(PIN_NUM_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CS, GPIO_PULLUP_ONLY);
	
    // Initialize the SPI bus.
    ret = spi_slave_initialize(VSPI_HOST, &buscfg, &slvcfg, 1);
    assert(ret==ESP_OK);
}
