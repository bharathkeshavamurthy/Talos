/*
 * legos.c
 *
 *  Created on: Jan 30, 2020
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity describes the group formation, dispersion, and re-formation logic for the e-puck2 robots within the Talos initiative.
 *
 *  Copyright (c) 2020. All Rights Reserved.
 */

/* NEAREST_NEIGHBOR_RSSI_THRESHOLD */
#ifndef NEAREST_NEIGHBOR_RSSI_THRESHOLD
	#define NEAREST_NEIGHBOR_RSSI_THRESHOLD -40
#endif
/* NEAREST_NEIGHBOR_RSSI_THRESHOLD */

/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
	#define NUMBER_OF_CHILDREN 3
#endif
/* MESH_SET_ROOT */

#define CORE_1 1
#define PIN_NUM_CS 5
#define PIN_NUM_CLK 1
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define DEFAULT_RSSI -120
#define SPI_TASK_PRIORITY 5
#define P2P_BUFFER_SIZE 1024
#define MAX_BUFFER_SIZE 48000
#define SPI_TASK_STACK_SIZE 2048
#define SPI_MAX_PACKET_SIZE 1024

static uint8_t running = 0;
/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
	static uint8_t formation, dispersion = 0;
	static uint8_t tx_buffer[P2P_BUFFER_SIZE] = {0, };
#endif
/* MESH_SET_ROOT */
static uint8_t rx_buffer[P2P_BUFFER_SIZE] = {0, };

static int motion_dynamics_flag = 0;
static const uint8_t MESH_ID[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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

typedef enum {
	DATA_BUFFER_EMPTY = 0,
	DATA_BUFFER_FULL = 1
} data_buffer_state_t;

typedef struct {
	data_buffer_state_t data_buffer_state;
	uint8_t *data;
} data_buffer_t;

uint8_t *spi_tx_buffer;
uint8_t *spi_rx_buffer;
data_buffer_t *data_buffer_1;
data_buffer_t *data_buffer_2;
data_buffer_t *data_buffer_previous;
data_buffer_t *data_buffer_current;

/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
	int node_layer = 1;
	uint8_t number_of_children = 0;
	mesh_type_t node_type = MESH_NODE;
	uint8_t children[NUMBER_OF_CHILDREN];
#else
	int node_layer = 2;
	mesh_type_t node_type = MESH_LEAF;
#endif
/* MESH_SET_ROOT */

/* The Handler method for a specific IP_EVENT: IP_EVENT_STA_GOT_IP */
void received_ip_address_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
	ip_event_got_ip_t *received_ip_address = (ip_event_got_ip_t *) event_data;
	/* Log a DEBUG message indicating that an IP address has been assigned to this TCP/IP adapter (of this node) - use the ${received_ip_address} member for additional information */
	/* ip4addr_ntoa(&received_ip_address->ip_info.ip) */
}

