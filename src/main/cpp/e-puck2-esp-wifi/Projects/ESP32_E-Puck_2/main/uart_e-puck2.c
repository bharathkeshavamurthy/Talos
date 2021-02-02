/*

File    : uart_e-puck2.h
Author  : Stefano Morgani
Date    : 10 January 2018
REV 1.0

Functions to configure and use the UART communication between the ESP32 and both the main processor (F407) and the programmer (F413).
*/
#include <string.h>

#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "freertos/event_groups.h"

#include "main_e-puck2.h"
#include "uart_e-puck2.h"
#include "rgb_led_e-puck2.h"
#include "utility.h"
#include "button_e-puck2.h"

#define UART_407 UART_NUM_1

const int EVT_SENSORS_BUFF_FILL_NEXT = BIT0;
const int EVT_SENSORS_BUFF_FILLED = BIT1;

uint8_t uart_tx_buff[UART_TX_BUFF_SIZE]; // Tx to F407.
sensors_buffer_t* uart_rx_buff1; // Rx from F407.
sensors_buffer_t* uart_rx_buff2;
sensors_buffer_t* uart_rx_buff_last;
sensors_buffer_t* uart_rx_buff_curr;

static EventGroupHandle_t uart_event_group;

void uart_set_actuators_state(uint8_t *buff) {
	// uart_tx_buff[2] = buff[2]; // Behaviors/others
	// uart_tx_buff[3] = buff[3]; // Left speed / left steps LSB
	// uart_tx_buff[4] = buff[4]; // Left speed / left steps MSB
	// uart_tx_buff[5] = buff[5]; // Right speed / right steps LSB
	// uart_tx_buff[6] = buff[6]; // Right speed / right steps MSB
	// uart_tx_buff[7] = buff[7]; // LEDs
	// uart_tx_buff[8] = buff[20];	// Sound.
	
	// rgb_update_all(&buff[8]);
	
	memcpy(&uart_tx_buff[2], &buff[2], 19);
}

sensors_buffer_t *uart_get_data_ptr(void) {
	// Wait for the next buffer to be filled in case it isn't.
	if(uart_rx_buff_curr->state == SENSORS_BUFF_EMPTY) {
		//rgb_led2_gpio_set(0, 0, 0);
		//printf("wait\r\n");
		xEventGroupWaitBits(uart_event_group, EVT_SENSORS_BUFF_FILLED, true, false, portMAX_DELAY);
	}
	//printf("get filled\r\n");
	xEventGroupClearBits(uart_event_group, EVT_SENSORS_BUFF_FILLED); // This bit remain set if the buffer was already filled, so clear it.
	
	uart_rx_buff_last = uart_rx_buff_curr;
	if(uart_rx_buff_curr == uart_rx_buff1) {
		uart_rx_buff_curr = uart_rx_buff2;
	} else {
		uart_rx_buff_curr = uart_rx_buff1;
	}
	uart_rx_buff_curr->state = SENSORS_BUFF_EMPTY;
	
	xEventGroupSetBits(uart_event_group, EVT_SENSORS_BUFF_FILL_NEXT); // Tell the SPI task to fill the next buffer.
	//printf("set fill next\r\n");
	
	return uart_rx_buff_last;
}

