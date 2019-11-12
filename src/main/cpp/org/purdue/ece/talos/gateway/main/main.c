/*
 * main.c
 *
 *  Created on: Nov 6, 2019
 *  Author: Bharath Keshavamurthy <bkeshava@purdue.edu>
 *  Organization: School of Electrical and Computer Engineering, Purdue University, West Lafayette, IN.
 *
 *  This entity details the creation of a WiFi gateway (STA + AP) in the Talos initiative leveraging the ESP-IDF from Espressif.
 *
 *  Copyright (c) 2019. All Rights Reserved.
 */

/* This entity has been derived from the WiFi capabilities of the ESP32 and several ESP-IDF utilities */

#include <string.h>

#include "lwip/err.h"
#include "lwip/sys.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_system.h"

/* The correctness indicator */
#define BLINK_GPIO 4

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The node tag for logging */
static const char *TAG = "Talos_Wifi_Gateway";

/* Retry Connection Counter */
static int s_retry_num = 0;

/* The event group allows multiple bits for each event, but we only care about one event- Are we connected to the AP with an IP? */
const int WIFI_CONNECTED_BIT = BIT0;

/* The Access Point Event Handler */
static void access_point_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	/* Connection Event */
	if (event_id == WIFI_EVENT_AP_STACONNECTED) {
		wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
		/* Do some stuff here */
	/* Disconnection Event */
	} else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
		wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
		/* Do some stuff here */
	}
}

/* The Station Event Handler */
static void station_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	/* Station Start Event */
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    /* Station Disconnected from AP Event */
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < CONFIG_STA_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
            s_retry_num++;
            ESP_LOGI(TAG, "Retrying to connect to the AP...");
        }
        ESP_LOGI(TAG,"Maximum connection attempts exceeded. Connection to the AP failed!");
    /* Station Connected to the AP Event */
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Received IP Address: %s", ip4addr_ntoa(&event->ip_info.ip)); // @suppress("Field cannot be resolved")
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

/* Initialize both the Access Point and the Station in this routine */
void gateway_initialization(void) {
	s_wifi_event_group = xEventGroupCreate();
	/* Initialize the TCP/IP stack */
	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	/* Default WiFi Configuration and Associated Initialization*/
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	/* Register the Access Point Event Handler for WIFI_EVENTs */
	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &access_point_event_handler, NULL));
	/* Provided AP Configurations */
	wifi_config_t ap_wifi_config = {
		.ap = {
			.ssid = CONFIG_AP_ESP_WIFI_SSID,
			.ssid_len = strlen(CONFIG_AP_ESP_WIFI_SSID),
			.password = CONFIG_AP_ESP_WIFI_PASSWORD,
			.max_connection = CONFIG_AP_MAX_CONN,
			.authmode = WIFI_AUTH_WPA_WPA2_PSK
		},
	};
	/* Authentication Model is WIFI_AUTH_OPEN when no password is provided */
	if (strlen(CONFIG_AP_ESP_WIFI_PASSWORD) == 0) {
		ap_wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}
	/* Register the 	Station Event Handler for WIFI_EVENTs and IP_EVENTs */
	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &station_event_handler, NULL));
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &station_event_handler, NULL));
	/* Provided STA Configurations */
	wifi_config_t sta_wifi_config = {
		.sta = {
			.ssid = CONFIG_STA_ESP_WIFI_SSID,
			.password = CONFIG_STA_ESP_WIFI_PASSWORD
		},
	};
	/* Set the provided configurations */
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_wifi_config));
	 ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_wifi_config));
	/* Start the Gateway with the provided configurations */
	ESP_ERROR_CHECK(esp_wifi_start());
	/* Correctness Indication Routine for Sanity */
	gpio_pad_select_gpio(BLINK_GPIO);
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
	for(int i=0; i<60; i++) {
		gpio_set_level(BLINK_GPIO, 0);
		vTaskDelay(1000 / (3 * portTICK_PERIOD_MS));
		gpio_set_level(BLINK_GPIO, 1);
		vTaskDelay(1000 / (3 * portTICK_PERIOD_MS));
	}
	/* Final Logging */
	ESP_LOGI(TAG, "Finished creating an Access Point with SSID: %s and Connected to %s", CONFIG_AP_ESP_WIFI_SSID, CONFIG_STA_ESP_WIFI_SSID);
}

/* Run Trigger */
void app_main(void)
{
    /* Initialize NVS */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Gateway Initialization */
    gateway_initialization();
}
