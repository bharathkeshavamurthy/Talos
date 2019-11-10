/*
 * main.c
 *
 *  Created on: Nov 8, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the design of a fully-distributed active mesh within the Talos initiative.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

/* This entity has been derived from the WiFi capabilities of the ESP32 and several ESP-IDF utilities along with the interface code for the e-puck2 */

/* Includes */
#include <string.h>

#include "main.h"
#include "mesh.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_attr.h"
#include "esp_mesh.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_system.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/spi_slave.h"
#include "freertos/FreeRTOS.h"
#include "esp_mesh_internal.h"
#include "freertos/xtensa_api.h"
#include "freertos/event_groups.h"

/* Variable Declarations */
uint8_t *spi_tx_buffer;
uint8_t *spi_rx_buffer;
data_buffer_t *data_buffer_1;
data_buffer_t *data_buffer_2;
data_buffer_t *data_buffer_previous;
data_buffer_t *data_buffer_current;

/* Handler routine for handling Parent Scans in the Mesh */
void mesh_scan_handler(int count) {
	/* Initialize the parent association struct */
	mesh_assoc_t associated_parent = { .layer = CONFIG_MESH_MAX_LAYER, .rssi = DEFAULT_RSSI };
	/* Initialize the parent record struct */
	wifi_ap_record_t associated_parent_record = {0};
	/* Initialize the parent config struct */
	wifi_config_t associated_parent_config = {0};
	/* Initialize the scan config struct */
	wifi_scan_config_t scan_config = {0};

	/* Internal variables */
	int ie_len = 0;
	int my_layer = -1;
	unsigned int parent_found = 0;

	/* Temporary members */
	mesh_type_t my_type = MESH_IDLE;
	mesh_assoc_t association;
	wifi_ap_record_t record;

	/* For all the APs scanned before the MESH_EVENT_SCAN_DONE event was posted */
	for (int i=0; i<count; i++) {
		/* Get mesh networking IE length of one AP */
		ESP_ERROR_CHECK(esp_mesh_scan_get_ap_ie_len(&ie_len));
		/* Get AP record */
		ESP_ERROR_CHECK(esp_mesh_scan_get_ap_record(&record, &association));

		/* IEEE 802.11s Mesh Information Element (IE) validity check */
		if (ie_len == sizeof(association)) {
#ifdef MESH_SET_NODE /* MESH_SET_NODE */
			/* The AP under consideration for being my parent is not idle, is allowed in this layer in terms of layer capacity, allows more child connections to it, and is pretty good in terms of received signal strength */
			if (association.mesh_type != MESH_IDLE && association.layer_cap && association.assoc < association.assoc_cap && record.rssi > -70) {
				/* Layer correctness check and Layer2 capacity check */
				if (association.layer < associated_parent.layer || association.layer2_cap < associated_parent.layer2_cap ) {
					/* Parent Found: Extract the record and the IE */
					parent_found = 1;
					memcpy(&associated_parent_record, &record, sizeof(record));
					memcpy(&associated_parent, &association, sizeof(association));
					/* If the layer capacity of the parent is not 1, I am a MESH_NODE else, I'm a MESH_LEAF */
					if (associated_parent.layer_cap != 1) {
						my_type = MESH_NODE;
					} else {
						my_type = MESH_LEAF;
					}
					/* My layer is my parent's layer + 1 */
					my_layer = associated_parent.layer + 1;
					break;
				}
			}
#endif /* MESH_SET_NODE */
		} else {
#ifdef MESH_SET_ROOT /* MESH_SET_ROOT */
			/* If the parent is not the router, set me as the root and my layer as the root layer */
			if (!strcmp(CONFIG_MESH_ROUTER_SSID, (char *) record.ssid)) {
				parent_found = true;
				memcpy(&associated_parent_record, &record, sizeof(record));
				my_type = MESH_ROOT;
				my_layer = MESH_ROOT_LAYER;
			}
#endif /* MESH_SET_ROOT */
		}
	}

	/* Flush the scan results */
	esp_mesh_flush_scan_result();
	if (parent_found == 1) {
		/* If a parent was found in the scan phase, populate the configs with the authmode, parent's bssid, bssid_set, channel, etc. */
		memcpy(&associated_parent_config.sta.ssid, &associated_parent_record.ssid, sizeof(associated_parent_record.ssid));
		associated_parent_config.sta.channel = associated_parent_record.primary;
		associated_parent_config.sta.bssid_set = 1;
		memcpy(&associated_parent_config.sta.bssid, &associated_parent_record.bssid, 6);
		ESP_ERROR_CHECK(esp_mesh_set_ap_authmode(associated_parent_record.authmode));
		/* If I'm the root, the router is my parent: use the CONFIG_MESH_ROUTER credentials to connect */
		if (my_type == MESH_ROOT) {
			if (associated_parent_record.authmode != WIFI_AUTH_OPEN) {
				memcpy(&associated_parent_config.sta.password, CONFIG_MESH_ROUTER_PASSWORD, strlen(CONFIG_MESH_ROUTER_PASSWORD));
			}
		}
		/* Else, my parent is an AP: use the CONFIG_MESH_AP credentials to connect */
		else {
			if (associated_parent_record.authmode != WIFI_AUTH_OPEN) {
				memcpy(&associated_parent_config.sta.password, CONFIG_MESH_AP_PASSWORD, strlen(CONFIG_MESH_AP_PASSWORD));
			}
		}
		/* Set the parent */
		ESP_ERROR_CHECK(esp_mesh_set_parent(&associated_parent_config, (mesh_addr_t *)&associated_parent.mesh_id, my_type, my_layer));
	} else {
		/* If no parents were found, you're currently orphaned: disable crypto funcs and scan again with new configurations (show_hidden, passive scan, and non-blocking) */
		ESP_ERROR_CHECK(esp_mesh_set_ie_crypto_funcs(NULL));
		esp_wifi_scan_stop();
		scan_config.show_hidden = 1;
		scan_config_scan_type = WIFI_SCAN_TYPE_PASSIVE;
		ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 0));
	}
}

