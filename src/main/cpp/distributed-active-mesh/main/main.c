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
#include "main.h"

/* Variable Declarations */
int right_motor_speed;
int left_motor_speed;
uint8_t *spi_tx_buffer;
uint8_t *spi_rx_buffer;
data_buffer_t *data_buffer_1;
data_buffer_t *data_buffer_2;
mesh_type_t my_type = MESH_IDLE;
data_buffer_t *data_buffer_previous;
data_buffer_t *data_buffer_current;

/* Handler routine for handling Parent Scans in the Mesh */
void mesh_scan_handler(int count) {
	/* Initialize the parent association struct */
	mesh_assoc_t associated_parent = { .layer = CONFIG_MESH_MAX_LAYERS, .rssi = DEFAULT_RSSI };
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
				parent_found = 1;
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
		scan_config.scan_type = WIFI_SCAN_TYPE_PASSIVE;
		ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 0));
	}
}

/* The peer to peer Rx task routine */
void p2p_rx(void *arg) {
	/* Initializing internal members */
	int flag = 0;
	int received_count = 0;
	esp_err_t error_code;
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
		/* Extract the motor speeds from the Rx buffer */
		right_motor_speed = (data.data[0] << 8) | data.data[1];
		right_motor_speed = (data.data[2] == 0x01) ? right_motor_speed : (-1 * right_motor_speed);
		left_motor_speed = (data.data[3] << 8) | data.data[4];
		left_motor_speed = (data.data[5] == 0x01) ? left_motor_speed : (-1 * left_motor_speed);
		received_count++;
	}
	is_running = 0;
	vTaskDelete(NULL);
}

/* The peer to peer Tx task routine */
void p2p_tx(void *arg) {
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
		/* Prepare the Tx buffer to send the motor speeds to the peers in my route table */
		tx_buffer[0] = (right_motor_speed >> 8) & 0xFF;
		tx_buffer[1] = right_motor_speed & 0xFF;
		tx_buffer[2] = (right_motor_speed >= 0) ? 0x01 : 0x00;
		tx_buffer[3] = (left_motor_speed >> 8) & 0xFF;
		tx_buffer[4] = left_motor_speed & 0xFF;
		tx_buffer[5] = (left_motor_speed >= 0) ? 0x01 : 0x00;
		for (int i=0; i<route_table_size; i++) {
			ESP_ERROR_CHECK(esp_mesh_send(&route_table[i], &data, MESH_DATA_P2P, NULL, 0));
		}
		if (route_table_size < 10) {
			vTaskDelay(1 * 1000 / portTICK_RATE_MS);
		}
	}
	is_running = 0;
	vTaskDelete(NULL);
}