void advsercom_task(void *pvParameter) {
	uint8_t uart_state = 0;
	
	uart_rx_buff_curr = uart_rx_buff1;
	
	int len = 0;
	int flush_len = 0;
	int flush_tot_len = 0;
	uint8_t loop_count = 0;
	uint8_t red_value = 0;
	uint16_t speed_value = 100;
	uint8_t flush_byte = 0;
	uint8_t temp = 0;
	
	// Prepare the requests based on the asercom protocol.
	memset(uart_tx_buff, 0, UART_TX_BUFF_SIZE);
	uart_tx_buff[0]=-0x08;	// (0xF8) All sensors request.
	uart_tx_buff[1]=-0x09;	// (0xF7) All actuators set command.		
	uart_tx_buff[UART_TX_BUFF_SIZE-1]=0;	// Terminator.
	
	while(1) {
		
		switch(uart_state) {
			case 0: // Send requests and commands.
				//rgb_led2_gpio_set(0, 0, 1);	
				// Send the requests based on asercom protocol.
				// Beware: from various tests resulted that at most 24 bytes are correctly sent to the F407, 
				// if you try sending more data than this, then the data are corrupted. To be analysed deeper...
				//if(uart_tx_buff[UART_TX_BUFF_SIZE-2]!=0) {
				//	printf("wrong cmd %d!\r\n", uart_tx_buff[UART_TX_BUFF_SIZE-2]);
				//}
				len = uart_tx_chars(UART_407, (char*)&uart_tx_buff[0], UART_TX_BUFF_SIZE);
				//printf("sending...\r\n");
				//uart_wait_tx_done(UART_407, portMAX_DELAY);	
				if(uart_wait_tx_done(UART_407, 100/portTICK_RATE_MS) != ESP_OK) {
					//printf("error on tx done!\r\n");
					break;
				}
				uart_state = 1;						
				//printf("sent=(%d)\r\n", len);
				break;
				
			case 1: // Receive sensors data.	
				//rgb_led2_gpio_set(0, 1, 0);
				// Read the expected bytes from the requests previously made.
				len = uart_read_bytes(UART_407, uart_rx_buff_curr->data, RESPONSE_SIZE, 50/portTICK_RATE_MS);
					
				// Flush the input buffer in case more data than expected are received.
				flush_len = 0;
				flush_tot_len = 0;
				while(1) {
					flush_len = uart_read_bytes(UART_407, &flush_byte, 1, 5/portTICK_RATE_MS);
					if(flush_len == 0) {
						break;
					}
					flush_tot_len += flush_len;
				}
				
				//printf("flush=(%d)\r\n", flush_tot_len);
				//printf("len=(%d)\r\n", len);
				
				// If all is ok, mark the buffer as ready.
				if(len==RESPONSE_SIZE && flush_tot_len==0) {
				
					temp = uart_rx_buff_curr->data[UART_RX_BUFF_SIZE-2];				
					uart_rx_buff_curr->data[UART_RX_BUFF_SIZE-2] = button_is_pressed(); // Set the button state directly from here because it's logically more correct since the button is connected to the ESP32 (instead of requesting the state from the F407).
																						// Moreover the state of the button is only updated through SPI if there is a WiFi connection opened and the camera image is streamed,
																						// so its state would be updated only if also the camera is requested, that is not the correct behaviour.
					
					uart_rx_buff_curr->data[UART_RX_BUFF_SIZE-1] = temp; // Put the empty byte (reserved for future usage) at the end of the packet.
				
					uart_rx_buff_curr->state = SENSORS_BUFF_FILLED;
					xEventGroupSetBits(uart_event_group, EVT_SENSORS_BUFF_FILLED);
					//printf("set filled\r\n");					
					xEventGroupWaitBits(uart_event_group, EVT_SENSORS_BUFF_FILL_NEXT, true, false, portMAX_DELAY);
					//printf("get fill next\r\n");
				} else {
					vTaskDelay(1000 / portTICK_PERIOD_MS); // Add a pause otherwise the data received by the F407 would be corrupted when the ESP32 and F407 aren't sync.
				}
				
				uart_state = 0;
				break;
		}
	}
}

void uart_init(void) {
    uart_config_t uart_config = {
        .baud_rate = 115200, //2500000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    //Configure parameters.
	uart_param_config(UART_407, &uart_config);
    //UART0 uses default pins.
    //Set UART2 pins(TX: GPIO17, RX: GPIO34)
    uart_set_pin(UART_407, 17, 34, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    //Install UART driver with both RX and TX buffers (we don't need an event queue here)
    uart_driver_install(UART_407, 2048, 2048, 0, NULL, 0);

	uart_rx_buff1 = (sensors_buffer_t*) malloc(sizeof(sensors_buffer_t));	
	uart_rx_buff1->data = (uint8_t*) malloc(UART_RX_BUFF_SIZE);
	if(uart_rx_buff1->data == NULL) {
		printf("cannot allocate uart rx buff1\r\n");
		return;
	} else {
		printf("uart rx buff1 allocated\r\n");
	}	
		
	uart_rx_buff2 = (sensors_buffer_t*) malloc(sizeof(sensors_buffer_t));	
	uart_rx_buff2->data = (uint8_t*) malloc(UART_RX_BUFF_SIZE);
	if(uart_rx_buff2->data == NULL) {
		printf("cannot allocate uart rx buff1\r\n");
		return;
	} else {
		printf("uart rx buff1 allocated\r\n");
	}
	
	uart_rx_buff_last = uart_rx_buff1;
	uart_rx_buff_curr = uart_rx_buff2;
	uart_rx_buff_curr->state = SENSORS_BUFF_EMPTY;	
	
	uart_event_group = xEventGroupCreate();
	
	xTaskCreatePinnedToCore(&advsercom_task, "advsercom_task", 2048, NULL, 4, NULL, CORE_0);	
}