/* The peer to peer Rx task routine */
void p2p_rx(void *arg) {
	/* Initializing internal members */
	int flag = 0;
	int received_count = 0;
	int send_count = 0;
	unsigned int is_running = 1;
	mesh_data_t data = {
			.data = rx_buffer,
			.size = RX_SIZE,
	};
	mesh_addr_t sender;

	while (is_running) {
		data.size = RX_SIZE;
		error_code = esp_mesh_recv(&sender, &data, portMAX_DELAY, &flag, NULL, 0);
		if (error_code != ESP_OK || !data.size) {
			continue;
		}
		if (data.size >= size(send_count)) {
			send_count = (data.data[25] << 24) | (data.data[24] << 16) | (data.data[23] << 8) | data.data[22];
		}
		received_count++;
	}
	vTaskDelete(NULL);
}

/* The peer to peer Tx task routine */
void p2p_tx(void) {
	unsigned int is_running = 1;
	mesh_addr_t route_table[CONFIG_MESH_ROUTE_TABLE_SIZE];
	mesh_data_t data = {
			.data = tx_buffer,
			.size = TX_SIZE,
			.proto = MESH_PROTO_BIN,
			.tos = MESH_TOS_P2P
	};
	int send_count = 0;
	int route_table_size = 0;
	while (is_running) {
		esp_mesh_get_routing_table((mesh_addr_t *) &route_table, CONFIG_MESH_ROUTE_TABLE_SIZE * 6, &route_table_size);
		send_count++;
		tx_buffer[25] = (send_count >> 24) & 0xFF;
		tx_buffer[24] = (send_count >> 16) & 0xFF;
		tx_buffer[23] = (send_count >> 8) & 0xFF;
		tx_buffer[22] = send_count & 0xFF;
		for (int i=0; i<route_table_size; i++) {
			ESP_ERROR_CHECK(esp_mesh_send(&route_table[i], &data, MESH_DATA_P2P, NULL, 0));
		}
		if (route_table_size < 10) {
			vTaskDelay(1 * 1000 / portTICK_RATE_MS);
		}
	}
	vTaskDelete(NULL);
}

/* Start peer to peer communication between me and my parent */
esp_err_t start_p2p_communication(void) {
	/* If p2p communication hasn't started, start it by creating Tx and Rx tasks */
	if (has_p2p_communication_started == 0) {
		has_p2p_communication_started = 1;
		xTaskCreate(p2p_tx, "P2P_TX", P2P_COMMUNICATION_TASK_STACK_DEPTH, NULL, P2P_COMMUNICATION_TASK_PRIORITY, NULL);
		xTaskCreate(p2p_rx, "P2P_RX", P2P_COMMUNICATION_TASK_STACK_DEPTH, NULL, P2P_COMMUNICATION_TASK_PRIORITY, NULL);
	}
	return ESP_OK;
}

