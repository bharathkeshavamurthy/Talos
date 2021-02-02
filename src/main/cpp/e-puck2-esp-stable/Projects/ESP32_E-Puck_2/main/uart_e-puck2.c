/*

File    : uart_e-puck2.h
Author  : Stefano Morgani
Date    : 10 January 2018
REV 1.0

Functions to configure and use the UART communication between the ESP32 and both the main processor (F407) and the programmer (F413).
*/

#include "driver/uart.h"
#include "soc/uart_struct.h"

#include "main_e-puck2.h"
#include "uart_e-puck2.h"

void echo_task(void *pvParameter)
{
    uint8_t* data = (uint8_t*) malloc(BUF_SIZE);
	int len = 0;
    while(1) {
        //Read data from UART2
		len = uart_read_bytes(UART_NUM_2, data, 1, 20 / portTICK_RATE_MS);
		if(len > 0) {
			//Write data back to UART0 and UART2
			//uart_write_bytes(UART_NUM_0, (const char*) data, len);
			uart_write_bytes(UART_NUM_2, (const char*) data, len);
		}
    }
}

void uart_init(void) {
    uart_config_t uart_config = {
        .baud_rate = 2500000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    //Configure parameters.
    //uart_param_config(UART_NUM_0, &uart_config);
	uart_param_config(UART_NUM_2, &uart_config);
    //UART0 uses default pins.
    //Set UART2 pins(TX: GPIO17, RX: GPIO34)
    uart_set_pin(UART_NUM_2, 17, 34, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    //Install UART driver (we don't need an event queue here)
    //In this example we don't even use a buffer for sending data.
	//uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);
}
