/*

File    : socket_e-puck2.c
Author  : Stefano Morgani
Date    : 22 March 2018
REV 1.0

Functions to configure and use the socket to exchange data through WiFi.
*/
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"

#include "socket_e-puck2.h"
#include "main_e-puck2.h"
#include "spi_e-puck2.h"
#include "esp_log.h"
#include "rgb_led_e-puck2.h"
#include "uart_e-puck2.h"

#define TCP_PORT 1000
#define TAG "socket:"
#define MAX_BUFF_SIZE 38400 // For the image.
#define SPI_PACKET_MAX_SIZE 4092

const int CONNECTED_BIT = BIT0;
const int DISCONNECTED_BIT = BIT1;

/* FreeRTOS event group to signal when we are connected & ready to send data */
static EventGroupHandle_t socket_event_group;

int get_socket_error_code(int socket)
{
    int result;
    u32_t optlen = sizeof(int);
    int err = getsockopt(socket, SOL_SOCKET, SO_ERROR, &result, &optlen);
    if (err == -1) {
        ESP_LOGE(TAG, "getsockopt failed:%s", strerror(err));
        return -1;
    }
    return result;
}

int show_socket_error_reason(const char *str, int socket)
{
    int err = get_socket_error_code(socket);

    if (err != 0) {
        ESP_LOGW(TAG, "%s socket error %d %s", str, err, strerror(err));
    }

    return err;
}