/* A routine to handle MESH_EVENTs */
void mesh_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
	wifi_scan_config_t scan_config = {0};

	switch(event_id) {

	case MESH_EVENT_STARTED: {
		/* Get the value of the current layer in this Mesh network */
		mesh_layer = esp_mesh_get_layer();
		/* Disable default self_organization and parent_selection features */
		ESP_ERROR_CHECK(esp_mesh_set_self_organized(0, 0));
		/* Stop scanning and enable detecting APs which are hidden. Furthermore, enable passive scans. */
		esp_wifi_scan_stop();
		scan_config.show_hidden = 1;
		scan_config.scan_type = WIFI_SCAN_TYPE_PASSIVE;
		/* Start scanning (async) with this new configuration */
		ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 0));
	}
	break;

	case MESH_EVENT_PARENT_CONNECTED: {
		/* Parse the received event_data which is a MESH_EVENT_PARENT_CONNECTED event */
		mesh_event_connected_t *connected = (mesh_event_connected_t *) event_data;
		/* The parent's layer */
		mesh_layer = connected->self_layer;
		last_layer = mesh_layer;
		/* Extract the parent's MAC address */
		memcpy(&mesh_parent_addr.addr, connected->connected.bssid, 6);
		if (esp_mesh_is_root()) {
			/* Start the DHCP client */
			tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA);
		}
		/* Start p2p communication with my parent */
		start_p2p_communication();
	}
	break;

	case MESH_EVENT_PARENT_DISCONNECTED: {
		/* Parse the received event_data which is a MESH_EVENT_PARENT_DISCONNECTED event */
		mesh_event_disconnected_t *disconnected = (mesh_event_disconnected_t *) event_data;
		/* Get the value of the current layer in this Mesh network */
		mesh_layer = esp_mesh_get_layer();
		/* If too many associations if the reason the parent disconnected from this node, stop scanning and start scanning again with new configurations (show_hidden, passive, non-blocking) */
		if (disconnected->reason == WIFI_REASON_ASSOC_TOOMANY) {
			esp_wifi_scan_stop();
			scan_config.show_hidden = 1;
			scan_config.scan_type = WIFI_SCAN_TYPE_PASSIVE;
			ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 0));
		}
	}
	break;

	case MESH_EVENT_LAYER_CHANGE: {
		/* Parse the received event_data which is a MESH_EVENT_LAYER_CHANGE event */
		mesh_event_layer_change_t *layer_change = (mesh_event_layer_change_t *) event_data;
		/* This node's new layer */
		mesh_layer = layer_change->new_layer;
		last_layer = mesh_layer;
	}
	break;

	case MESH_EVENT_SCAN_DONE: {
		/* Parse the received event_data which is a MESH_EVENT_SCAN_DONE event */
		mesh_event_scan_done_t *scan_done = (mesh_event_scan_done_t *) event_data;
		/* Call the mesh_scan_handler routine */
		mesh_scan_handler(scan_done->number);
	}
	break;

	case MESH_EVENT_STOPPED: {
		/* Get the value of the current layer in this Mesh network */
		mesh_layer = esp_mesh_get_layer();
	}
	break;

	default: {
		/* Just log it */
		ESP_LOGI(MESH_TAG, "Mesh Event ID: %d", event_id);
	}
	break;

	}
}

/* Get the data pointer over SPI */
data_buffer_t* spi_get_data(void) {
	/* If the current data buffer is empty, wait for the buffer to be filled with new information */
	if (data_buffer_current->data_buffer_state == DATA_BUFFER_EMPTY) {
		xEventgroupWaitbits(spi_event_group, EVENT_DATA_BUFFER_FILLED, true, false, portMAX_DELAY);
	}
	/* Clear the EVENT_DATA_BUFFER_FILLED bit */
	xEventGroupClearBits(spi_event_group, EVENT_DATA_BUFFER_FILLED);
	/* Extract the new information and prepare the buffers for the next transaction */
	data_buffer_previous = data_buffer_current;
	data_buffer_current = (data_buffer_current == data_buffer_1) ? data_buffer_2 : data_buffer_1;
	data_buffer_current->data_buffer_state = DATA_BUFFER_EMPTY;
	/* Fill the next buffer by setting the EVENT_DATA_BUFFER_FILL_NEXT bit in the spi_event_group */
	xEventGroupSetBits(spi_event_group, EVENT_DATA_BUFFER_FILL_NEXT);
	/* Return the extracted information from this read cycle */
	return data_buffer_previous;
}