/* The Handler method for handling parent scans in the mesh network */
void mesh_scan_handler(int count) {
	int information_element_length = 0;
	unsigned int parent_found = 0;

	mesh_assoc_t association;
	mesh_assoc_t associated_parent = {
			.layer = CONFIG_MESH_MAX_LAYERS,
			.rssi = DEFAULT_RSSI
	};
	wifi_ap_record_t wifi_ap_record;
	wifi_ap_record_t associated_parent_record = {0};
	wifi_config_t associated_parent_config = {0};
	wifi_scan_config_t wifi_scan_config = {0};

	for (int i=0; i< count; i++) {
		ESP_ERROR_CHECK(esp_mesh_scan_get_ap_ie_len(&information_element_length));
		ESP_ERROR_CHECK(esp_mesh_scan_get_ap_record(&wifi_ap_record, &association));
		if (information_element_length == sizeof(association)) {
			/* This is a simple single secondary layer mesh network coordinated by a designated gateway that's connected to the router in the WLAN */
			/* Hence, there is no need for complicated topology checks here... */
			if (association.mesh_type != MESH_IDLE && association.assoc < association.assoc_cap && wifi_ap_record.rssi > NEAREST_NEIGHBOR_RSSI_THRESHOLD) {
				parent_found = 1;
				memcpy(&associated_parent, &association, sizeof(association));
				memcpy(&associated_parent_record, &wifi_ap_record, sizeof(wifi_ap_record));
				break;
			}
		} else {
/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
			if (!strcmp(CONFIG_MESH_ROUTER_SSID, (char *) wifi_ap_record.ssid)) {
				parent_found = 1;
				memcpy(&associated_parent_record, &wifi_ap_record, sizeof(wifi_ap_record));
				node_type = MESH_ROOT;
			}
#endif
/* MESH_SET_ROOT */
		}
	}

	ESP_ERROR_CHECK(esp_mesh_flush_scan_result());

	if (parent_found == 1) {
		associated_parent_config.sta.channel = associated_parent_record.primary;
		associated_parent_config.sta.bssid_set = 1;
		memcpy(&associated_parent_config.sta.ssid, &associated_parent_record.ssid, sizeof(associated_parent_record.ssid));
		memcpy(&associated_parent_config.sta.bssid, &associated_parent_record.bssid, 6);
		ESP_ERROR_CHECK(esp_mesh_set_ap_authmode(associated_parent_record.authmode));
		if (node_type == MESH_ROOT && associated_parent_record.authmode != WIFI_AUTH_OPEN) {
			memcpy(&associated_parent_config.sta.password, CONFIG_MESH_ROUTER_PASSWORD, strlen(CONFIG_MESH_ROUTER_PASSWORD));
		} else if (associated_parent_record.authmode != WIFI_AUTH_OPEN) {
			memcpy(&associated_parent_config.sta.password, &CONFIG_MESH_AP_PASSWORD, strlen(CONFIG_MESH_AP_PASSWORD));
		} else{
			/* Security is Paramount: Log an ERROR message saying that Open WiFi Authentication Mode is not supported! */
			return;
		}
		ESP_ERROR_CHECK(esp_mesh_set_parent(&associated_parent_config, (mesh_addr_t *) &associated_parent.mesh_id, node_type, node_layer));
	} else {
		/* TODO: This might be a little too drastic...Re-evaluate this! */
		ESP_ERROR_CHECK(esp_mesh_set_ie_crypto_funcs(NULL));
		esp_wifi_scan_stop();
		wifi_scan_config.show_hidden = 1;
		wifi_scan_config.scan_type = WIFI_SCAN_TYPE_PASSIVE;
		/* TODO: Is a blocking-call necessary here...Again, is this too drastic a step? Re-evaluate this! */
		ESP_ERROR_CHECK(esp_wifi_scan_start(&wifi_scan_config, 0));
	}
}

/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
/* The Mesh P2P Tx Routine */
void mesh_p2p_tx(void *arg) {
	int flag = 0;
	int route_table_size = 0;
	mesh_data_t data;
	data.data = tx_buffer;
	data.size = P2P_BUFFER_SIZE;
	mesh_addr_t route_table[NUMBER_OF_CHILDREN];
	while (running) {
		esp_mesh_get_routing_table((mesh_addr_t *) &route_table, NUMBER_OF_CHILDREN * 6, &route_table_size);
		if (route_table_size < NUMBER_OF_CHILDREN) {
			vTaskDelay(1000 / portTICK_RATE_MS);
		}
		if (formation) {
			motion_dynamics_flag = 1;
		} else if(dispersion) {
			motion_dynamics_flag = 2;
		} else {
			motion_dynamics_flag = 3;
		}
		tx_buffer[0] = motion_dynamics_flag;
		for (int i=0; i<route_table_size; i++) {
			ESP_ERROR_CHECK(esp_mesh_send(&route_table[i], &data, MESH_DATA_P2P, NULL, 0));
		}
	}
	vTaskDelete(NULL);
}
#else
/* The Mesh Peer-to-Peer Rx Routine */
void mesh_p2p_rx(void *arg) {
	int flag = 0;
	mesh_addr_t sender_address;
	mesh_data_t received_data;
	received_data.data = rx_buffer;
	received_data.size = P2P_BUFFER_SIZE;
	while (running) {
		received_data.size = P2P_BUFFER_SIZE;
		ESP_ERROR_CHECK(esp_mesh_recv(&sender_address, &received_data, portMAX_DELAY, &flag, NULL, 0));
		if (!received_data.size) {
			motion_dynamics_flag = received_data[0];
		}
	}
	vTaskDelete(NULL);
}
#endif
/* MESH_SET_ROOT */

/* Peer-to-Peer Communication Start Routine */
esp_err_t mesh_p2p_communication_start(void) {
	if (!running) {
		running = 1;
/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
		xTaskCreate(mesh_p2p_tx, "Mesh_Tx", 3072, NULL, 5, NULL);
#else
		xTaskCreate(mesh_p2p_rx, "Mesh_Rx", 3072, NULL, 5, NULL);
#endif
/* MESH_SET_ROOT */
	}
	return ESP_OK;
}

