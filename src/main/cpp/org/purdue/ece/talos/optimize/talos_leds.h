/* This header file has been derived from leds.h */

#ifndef TALOS_LEDS_H_
#define TALOS_LEDS_H_

#ifdef __cplusplus /* __cplusplus */
extern "C" {
#endif /* __cplusplus */

#include <hal.h>
#include <string.h>

#include "spi_communication.h"

#define RGB_MAX_INTENSITY 100

typedef enum {
	TALOS_LED1,
	TALOS_LED3,
	TALOS_LED5,
	TALOS_LED7,
	TALOS_NUM_LED,
} talos_led_name_t;

typedef enum {
	TALOS_LED2,
	TALOS_LED4,
	TALOS_LED6,
	TALOS_LED8,
	TALOS_NUM_RGB_LED,
} talos_rgb_led_name_t;

typedef enum {
	TALOS_RED_LED,
	TALOS_GREEN_LED,
	TALOS_BLUE_LED,
	TALOS_NUM_COLOR_LED,
} talos_color_led_name_t;

void talos_set_led(talos_led_name_t led_number, unsigned int value);

void talos_clear_leds(void);

void talos_set_body_led(unsigned int value);

void talos_set_front_led(unsigned int value);

void talos_get_all_rgb_state(uint8_t* values);

void talos_toggle_rgb_led(talos_rgb_led_name_t led_number, talos_color_led_name_t led, uint8_t intensity);

void talos_set_rgb_led(talos_rgb_led_name_t led_number, uint8_t red_val, uint8_t green_val, uint8_t blue_val);

#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */

#endif /* TALOS_LEDS_H_ */