/* The core SPI task */
void spi_task(void) {
	/* Allocate the buffers and configure the spi_slave_transaction */
	memset(spi_tx_buffer, 0x01, SPI_PACKET_MAX_SIZE);
	memset(spi_rx_buffer, 0x01, SPI_PACKET_MAX_SIZE);
	memset(&spi_slave_transaction, 0, sizeof(spi_slave_transaction));
	spi_slave_transaction.tx_buffer = spi_tx_buffer;
	spi_slave_transaction.rx_buffer = spi_rx_buffer;
	spi_slave_transaction.length = SPI_PACKET_MAX_SIZE * 8;
	spi_slave_transaction.user = (void *) 0;

	/* Internal members */
	uint8_t spi_state = 0;
	uint16_t number_of_packets = 0;
	uint32_t remaining_bytes = 0;
	uint8_t packet_id = 0;

	/* Forever */
	while (1) {
		/* Enable data reception from the SPI Master */
		spi_tx_buffer[0] = 1;
		/* Get the values of the stepper motors: -"E" => 2's complement of Hex(ASCII(E)) = 2's complement of Hex(69) = 2's complement of 45 = 2's complement of (0100 0101) = 1011 1011 = 0xBB */
		spi_tx_buffer[1] = 0xBB;
		switch(spi_state) {
		/* Prepare to read the header */
		case 0: {
			spi_slave_transaction.rx_buffer = spi_rx_buffer;
			/* Always clear this transaction length bit because I use it to determine the number of bits received in the next transaction with the master */
			spi_slave_transaction.trans_len = 0;
			spi_state = 1;
		}
		break;
		/* Read the header */
		case 1: {
			ESP_ERROR_CHECK(spi_slave_transmit(VSPI_HOST, &spi_slave_transaction, portMAX_DELAY));
			if (spi_slave_transaction.trans_len == 0) {
				break;
			/* Header correctness check */
			} else if (spi_slave_transaction.trans_len == 12 * 8) {
				spi_state = 2;
			} else {
				spi_state = 0;
			}
		}
		break;
		/* Prepare to read data from the master except for some remaining bytes that'll be left over... */
		case 2: {
			number_of_packets = MAX_BUFFER_SIZE / SPI_MAX_PACKET_SIZE;
			packet_id = 0;
			spi_state = 3;
		}
		break;
		/* Read data (there's some data left over...) */
		case 3: {
			spi_slave_transaction.rx_buffer = &(data_buffer_current->data(packet_id * SPI_MAX_PACKET_SIZE));
			spi_slave_transaction.trans_len = 0;
			ESP_ERROR_CHECK(spi_slave_transmit(VSPIHOST, &spi_slave_transaction, portMAX_DELAY));
			if (spi_slave_transaction.trans_len == 0) {
				break;
			} else if (spi_slave_transaction.trans_len == SPI_MAX_PACKET_SIZE * 8) {
				packet_id += 1;
				if (packet_id == number_of_packets) {
					spi_state = 4;
				}
			} else {
				spi_state = 0;
			}
		}
		break;
		/* Prepare to read the leftovers */
		case 4: {
			remaining_bytes = MAX_BUFFER_SIZE % SPI_MAX_PACKET_SIZE;
			spi_slave_transaction.rx_buffer = &(data_buffer_current->data[packet_id * SPI_MAX_PACKET_SIZE]);
			spi_slave_transaction.trans_len = 0;
			spi_state = 5;
		}
		break;
		/* Read the leftovers */
		case 5: {
			ESP_ERROR_CHECK(spi_slave_transmit(VSPI_HOST, &spi_slave_transaction, portMAX_DELAY));
			if (spi_slave_transaction.trans_len == 0) {
				break;
			} else if (spi_slave_transaction.trans_len == remaining_bytes * 8) {
				data_buffer_current->data_buffer_state = DATA_BUFFER_FILLED;
				spi_state = 6;
				xEventGroupSetBits(spi_event_group, EVENT_DATA_BUFFER_FILLED);
			} else {
				spi_state = 0;
			}
		}
		break;
		/* Next "new" transaction */
		case 6: {
			spi_state = 0;
		}
		break;
		default: {
			ESP_LOGW(MESH_TAG, "Invalid SPI State: %d", spi_state);
		}
		break;
		}
	}
}