/* Peer-to-Peer Communication Stop Routine */
esp_err_t mesh_p2p_communication_stop(void) {
	if (running) {
		running = 0;
	}
	return ESP_OK;
}

/* The Handler method for MESH_EVENTs */
/* NOTE: There are some filler cases added for enhanced debugging */
/* NOTE: Self-Organization has been disabled in the mesh start sequence; Nearest Neighbor Discovery using RSSI is enabled in the scan handler routine */
void mesh_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
	wifi_scan_config_t scan_config = {0};

	switch(event_id) {
	case MESH_EVENT_STARTED: {
		/* Log a DEBUG message here indicating that the mesh has been started */
		ESP_ERROR_CHECK(esp_mesh_set_self_organized(0, 0));
		esp_wifi_scan_stop();
		scan_config.show_hidden = 1;
		scan_config.scan_type = WIFI_SCAN_TYPE_PASSIVE;
		ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 0));
	}
	break;

	case MESH_EVENT_STOPPED: {
		/* Log an DEBUG message here indicating that the mesh has been terminated */
	}
	break;

	case MESH_EVENT_SCAN_DONE: {
		/* Log a DEBUG message here indicating that the scan operation has been completed */
		mesh_event_scan_done_t *scan_done = (mesh_event_scan_done_t *) event_data;
		mesh_scan_handler(scan_done->number);
	}
	break;

	case MESH_EVENT_PARENT_CONNECTED: {
		mesh_event_connected_t *parent_connected = (mesh_event_connected_t *) event_data;
		memcpy(&mesh_parent_addr.addr, parent_connected->connected.bssid, 6);
		if (esp_mesh_is_root()) {
			ESP_ERROR_CHECK(tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA));
		}
/* MESH_SET_ROOT */
#ifndef MESH_SET_ROOT
		ESP_ERROR_CHECK(mesh_p2p_communication_start());
#endif
/* MESH_SET_ROOT */
		/* Log a DEBUG message here indicating that this node has connected to a parent - use the ${parent_connected} member for additional information */
	}
	break;

	case MESH_EVENT_PARENT_DISCONNECTED: {
		mesh_event_disconnected_t *parent_disconnected = (mesh_event_disconnected_t *) event_data;
		/* Log the reason for disconnection here, i.e. parent_disconnected->reason */
/* MESH_SET_ROOT */
#ifndef MESH_SET_ROOT
		ESP_ERROR_CHECK(mesh_p2p_communication_stop());
#endif
/* MESH_SET_ROOT */
		esp_wifi_scan_stop();
		scan_config.show_hidden = 1;
		scan_config.scan_type = WIFI_SCAN_TYPE_PASSIVE;
		ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 0));
		/* Log a DEBUG message here indicating that this node has disconnected from its parent - use the ${parent_disconnected} member for additional information */
	}
	break;

	case MESH_EVENT_NO_PARENT_FOUND: {
		mesh_event_no_parent_found_t *no_parent_found = (mesh_event_no_parent_found_t *) event_data;
		/* Log a DEBUG message here indicating that no parent has been found for this node - use the ${no_parent_found} member for additional information */
	}
	break;

	case MESH_EVENT_CHILD_CONNECTED: {
		mesh_event_child_connected_t *child_connected = (mesh_event_child_connected_t *) event_data;
/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
		children[number_of_children] = child_connected->mac;
		number_of_children++;
		if (number_of_children == NUMBER_OF_CHILDREN) {
			formation = 1;
			ESP_ERROR_CHECK(mesh_p2p_communication_start());
		}
#endif
/* MESH_SET_ROOT */
		/* Log a DEBUG message here indicating that a child has connected to this node - use the ${child_connected} member for additional information */
	}
	break;

	case MESH_EVENT_CHILD_DISCONNECTED: {
		mesh_event_child_disconnected_t *child_disconnected = (mesh_event_child_disconnected_t *) event_data;
/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
		disconnected_child = 0;
		ESP_ERROR_CHECK(mesh_p2p_communication_stop());
		for (int child=0; child<number_of_children; child++) {
			if (children[child] == child_disconnected->mac) {
				disconnected_child = child;
			}
		}
		for (int child=disconnected_child; child<number_of_children-1; child++) {
			children[child] = children[child+1];
		}
#endif
/* MESH_SET_ROOT */
		/* Log a DEBUG message here indicating that a child has disconnected from this node - use the ${child_disconnected} member for additional information */
	}
	break;

	case MESH_EVENT_ROUTING_TABLE_ADD: {
		mesh_event_routing_table_change_t *routing_table_change = (mesh_event_routing_table_change_t *) event_data;
		/* Log a DEBUG message here indicating an addition to the routing table - use the ${routing_table_change} member for additional information */
	}
	break;

	case MESH_EVENT_ROUTING_TABLE_REMOVE: {
		mesh_event_routing_table_change_t *routing_table_change = (mesh_event_routing_table_change_t *) event_data;
		/* Log a DEBUG message here indicating a removal from the routing table - use the ${routing_table_change} member for additional information */
	}
	break;

	case MESH_EVENT_NETWORK_STATE: {
		mesh_event_network_state_t *network_state = (mesh_event_network_state_t *) event_data;
		/* Log a DEBUG message here printing the state of the mesh network - use the ${network_state} member to determine if the network is root-less */
	}
	break;

	case MESH_EVENT_ROOT_FIXED: {
		mesh_event_root_fixed_t *root_fixed = (mesh_event_root_fixed_t *) event_data;
		/* Log a DEBUG message here indicating that the root for this mesh network has been fixed - use the ${root_fixed} member for additional information */
	}
	break;

	case MESH_EVENT_ROOT_ADDRESS: {
		mesh_event_root_address_t *root_address = (mesh_event_root_address_t *) event_data;
		/* Log a DEBUG message here printing the MAC address of the root node of this mesh network - use the ${root_address} member for extracting the relevant MAC address */
	}
	break;

	case MESH_EVENT_CHANNEL_SWITCH: {
		mesh_event_channel_switch_t *channel_switch = (mesh_event_channel_switch_t *) event_data;
		/* Log a DEBUG message here indicating a channel switch - use the ${channel_switch} member for additional information */
	}
	break;

	case MESH_EVENT_ROUTER_SWITCH: {
		mesh_event_router_switch_t *router_switch = (mesh_event_router_switch_t *) event_data;
		/* Log a DEBUG message here indicating a router switch - use the ${router_switch} member for additional information */
	}
	break;

	case MESH_EVENT_FIND_NETWORK: {
		mesh_event_find_network_t *find_network = (mesh_event_find_network_t *) event_data;
		/* Log a DEBUG message indicating a channel switch and a router search - use the ${find_network} member and associated ${channel} and ${router_bssid} sub-members for additional information */
	}
	break;

	case MESH_EVENT_STOP_RECONNECTION: {
		/* Log a DEBUG message saying a <STOP_RECONNECTION> event has been received for this node in this mesh */
	}
	break;

	default: {
		/* Log an ERROR message indicating the reception of an incorrect MESH_EVENT identifier */
		break;
	}

	}
}

