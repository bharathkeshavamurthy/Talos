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

#include "mesh.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_mesh_internal.h"

/* Macros */
#ifndef MESH_SET_ROOT
#define MESH_SET_NODE
#endif

/* Constant Declarations */
static const int DEFAULT_RSSI = -120;
static const uint8_t MESH_ID[6] = {0x77, 0x77, 0x77, 0x77, 0x77, 0x77};

/* Variable Declarations */
static int mesh_layer = -1;
static uint8_t last_layer = 0;
static mesh_addr_t mesh_parent_addr;

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
	int parent_found = 0;

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
#ifdef MESH_SET_NODE
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
#endif
		} else {
#ifdef MESH_SET_ROOT
			/* If the parent is not the router, set me as the root and my layer as the root layer */
			if (!strcmp(CONFIG_MESH_ROUTER_SSID, (char *) record.ssid)) {
				parent_found = true;
				memcpy(&associated_parent_record, &record, sizeof(record));
				my_type = MESH_ROOT;
				my_layer = MESH_ROOT_LAYER;
			}
#endif
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
		mesh_event_connected_t *connected = (mesh_event_connected_t *)event_data;
		/* The parent's layer */
		mesh_layer = connected->self_layer;
		last_layer = mesh_layer;
		/* Extract the parent's MAC address */
		memcpy(&mesh_parent_addr.addr, connected->connected.bssid, 6);
		if (esp_mesh_is_root()) {
			/* Start the DHCP client */
			tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA);
		}
	}
	break;

	case MESH_EVENT_PARENT_DISCONNECTED: {
		/* Parse the received event_data which is a MESH_EVENT_PARENT_DISCONNECTED event */
		mesh_event_disconnected_t *disconnected = (mesh_event_disconnected_t *)event_data;
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
		mesh_event_layer_change_t *layer_change = (mesh_event_layer_change_t *)event_data;
		/* This node's new layer */
		mesh_layer = layer_change->new_layer;
		last_layer = mesh_layer;
	}
	break;

	case MESH_EVENT_SCAN_DONE: {
		/* Parse the received event_data which is a MESH_EVENT_SCAN_DONE event */
		mesh_event_scan_done_t *scan_done = (mesh_event_scan_done_t *)event_data;
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
}