/* Initialize the SPI communication interface between the STM32F407uC and the ESP32 radio module */
void spi_init(void) {
	/* Allocate the SPI Tx/Rx/Data Buffers */
	spi_tx_buffer = (uint8_t *) heap_caps_malloc(SPI_PACKET_MAX_SIZE, MALLOC_CAP_DMA);
	spi_rx_buffer = (uint8_t *) heap_caps_malloc(SPI_PACKET_MAX_SIZE, MALLOC_CAP_DMA);
	data_buffer_1 = (data_buffer_t *) heap_caps_calloc(1, sizeof(data_buffer_t), MALLOC_CAP_DMA);
	data_buffer_1->data = (uint8_t *) heap_caps_calloc(MAX_BUFFER_SIZE, 1, MALLOC_CAP_DMA);
	data_buffer_2 = (data_buffer_t *) heap_caps_calloc(1, sizeof(data_buffer_t), MALLOC_CAP_DMA);
	data_buffer_2->data = (uint8_t *) heap_caps_calloc(MAX_BUFFER_SIZE, 1, MALLOC_CAP_DMA);
	if (spi_tx_buffer == NULL || spi_rx_buffer == NULL || data_buffer_1->data == NULL || data_buffer_2->data == NULL) {
		ESP_LOGE(MESH_TAG, "Memory Allocation Failed for one of the SPI Tx or Rx or Data Buffers!");
	}
	data_buffer_previous = data_buffer_1;
	data_buffer_current = data_buffer_2;
	data_buffer_current->data_buffer_state = DATA_BUFFER_EMPTY;

	/* Create an SPI Event Group */
	spi_event_group = xEventGroupCreate();

	/* Enable pull-ups on the SPI input lines (MOSI, CLK, and CS) in order to prevent rogue pulses from being detected on these lines when the master is not connected */
	gpio_set_pull_mode(PIN_NUM_MOSI, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(PIN_NUM_CLK, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(PIN_NUM_CS, GPIO_PULLUP_ONLY);

	/* SPI Bus Initialization */
	ESP_ERROR_CHECK(spi_slave_initialize(VSPI_HOST, &spi_bus_config, &spi_slave_interface_config, 1));
}

/* Run Trigger */
/* ESP_ERROR_CHECK: An external wrapper to monitor error codes returned by the task under analysis -> Subsequent return upon success OR Invoke abort and print the errors upon failure */
void app_main(void) {
	/* Primary Initializations */
	/* Non-Volatile Flash Storage Initialization */
	ESP_ERROR_CHECK(nvs_flash_init());
	/* TCP/IP Adapter Initialization */
	tcpip_adapter_init();

	/* TCP/IP Adapter Configurations */
	/* Stop DHCP Server on this node if it is an Access Point */
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
	/* Stop DHCP Client on this node if it is a Station */
	ESP_ERROR_CHECK(tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA));

	/* Event Loop Creation */
	/* Create a default event loop (queue, mutex, events posted metric, events dropped metric, and queue pointer) */
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	/* WiFi Initialization */
	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	/* Use the Flash Storage on-board the ESP32 */
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));

	/* Start WiFi */
	ESP_ERROR_CHECK(esp_wifi_start());

	/* Mesh Initialization */
	ESP_ERROR_CHECK(esp_mesh_init());
	/* Register the mesh_event_handler() routine as the Handler for MESH_EVENTs */
	ESP_ERROR_CHECK(esp_event_handler_register(MESH_EVENT, ESP_EVENT_ANY_ID, &mesh_event_handler, NULL));
	/* Enable default IE Crypto in the Mesh */
	mesh_cfg_t mesh_init_config = MESH_INIT_CONFIG_DEFAULT();
	/* Add the MESH_ID to the Mesh Configurations */
	memcpy((uint8_t *) &mesh_init_config.mesh_id, MESH_ID, 6);
	/* Configure the Router for this Mesh */
	mesh_init_config.channel = CONFIG_MESH_CHANNEL;
	memcpy((uint8_t *) &mesh_init_config.router.ssid, CONFIG_MESH_ROUTER_SSID, strlen(CONFIG_MESH_ROUTER_SSID));
	memcpy((uint8_t *) &mesh_init_config.router.password, CONFIG_MESH_ROUTER_PASSWORD, strlen(CONFIG_MESH_ROUTER_PASSWORD));
	/* Configure the node with AP credentials if it is an Access Point */
	ESP_ERROR_CHECK(esp_mesh_set_ap_authmode(CONFIG_MESH_AP_AUTHMODE));
	mesh_init_config.mesh_ap.max_connection = CONFIG_MESH_AP_MAX_CONNECTIONS;
	memcpy((uint8_t *) &mesh_init_config.mesh_ap.password, CONFIG_MESH_AP_PASSWORD, strlen(CONFIG_MESH_AP_PASSWORD));
	/* Set the Configurations member to the Mesh being created */
	ESP_ERROR_CHECK(esp_mesh_set_config(&mesh_init_config));

	/* Start the Mesh */
	ESP_ERROR_CHECK(esp_mesh_start());

	/* Initialize SPI */
	spi_init();
	/* Create a pinned SPI communication task */
	xTaskCreatePinnedToCore(spi_task, "SPI Task", SPI_TASK_STACK_SIZE, NULL, SPI_TASK_PRIORITY, NULL, CORE_1);
}