/* The SPI Task Routine */
void spi_task(void *arg) {
	memset(spi_tx_buffer, 0x00, SPI_MAX_PACKET_SIZE);
	memset(spi_rx_buffer, 0x00, SPI_MAX_PACKET_SIZE);
	memset(&spi_slave_transaction, 0, sizeof(spi_slave_transaction));
	spi_slave_transaction.tx_buffer = spi_tx_buffer;
	spi_slave_transaction.rx_buffer = spi_rx_buffer;
	spi_slave_transaction.length = SPI_MAX_PACKET_SIZE * 8;
	spi_slave_transaction.user = (void *) 0;
	while (running) {
		spi_tx_buffer[0] = motion_dynamics_flag;
		spi_slave_transaction.trans_len = 1 * 8;
		spi_slave_transaction.tx_buffer = spi_tx_buffer;
		ESP_ERROR_CHECK(spi_slave_transmit(VSPI_HOST, &spi_slave_transaction, portMAX_DELAY));
	}
	memset(spi_tx_buffer, 0x00, SPI_MAX_PACKET_SIZE);
	memset(spi_rx_buffer, 0x00, SPI_MAX_PACKET_SIZE);
	spi_tx_buffer[0] = 0; /* A Reset */
	spi_slave_transaction.trans_len = 1 * 8;
	spi_slave_transaction.tx_buffer = spi_tx_buffer;
	spi_slave_transaction.rx_buffer = spi_rx_buffer;
	ESP_ERROR_CHECK(spi_slave_transmit(VSPI_HOST, &spi_slave_transaction, portMAX_DELAY));
}