void socket_task(void *pvParameter) {
	int server_sock = 0, client_sock = 0;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	uint8_t conn_state = 0;
	image_buffer_t* img_buff = NULL;
	uint16_t num_packets = 0;
	uint32_t remaining_bytes = 0;
	unsigned int packet_id = 0;
    EventBits_t evg_bits;
	int16_t len = 0;
	sensors_buffer_t* sensors_buff = NULL;
	uint8_t actuators_buff[ACTUATORS_BUFF_LEN], actuators_buff_last[ACTUATORS_BUFF_LEN]; // Packet id (1) + behavior/others (1) + speed left (2) + speed right (2) + LEDs (1) + RGB LEDs (12) + sound (1)	
	uint8_t header[1];
	uint8_t conn_error = 0;
	uint8_t valid_cmd_received = 0;
	int rx_bytes_not_read = 0;
	int16_t bytes_sent = 0;
	uint16_t num_cmd_packets = 0;
	
	while(1) {
		evg_bits = xEventGroupGetBits(socket_event_group);
		if (evg_bits & DISCONNECTED_BIT) {
			close(server_sock);
			conn_state = 0;
		}
		
		switch(conn_state) {	
			case 0: // Wait connection to the AP.
				printf("socket_server: waiting for start bit\n");
				xEventGroupWaitBits(socket_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
				conn_state = 1;
				break;
				
			case 1: // Create TCP server.
				server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (server_sock < 0) {
					show_socket_error_reason("create_server", server_sock);
					break;
				}

				server_addr.sin_family = AF_INET;
				server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
				server_addr.sin_port = htons(TCP_PORT);
				if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
					show_socket_error_reason("bind_server", server_sock);
					close(server_sock);
					break;
				}
				if (listen(server_sock, 1) < 0) {
					show_socket_error_reason("listen_server", server_sock);
					close(server_sock);
					break;
				}
				conn_state = 2;
				break;
				
			case 2: // Wait connection from a peer.
				//rgb_update_led2(0, 100, 0);
				rgb_led2_gpio_set(1, 0, 1);
				valid_cmd_received = 0;
    		    printf("socket_server: waiting for connection\n");
    		    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    		    if (client_sock < 0) {
    		        show_socket_error_reason("accept_server", client_sock);
    		        close(server_sock);
					conn_state = 1;
    		        break;
    		    }
    		    printf("socket_server: connection established\n");
    		    conn_state = 3;
				conn_error = 0;
				memset(actuators_buff_last, 0, ACTUATORS_BUFF_LEN);
				
				/*
				int n;
				unsigned int m = sizeof(n);
				getsockopt(client_sock, SOL_SOCKET, SO_RCVBUF,(void *)&n, &m);
				printf("rx buff size = %d\r\n", n);
				getsockopt(client_sock, SOL_SOCKET, SO_SNDBUF,(void *)&n, &m);
				printf("tx buff size = %d\r\n", n);
				*/
				break;
				
			case 3: // Receive commands (new actuators values).	
				ioctl(client_sock, FIONREAD, &rx_bytes_not_read );
				//printf("rx state = %d\r\n", rx_bytes_not_read);
				
				if(rx_bytes_not_read >= ACTUATORS_BUFF_LEN*5) { // Flush the input buffer to keep the robot responsive to recent commands.
					//printf("flushing rx...\r\n");
					num_cmd_packets = rx_bytes_not_read/ACTUATORS_BUFF_LEN;
					if((num_cmd_packets*ACTUATORS_BUFF_LEN) == rx_bytes_not_read) {	// Keep the last command valid.
						for(int i=0; i<(num_cmd_packets-1); i++) {
							recv(client_sock, &actuators_buff[0], ACTUATORS_BUFF_LEN, MSG_DONTWAIT);
						}
					} else { // The new command is partially received, so keep it and remove all the previous ones.
						//printf("b");
						for(int i=0; i<num_cmd_packets; i++) {
							recv(client_sock, &actuators_buff[0], ACTUATORS_BUFF_LEN, MSG_DONTWAIT);
						}
					}
				}	
				
				len = recv(client_sock, &actuators_buff[0], ACTUATORS_BUFF_LEN, MSG_DONTWAIT);
				if(len < 0) {
					if(get_socket_error_code(client_sock) != 11) { // When error code is 11, it means that no data was available, so continue normally with the next state, otherwise terminate the connection.
						show_socket_error_reason("recv_cmd", client_sock);
						conn_state = 2;
						break;
					}
				} else if((len==ACTUATORS_BUFF_LEN) && (actuators_buff[0]==0x80)) {
					valid_cmd_received = 1;
					memcpy(actuators_buff_last, actuators_buff, ACTUATORS_BUFF_LEN);					
				} else {
					//printf("received only %d bytes, wait for the remaining\r\n", len);
					len += recv(client_sock, &actuators_buff[len], ACTUATORS_BUFF_LEN-len, MSG_WAITALL);
					memcpy(actuators_buff_last, actuators_buff, ACTUATORS_BUFF_LEN);
				}
				
				//printf("tx state = %d, %d\r\n", get_snd_buf(client_sock), get_snd_queuelen(client_sock));
								
				if(valid_cmd_received == 1) { // Wait for at least the first command before sending back anything.
					//printf("cmd len = %d (%X)\n", len, actuators_buff[0]);
					//if(actuators_buff[ACTUATORS_BUFF_LEN-1]!=0) {
					//	printf("aaaahhh!!!\r\n");
					//}
					// Check id == 0x80 needed??
					uart_set_actuators_state(actuators_buff_last);
					if(actuators_buff_last[1] == 0x00) {
						conn_state = 6;
					} else if(actuators_buff_last[1]&0x01) {
						conn_state = 4; // Send image first.
					} else if(actuators_buff_last[1]&0x02) {
						conn_state = 5; // Send only sensors.
					}
				}				
				break;				
				
			case 4: // Exchanging image.
				//rgb_led2_gpio_set(0, 1, 1); // Turn on red.
				img_buff = spi_get_data_ptr(1);				
				//rgb_led2_gpio_set(1, 1, 1); // Turn off all.			
    		    num_packets = MAX_BUFF_SIZE/SPI_PACKET_MAX_SIZE;
    		    remaining_bytes = MAX_BUFF_SIZE%SPI_PACKET_MAX_SIZE;
				rgb_led2_gpio_set(1, 1, 0); // Turn on blue.
				header[0] = 0x01;
    			if( send(client_sock, header, 1, 0) < 0) { // Send id=0x01
    				show_socket_error_reason("send_image_header", client_sock);
    				conn_state = 2;
					break;
    			}				
    			for(packet_id=0; packet_id<num_packets; packet_id++) {
    				if((len=send(client_sock, &(img_buff->data[SPI_PACKET_MAX_SIZE*packet_id]), SPI_PACKET_MAX_SIZE, 0)) < 0) {
    					show_socket_error_reason("send_data", client_sock);
    					conn_error = 1;
    					break;
    				}
					//printf("%d)%d\r\n", packet_id, len);
    			}
    			if(remaining_bytes>0 && conn_error==0) { // If there is a last image segment and no errors occurred.
    				if((len=send(client_sock, &(img_buff->data[SPI_PACKET_MAX_SIZE*packet_id]), remaining_bytes, 0)) < 0) {
    					show_socket_error_reason("send_data", client_sock);
    					conn_error = 1;
    				}
					//printf("%d)%d\r\n", packet_id, len);
    			}	
				rgb_led2_gpio_set(1, 1, 1); // Turn off all.
				if(conn_error == 0) {
					if(actuators_buff[1]&0x02) { // Send also sensors.
						conn_state = 5;
					} else {
						uart_get_data_ptr();
						conn_state = 3;
					}
				} else {
					conn_state = 2;
				}
				break;
				
			case 5: // Send sensors values.
				spi_get_data_ptr(0); // This is needed to update the RGB LEDs state when the image isn't exchanged.
				sensors_buff = uart_get_data_ptr();
				rgb_led2_gpio_set(1, 1, 0); // Turn on blue.
				header[0] = 0x02;
    			if( send(client_sock, header, 1, 0) < 0) { // Send id=0x02
    				show_socket_error_reason("send_sensor_header", client_sock);
    				conn_state = 2;
					break;
    			}	
				//printf("Send data\r\n");
				bytes_sent = send(client_sock, &(sensors_buff->data[0]), UART_RX_BUFF_SIZE, 0);				
    			if(bytes_sent < 0) {
    				show_socket_error_reason("send_sensor_data", client_sock);
    				conn_state = 2;
					break;
    			}
				if(bytes_sent < UART_RX_BUFF_SIZE) {
					//printf("sent only %d bytes\r\n", bytes_sent);
				} else {
					//printf("sent\r\n");
				}
				rgb_led2_gpio_set(1, 1, 1); // Turn off all.
				conn_state = 3;
				break;
				
			case 6: // Send empty packet (only id with no payload).
				spi_get_data_ptr(0); // This is needed to update the RGB LEDs state when the image isn't exchanged.
				header[0] = 0x03;
    			if( send(client_sock, header, 1, 0) < 0) { // Send id=0x03
    				show_socket_error_reason("send_empty_header", client_sock);
    				conn_state = 2;
					break;
    			}
				sensors_buff = uart_get_data_ptr();
				conn_state = 3;
				break;
		}
		
    	vTaskDelay( (TickType_t)10); /* allows the freeRTOS scheduler to take over if needed */		
	}

}

void socket_set_event_connected(void) {
	xEventGroupSetBits(socket_event_group, CONNECTED_BIT);
}

void socket_set_event_disconnected(void) {
	xEventGroupSetBits(socket_event_group, DISCONNECTED_BIT);
}

void socket_init(void) {
	socket_event_group = xEventGroupCreate();
}
