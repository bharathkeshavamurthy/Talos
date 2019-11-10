/* This C source file has been derived from leds.c */

#include "talos_leds.h"

static uint8_t talos_rgb_led[TALOS_NUM_RGB_LED][TALOS_NUM_COLOR_LED];

void talos_set_led(talos_led_name_t led_number, unsigned int value) {
	switch(led_number) {
		case TALOS_LED1:
			if(value>1)  {
				palTogglePad(GPIOD, GPIOD_LED1);
			} else {
				value ? palClearPad(GPIOD, GPIOD_LED1) : palSetPad(GPIOD, GPIOD_LED1);
			}
			break;
		case TALOS_LED3:
			if(value>1) {
				palTogglePad(GPIOD, GPIOD_LED3);
			} else {
				value ? palClearPad(GPIOD, GPIOD_LED3) : palSetPad(GPIOD, GPIOD_LED3);
			}
			break;
		case TALOS_LED5:
			if(value>1) {
				palTogglePad(GPIOD, GPIOD_LED5);
			} else {
				value ? palClearPad(GPIOD, GPIOD_LED5) : palSetPad(GPIOD, GPIOD_LED5);
			}
			break;
		case TALOS_LED7:
			if(value>1) {
				palTogglePad(GPIOD, GPIOD_LED7);
			} else {
				value ? palClearPad(GPIOD, GPIOD_LED7) : palSetPad(GPIOD, GPIOD_LED7);
			}
			break;
		default:
			for(int i=0; i<4; i++) {
				talos_set_led(i, value);
			}
	}
}

void talos_set_rgb_led(talos_rgb_led_name_t led_number, uint8_t red_val, uint8_t green_val, uint8_t blue_val) {
	if(!talos_spi_rgb_setting_is_enabled()) {
		talos_spi_rgb_setting_enable();
	}
	talos_rgb_led[led_number][TALOS_RED_LED] = red_val;
	talos_rgb_led[led_number][TALOS_GREEN_LED] = green_val;
	talos_rgb_led[led_number][TALOS_BLUE_LED] = blue_val;
}

void talos_toggle_rgb_led(talos_rgb_led_name_t led_number, talos_color_led_name_t led, uint8_t intensity) {
	if(!talos_spi_rgb_setting_is_enabled()) {
		talos_spi_rgb_setting_enable();
	}
	if(talos_rgb_led[led_number][led] > 0) {
		talos_rgb_led[led_number][led] = 0;
	} else {
		talos_rgb_led[led_number][led] = intensity;
	}
}

void talos_set_body_led(unsigned int value) {
	if(value>1) {
		palTogglePad(GPIOB, GPIOB_LED_BODY);
	} else {
		value ? palSetPad(GPIOB, GPIOB_LED_BODY) : palClearPad(GPIOB, GPIOB_LED_BODY);
	}
}

void talos_set_front_led(unsigned int value) {
	if(value>1) {
		palTogglePad(GPIOD, GPIOD_LED_FRONT);
	} else {
		value ? palSetPad(GPIOD, GPIOD_LED_FRONT) : palClearPad(GPIOD, GPIOD_LED_FRONT);
	}
}

void talos_clear_leds(void)
{
	for(int i=0; i<4; i++) {
		talos_set_led(i, 0);
		talos_set_rgb_led(i, 0, 0, 0);
	}
}

void talos_get_all_rgb_state(uint8_t* values) {
	memcpy(values, talos_rgb_led, TALOS_NUM_RGB_LED * TALOS_NUM_COLOR_LED);
}