/* The SPI Initialization routine */
void spi_init(void) {
	/* SPI Event Group */
	spi_event_group = xEventGroupCreate();

	/* Memory Allocation for the SPI Tx and Rx Buffers */
	spi_tx_buffer = (uint8_t *) heap_caps_malloc(SPI_MAX_PACKET_SIZE, MALLOC_CAP_DMA);
	spi_rx_buffer = (uint8_t *) heap_caps_malloc(SPI_MAX_PACKET_SIZE, MALLOC_CAP_DMA);

	/* Memory Allocation for the Data Buffers */
	data_buffer_1 = (data_buffer_t *) heap_caps_calloc(1, sizeof(data_buffer_t), MALLOC_CAP_DMA);
	data_buffer_2 = (data_buffer_t *) heap_caps_calloc(1, sizeof(data_buffer_t), MALLOC_CAP_DMA);
	data_buffer_1->data = (uint8_t *) heap_caps_calloc(MAX_BUFFER_SIZE, 1, MALLOC_CAP_DMA);
	data_buffer_2->data = (uint8_t *) heap_caps_calloc(MAX_BUFFER_SIZE, 1, MALLOC_CAP_DMA);
	if (spi_tx_buffer == NULL || spi_rx_buffer == NULL || data_buffer_1->data == NULL || data_buffer_2->data == NULL) {
		/* Log an error here */
		/* Terminal output is not available in this version of epuck-2: It experiences variations in XTAL frequency causing junk values to be logged to the serial port */
	}

	/* Initial Buffer Assignments */
	data_buffer_previous = data_buffer_1;
	data_buffer_current = data_buffer_2;
	data_buffer_current->data_buffer_state = DATA_BUFFER_EMPTY;

	/* Pull down all relevant GPIO pins to prevent rogue pulses from driving the M/S */
	gpio_set_pull_mode(PIN_NUM_MISO, GPIO_PULLDOWN_ONLY);
	gpio_set_pull_mode(PIN_NUM_MOSI, GPIO_PULLDOWN_ONLY);
	gpio_set_pull_mode(PIN_NUM_CLK, GPIO_PULLDOWN_ONLY);
	gpio_set_pull_mode(PIN_NUM_CS, GPIO_PULLDOWN_ONLY);

	/* SPI Slave Initialization */
	ESP_ERROR_CHECK(spi_slave_initialize(VSPI_HOST, &spi_bus_config, &spi_slave_interface_config, 1));
}

/* Run Trigger */
void app_main(void) {
	/* Initialize the Non-Volatile Storage memory within ESP32 */
	ESP_ERROR_CHECK(nvs_flash_init());

	/* Create a default event loop */
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	/* Initialize the Serial Peripheral Interface (SPI) and create & pin the SPI task to Core-1 */
	spi_init();
	xTaskCreatePinnedToCore(spi_task, "SPI_TASK", SPI_TASK_STACK_SIZE, NULL, SPI_TASK_PRIORITY, NULL, CORE_1);

	/* TCP/IP Initializations and Configurations */
	tcpip_adapter_init();
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
	ESP_ERROR_CHECK(tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA));
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &received_ip_address_event_handler, NULL));

	/* WiFi Initializations and Configurations */
	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
	ESP_ERROR_CHECK(esp_wifi_start());

	/* Mesh Initializations and Configurations */
	ESP_ERROR_CHECK(esp_mesh_init());
	ESP_ERROR_CHECK(esp_event_handler_register(MESH_EVENT, ESP_EVENT_ANY_ID, &mesh_event_handler, NULL));
	mesh_cfg_t mesh_init_config = MESH_INIT_CONFIG_DEFAULT();
	mem_cpy((uint8_t *) &mesh_init_config.mesh_id, MESH_ID, 6);
	mesh_init_config.channel = CONFIG_MESH_CHANNEL;
	mem_cpy((uint8_t *) &mesh_init_config.router.ssid, CONFIG_MESH_ROUTER_SSID, strlen(CONFIG_MESH_ROUTER_SSID));
	mem_cpy((uint8_t *) &mesh_init_config.router.password, CONFIG_MESH_ROUTER_PASSWORD, strlen(CONFIG_MESH_ROUTER_PASSWORD));
	mesh_init_config.mesh_ap.max_connection = CONFIG_MESH_AP_MAX_CONNECTIONS;
	memcpy((uint8_t *) &mesh_init_config.mesh_ap.password, CONFIG_MESH_AP_PASSWORD, strlen(CONFIG_MESH_AP_PASSWORD));
	ESP_ERROR_CHECK(esp_mesh_set_ap_authmode(CONFIG_MESH_AP_AUTHMODE));
	ESP_ERROR_CHECK(esp_mesh_set_config(&mesh_init_config));
	ESP_ERROR_CHECK(esp_mesh_start());
}
