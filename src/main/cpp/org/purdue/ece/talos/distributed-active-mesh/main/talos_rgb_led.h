/* This header file has been derived from the RGB LED control logic of the e-puck2 */

#ifndef TALOS_RGB_LED_H
#define TALOS_RGB_LED_H

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_err.h"
#include "esp_attr.h"
#include "esp_mesh.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_system.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/spi_slave.h"
#include "freertos/FreeRTOS.h"
#include "esp_mesh_internal.h"
#include "freertos/xtensa_api.h"
#include "freertos/event_groups.h"

#define RGB_MIN_DUTY 0
#define RGB_MAX_DUTY 8191
#define RGB_PWM_FREQ 5000
#define RGB_MAX_INTENSITY 100
#define RGB_MAX_COLOR_VALUE 255

#define RGB_LED2_RED_GPIO 32
#define RGB_LED2_BLUE_GPIO 25
#define RGB_LED2_GREEN_GPIO 33

#define RGB_LED4_RED_GPIO 14
#define RGB_LED4_BLUE_GPIO 26
#define RGB_LED4_GREEN_GPIO 27

#define RGB_LED6_RED_GPIO 22
#define RGB_LED6_BLUE_GPIO 13
#define RGB_LED6_GREEN_GPIO 21

#define RGB_LED8_RED_GPIO 4
#define RGB_LED8_BLUE_GPIO 15
#define RGB_LED8_GREEN_GPIO 16

#define RGB_TASK_PRIORITY 4
#define RGB_TASK_STACK_SIZE 2048

typedef enum {
	LED2,
	LED4,
	LED6,
	LED8,
	NUM_RGB_LED,
} rgb_led_name_t;

typedef enum {
	RED_LED,
	GREEN_LED,
	BLUE_LED,
	NUM_LED,
} led_name_t;

typedef enum {
	RED,
	GREEN,
	BLUE,
	YELLOW,
	LIGHT_BLUE,
	MAGENTA,
	WHITE,
	NUM_COLORS,
}color_t;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} rgb_color_t;

extern rgb_color_t color[NUM_COLORS];

void rgb_init(void);

void rgb_set_intensity(rgb_led_name_t rgb_led, led_name_t led, uint8_t intensity, uint16_t time_ms);

void rgb_set_color(rgb_led_name_t led, uint8_t intensity, rgb_color_t* color_value, uint16_t time_ms);

void rgb_update_all(uint8_t *value);

void rgb_task(void *pvParameter);

void rgb_update_led2(uint8_t r, uint8_t g, uint8_t b);

void rgb_led2_gpio_set(uint8_t r, uint8_t g, uint8_t b);

void rgb_led4_gpio_set(uint8_t r, uint8_t g, uint8_t b);

void rgb_led6_gpio_set(uint8_t r, uint8_t g, uint8_t b);

void rgb_led8_gpio_set(uint8_t r, uint8_t g, uint8_t b);

void rgb_stop_pwm(void);

#endif /* TALOS_RGB_LED_H */
