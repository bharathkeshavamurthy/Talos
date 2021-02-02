/*

File    : main_e-puck2.c
Author  : Eliot Ferragni, Stefano Morgani
Date    : 12 november 2017
REV 1.0

Firmware to be run on the ESP32 of the e-puck2
*/

#define __BTSTACK_FILE__ "ESP32_E-Puck_2.c"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/xtensa_api.h"
#include "freertos/queue.h"
#include "esp_attr.h"   
#include "esp_err.h"
#include "main_e-puck2.h"
#include "rgb_led_e-puck2.h"
#include "uart_e-puck2.h"
#include "bluart_e-puck2.h"
#include "rfcomm_e-puck2.h"
#include "button_e-puck2.h"
#include "spi_e-puck2.h"
#include "http_server.h"
#include "wifi_manager.h"
#include "utility.h"
#include "nvs_flash.h"
#include "socket_e-puck2.h"

extern int btstack_main(void);

void app_main(void)
{ 
  rgb_init();
  button_init();
  //bluart_init();
  uart_init();
  spi_init();
  robot_read_id();
  ESP_ERROR_CHECK( nvs_flash_init() );
  socket_init();
 
  // Start the HTTP Server task.
  xTaskCreatePinnedToCore(&http_server, "http_server", 2048, NULL, 4, NULL, CORE_1);
  
  // WiFi configuration task.
  xTaskCreatePinnedToCore(&wifi_manager, "wifi_manager", 4096, NULL, 4, NULL, CORE_1);

  // WiFi stream task.
  xTaskCreatePinnedToCore(&socket_task, "socket_task", 4096, NULL, 5, NULL, CORE_1);

  // SPI communication task.
  xTaskCreatePinnedToCore(spi_task, "spi_task", SPI_TASK_STACK_SIZE, NULL, SPI_TASK_PRIO, NULL, CORE_1);
 
  // Button handling task.
  xTaskCreatePinnedToCore(button_task, "button_task", 2048, NULL, 4, NULL, CORE_1);  
  
  // RGB handling task.
  xTaskCreatePinnedToCore(rgb_task, "rgb_task", 2048, NULL, 4, NULL, CORE_0);  
}
