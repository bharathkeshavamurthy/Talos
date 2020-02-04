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

#ifndef MESH_SET_ROOT
	#define MESH_SET_ROOT
#endif

#define CORE_1 1
#define PIN_NUM_CS 5
#define PIN_NUM_CLK 18
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define SPI_TASK_PRIORITY 5
#define NUMBER_OF_CHILDREN 3
#define MAX_BUFFER_SIZE 48000
#define SPI_TASK_STACK_SIZE 2048
#define SPI_MAX_PACKET_SIZE 1024

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

/* The Gateway */
/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
uint8_t number_of_children = 0;
uint8_t children[NUMBER_OF_CHILDREN];
#endif
/* MESH_SET_ROOT */

/* The Handler method for MESH_EVENTs */
void mesh_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
	wifi_scan_config_t scan_config = {0};

	switch(event_id) {
	case MESH_EVENT_STARTED: {
		ESP_ERROR_CHECK(esp_mesh_set_self_organized(0, 0));
		esp_wifi_scan_stop();
		scan_config.show_hidden = 1;
		scan_config.scan_type = WIFI_SCAN_TYPE_PASSIVE;
		ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 0));
	}
	break;

	case MESH_EVENT_SCAN_DONE: {
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
/* Not the Gateway */
/* MESH_SET_ROOT */
#ifndef MESH_SET_ROOT
		ESP_ERROR_CHECK(mesh_p2p_communication_start());
#endif
/* MESH_SET_ROOT */
	}
	break;

	case MESH_EVENT_PARENT_DISCONNECTED: {
		mesh_event_disconnected_t *parent_disconnected = (mesh_event_disconnected_t *) event_data;
		/* Log the reason for disconnection here, i.e. parent_disconnected->reason */
		esp_wifi_scan_stop();
		scan_config.show_hidden = 1;
		scan_config.scan_type = WIFI_SCAN_TYPE_PASSIVE;
		ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 0));
	}
	break;

	case MESH_EVENT_CHILD_CONNECTED: {
		mesh_event_child_connected_t *child_connected = (mesh_event_child_connected_t *) event_data;
/* The Gateway */
/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
		children[number_of_children] = child_connected->mac;
		number_of_children++;
#endif
/* MESH_SET_ROOT */
	}
	break;

	case MESH_EVENT_CHILD_DISCONNECTED: {
		mesh_event_child_disconnected_t *child_disconnected = (mesh_event_child_disconnected_t *) event_data;
/* The Gateway */
/* MESH_SET_ROOT */
#ifdef MESH_SET_ROOT
		disconnected_child = 0;
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
	}
	break;
	}
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