/* Start peer to peer communication between me and my parent */
esp_err_t start_p2p_communication(void) {
	/* If p2p communication hasn't started, start it by creating Tx and Rx tasks */
	if (has_p2p_communication_started == 0) {
		has_p2p_communication_started = 1;
		if (my_type != MESH_LEAF) {
			xTaskCreate(p2p_tx, "P2P_TX", P2P_COMMUNICATION_TASK_STACK_DEPTH, NULL, P2P_COMMUNICATION_TASK_PRIORITY, NULL);
		}
#ifndef MESH_SET_ROOT /* MESH_SET_ROOT */
		xTaskCreate(p2p_rx, "P2P_RX", P2P_COMMUNICATION_TASK_STACK_DEPTH, NULL, P2P_COMMUNICATION_TASK_PRIORITY, NULL);
#endif /* MESH_SET_ROOT */
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
data_buffer_t* spi_get_data_ptr(void) {
	/* If the current data buffer is empty, wait for the buffer to be filled with new information */
	if (data_buffer_current->data_buffer_state == DATA_BUFFER_EMPTY) {
		xEventGroupWaitBits(spi_event_group, EVENT_DATA_BUFFER_FILLED, true, false, portMAX_DELAY);
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
void spi_task(void *arg) {
	/* Allocate the buffers and configure the spi_slave_transaction */
	memset(spi_tx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
	memset(spi_rx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
	memset(&spi_slave_transaction, 0, sizeof(spi_slave_transaction));
	spi_slave_transaction.tx_buffer = spi_tx_buffer;
	spi_slave_transaction.rx_buffer = spi_rx_buffer;
	spi_slave_transaction.length = SPI_MAX_PACKET_SIZE * 8;
	spi_slave_transaction.user = (void *) 0;

	/* Forever */
	while (1) {
#ifdef MESH_SET_ROOT /* MESH_SET_ROOT */
		/* RGB LEDs transaction */
		memset(spi_tx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		memset(spi_rx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		spi_slave_transaction.tx_buffer = spi_tx_buffer;
		spi_slave_transaction.rx_buffer = spi_rx_buffer;
		spi_slave_transaction.trans_len = 0;
		ESP_ERROR_CHECK(spi_slave_transmit(VSPI_HOST, &spi_slave_transaction, portMAX_DELAY));
		if (spi_slave_transaction.trans_len == 12 * 8) {
			rgb_update_all(spi_rx_buffer);
			data_buffer_current->data_buffer_state = EVENT_DATA_BUFFER_FILLED;
			xEventGroupSetBits(spi_event_group, EVENT_DATA_BUFFER_FILLED);
		}
#endif /* MESH_SET_ROOT */

#ifndef MESH_SET_ROOT /* MESH_SET_ROOT */
		/* Slave driving the Master's motors */
		memset(spi_tx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		memset(spi_rx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		spi_slave_transaction.rx_buffer = spi_rx_buffer;
		spi_slave_transaction.trans_len = 0;
		spi_tx_buffer[0] = (right_motor_speed >> 8) & 0xFF;
		spi_tx_buffer[1] = right_motor_speed & 0xFF;
		spi_tx_buffer[2] = (right_motor_speed >= 0) ? 0x01 : 0x00;
		spi_tx_buffer[3] = (left_motor_speed >> 8) & 0xFF;
		spi_tx_buffer[4] = left_motor_speed & 0xFF;
		spi_tx_buffer[5] = (left_motor_speed >= 0) ? 0x01 : 0x00;
		spi_slave_transaction.tx_buffer = spi_tx_buffer;
		/* Execute the transaction with the Master */
		ESP_ERROR_CHECK(spi_slave_transmit(VSPI_HOST, &spi_slave_transaction, portMAX_DELAY));
#endif /* MESH_SET_ROOT */

#ifdef MESH_SET_ROOT /* MESH_SET_ROOT */
		/* Receive the motor information from the Master in order to send this information to my peers */
		memset(spi_tx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		memset(spi_rx_buffer, 0xFF, SPI_MAX_PACKET_SIZE);
		spi_slave_transaction.tx_buffer = spi_tx_buffer;
		spi_slave_transaction.rx_buffer = spi_rx_buffer;
		spi_slave_transaction.trans_len = 0;
		/* Execute the transaction with the Master */
		ESP_ERROR_CHECK(spi_slave_transmit(VSPI_HOST, &spi_slave_transaction, portMAX_DELAY));
		if (spi_slave_transaction.trans_len == 6 * 8) {
			right_motor_speed = (spi_rx_buffer[0] << 8) | spi_rx_buffer[1];
			right_motor_speed = (spi_rx_buffer[2] == 0x01) ? right_motor_speed : (-1 * right_motor_speed);
			left_motor_speed = (spi_rx_buffer[3] << 8) | spi_rx_buffer[4];
			left_motor_speed = (left_motor_speed[5] == 0x01) ? left_motor_speed : (-1 * left_motor_speed);
			data_buffer_current->data_buffer_state = EVENT_DATA_BUFFER_FILLED;
			xEventGroupSetBits(spi_event_group, EVENT_DATA_BUFFER_FILLED);
		}
#endif /* MESH_SET_ROOT */
	}
}

/* Initialize the SPI communication interface between the STM32F407uC and the ESP32 radio module */
void spi_init(void) {
	/* Allocate the SPI Tx/Rx/Data Buffers */
	spi_tx_buffer = (uint8_t *) heap_caps_malloc(SPI_MAX_PACKET_SIZE, MALLOC_CAP_DMA);
	spi_rx_buffer = (uint8_t *) heap_caps_malloc(SPI_MAX_PACKET_SIZE, MALLOC_CAP_DMA);
	data_buffer_1 = (data_buffer_t *) heap_caps_calloc(1, sizeof(data_buffer_t), MALLOC_CAP_DMA);
	data_buffer_1->data = (uint8_t *) heap_caps_calloc(MAX_BUFFER_SIZE, 1, MALLOC_CAP_DMA);
	data_buffer_2 = (data_buffer_t *) heap_caps_calloc(1, sizeof(data_buffer_t), MALLOC_CAP_DMA);
	data_buffer_2->data = (uint8_t *) heap_caps_calloc(MAX_BUFFER_SIZE, 1, MALLOC_CAP_DMA);
	if (spi_tx_buffer == NULL || spi_rx_buffer == NULL || data_buffer_1->data == NULL || data_buffer_2->data == NULL) {
		ESP_LOGE(MESH_TAG, "Memory Allocation Failed for one of the SPI Tx or Rx or Data Buffers!");
		return;
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

	/* Initialize the RGB LEDs */
	rgb_init();

	/* Initialize SPI */
	spi_init();
	/* Create a pinned SPI communication task */
	xTaskCreatePinnedToCore(spi_task, "SPI Task", SPI_TASK_STACK_SIZE, NULL, SPI_TASK_PRIORITY, NULL, CORE_1);

	/* Create a pinned RGB handling task */
	xTaskCreatePinnedToCore(rgb_task, "RGB Task", RGB_TASK_STACK_SIZE, NULL, RGB_TASK_PRIORITY, NULL, CORE_0);

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
}
