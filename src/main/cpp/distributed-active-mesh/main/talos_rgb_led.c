/* This C source file has been derived from the RGB LED control logic of the e-puck2 */

#include "talos_rgb_led.h"

#define INTR_NO_FLAG  0

const int UPDATE_ALL_RGB_LEDS = BIT0;
const int UPDATE_LED2 = BIT1;

uint8_t rgb_value[NUM_RGB_LED][NUM_LED]= {0};

static EventGroupHandle_t rgb_event_group;

ledc_timer_config_t led_timer = {
    .duty_resolution = LEDC_TIMER_13_BIT,
    .freq_hz = RGB_PWM_FREQ,
    .timer_num = LEDC_TIMER_0
};

ledc_channel_config_t led_config[NUM_RGB_LED][NUM_LED] = {
  {
    {
      .gpio_num     = RGB_LED2_RED_GPIO,
      .speed_mode   = LEDC_HIGH_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_0,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
    {
      .gpio_num     = RGB_LED2_GREEN_GPIO,
      .speed_mode   = LEDC_HIGH_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_1,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
    { .gpio_num     = RGB_LED2_BLUE_GPIO,
      .speed_mode   = LEDC_HIGH_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_2,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
  },
  {
    {
      .gpio_num     = RGB_LED4_RED_GPIO,
      .speed_mode   = LEDC_HIGH_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_3,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
    {
      .gpio_num     = RGB_LED4_GREEN_GPIO,
      .speed_mode   = LEDC_HIGH_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_4,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
    {
      .gpio_num     = RGB_LED4_BLUE_GPIO,
      .speed_mode   = LEDC_HIGH_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_5,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
  },
  {
    {
      .gpio_num     = RGB_LED6_RED_GPIO,
      .speed_mode   = LEDC_HIGH_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_6,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
    {
      .gpio_num     = RGB_LED6_GREEN_GPIO,
      .speed_mode   = LEDC_HIGH_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_7,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
    {
      .gpio_num     = RGB_LED6_BLUE_GPIO,
      .speed_mode   = LEDC_LOW_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_0,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
  },
  {
    {
      .gpio_num     = RGB_LED8_RED_GPIO,
      .speed_mode   = LEDC_LOW_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_1,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
    {
      .gpio_num     = RGB_LED8_GREEN_GPIO,
      .speed_mode   = LEDC_LOW_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_2,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
    {
      .gpio_num     = RGB_LED8_BLUE_GPIO,
      .speed_mode   = LEDC_LOW_SPEED_MODE,
      .duty         = RGB_MAX_DUTY,
      .channel      = LEDC_CHANNEL_3,
      .intr_type    = LEDC_INTR_FADE_END,
      .timer_sel    = LEDC_TIMER_0,
    },
  },
};

rgb_color_t color[NUM_COLORS] ={
  {
    .red    = 255,
    .green  = 0,
    .blue   = 0,
  },
  {
    .red    = 0,
    .green  = 255,
    .blue   = 0,
  },
  {
    .red    = 0,
    .green  = 0,
    .blue   = 255,
  },
  {
    .red    = 255,
    .green  = 255,
    .blue   = 0,
  },
  {
    .red    = 0,
    .green  = 255,
    .blue   = 255,
  },
  {
    .red    = 255,
    .green  = 0,
    .blue   = 255,
  },
  {
    .red    = 255,
    .green  = 255,
    .blue   = 255,
  },
};

void rgb_init(void){
  led_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_timer_config(&led_timer);
  led_timer.speed_mode = LEDC_LOW_SPEED_MODE;
  ledc_timer_config(&led_timer);
  uint8_t rgb_led = 0;
  uint8_t led = 0;
  for(rgb_led = LED2 ; rgb_led < NUM_RGB_LED ; rgb_led++){
    for(led = 0 ; led < NUM_LED ; led++){
      ledc_channel_config(&led_config[rgb_led][led]);
    }
  }
  ledc_fade_func_install(INTR_NO_FLAG);
  rgb_event_group = xEventGroupCreate();
}

void rgb_set_intensity(rgb_led_name_t rgb_led, led_name_t led, uint8_t intensity, uint16_t time_ms){
  if(intensity > RGB_MAX_INTENSITY){
	return;
  }
  uint32_t value = (RGB_MAX_INTENSITY - intensity) * RGB_MAX_DUTY / RGB_MAX_INTENSITY;
  if(ledc_set_fade_with_time(led_config[rgb_led][led].speed_mode, led_config[rgb_led][led].channel, value, time_ms) != ESP_OK) {
	  return;
  }
  ledc_fade_start(led_config[rgb_led][led].speed_mode, led_config[rgb_led][led].channel, LEDC_FADE_NO_WAIT);
}

void rgb_set_color(rgb_led_name_t rgb_led, uint8_t intensity, rgb_color_t* color_value , uint16_t time_ms){
  if(intensity > RGB_MAX_INTENSITY){
    intensity = RGB_MAX_INTENSITY;
  }
  uint32_t value_color[NUM_LED];
  value_color[RED_LED] = RGB_MAX_DUTY - (((((( (color_value->red) * RGB_MAX_INTENSITY ) / RGB_MAX_COLOR_VALUE ) * intensity ) / RGB_MAX_INTENSITY ) * RGB_MAX_DUTY) / RGB_MAX_INTENSITY );
  value_color[GREEN_LED] = RGB_MAX_DUTY - (((((( (color_value->green) * RGB_MAX_INTENSITY ) / RGB_MAX_COLOR_VALUE ) * intensity ) / RGB_MAX_INTENSITY ) * RGB_MAX_DUTY) / RGB_MAX_INTENSITY );
  value_color[BLUE_LED] = RGB_MAX_DUTY - (((((( (color_value->blue) * RGB_MAX_INTENSITY ) / RGB_MAX_COLOR_VALUE ) * intensity ) / RGB_MAX_INTENSITY ) * RGB_MAX_DUTY) / RGB_MAX_INTENSITY );
  uint8_t led = 0;
  for(led = 0 ; led < NUM_LED ; led++ ){
    ledc_set_fade_with_time(led_config[rgb_led][led].speed_mode, led_config[rgb_led][led].channel, value_color[led], time_ms);
    ledc_fade_start(led_config[rgb_led][led].speed_mode, led_config[rgb_led][led].channel, LEDC_FADE_NO_WAIT);
  }
}

void rgb_update_all(uint8_t *value) {
	memcpy(rgb_value, value, 12);
	xEventGroupSetBits(rgb_event_group, UPDATE_ALL_RGB_LEDS);
}

void rgb_update_led2(uint8_t r, uint8_t g, uint8_t b) {
	rgb_value[0][0] = r;
	rgb_value[0][1] = g;
	rgb_value[0][2] = b;
	xEventGroupSetBits(rgb_event_group, UPDATE_LED2);
}

void rgb_task(void *pvParameter) {
	EventBits_t event_bits;
	while(1) {
		event_bits = xEventGroupWaitBits(rgb_event_group, UPDATE_ALL_RGB_LEDS | UPDATE_LED2, true, false, portMAX_DELAY);
		if(event_bits & UPDATE_ALL_RGB_LEDS) {
			rgb_set_intensity(LED2, RED_LED, rgb_value[0][0], 0);
			rgb_set_intensity(LED2, GREEN_LED, rgb_value[0][1], 0);
			rgb_set_intensity(LED2, BLUE_LED, rgb_value[0][2], 0);
			rgb_set_intensity(LED4, RED_LED, rgb_value[1][0], 0);
			rgb_set_intensity(LED4, GREEN_LED, rgb_value[1][1], 0);
			rgb_set_intensity(LED4, BLUE_LED, rgb_value[1][2], 0);
			rgb_set_intensity(LED6, RED_LED, rgb_value[2][0], 0);
			rgb_set_intensity(LED6, GREEN_LED, rgb_value[2][1], 0);
			rgb_set_intensity(LED6, BLUE_LED, rgb_value[2][2], 0);
			rgb_set_intensity(LED8, RED_LED, rgb_value[3][0], 0);
			rgb_set_intensity(LED8, GREEN_LED, rgb_value[3][1], 0);
			rgb_set_intensity(LED8, BLUE_LED, rgb_value[3][2], 0);
		} else if(event_bits & UPDATE_LED2) {
			rgb_set_intensity(LED2, RED_LED, rgb_value[0][0], 0);
			rgb_set_intensity(LED2, GREEN_LED, rgb_value[0][1], 0);
			rgb_set_intensity(LED2, BLUE_LED, rgb_value[0][2], 0);
		}
	}
}

void rgb_led2_gpio_set(uint8_t r, uint8_t g, uint8_t b) {
	gpio_set_level(RGB_LED2_RED_GPIO, r);
	gpio_set_level(RGB_LED2_GREEN_GPIO, g);
	gpio_set_level(RGB_LED2_BLUE_GPIO, b);
}

void rgb_led4_gpio_set(uint8_t r, uint8_t g, uint8_t b) {
	gpio_set_level(RGB_LED4_RED_GPIO, r);
	gpio_set_level(RGB_LED4_GREEN_GPIO, g);
	gpio_set_level(RGB_LED4_BLUE_GPIO, b);
}

void rgb_led6_gpio_set(uint8_t r, uint8_t g, uint8_t b) {
	gpio_set_level(RGB_LED6_RED_GPIO, r);
	gpio_set_level(RGB_LED6_GREEN_GPIO, g);
	gpio_set_level(RGB_LED6_BLUE_GPIO, b);
}

void rgb_led8_gpio_set(uint8_t r, uint8_t g, uint8_t b) {
	gpio_set_level(RGB_LED8_RED_GPIO, r);
	gpio_set_level(RGB_LED8_GREEN_GPIO, g);
	gpio_set_level(RGB_LED8_BLUE_GPIO, b);
}

void rgb_stop_pwm(void) {
	uint8_t rgb_led = 0;
	uint8_t led = 0;
	for(rgb_led = LED2 ; rgb_led < NUM_RGB_LED ; rgb_led++){
		for(led = 0 ; led < NUM_LED ; led++){
			ledc_stop(led_config[rgb_led][led].speed_mode, led_config[rgb_led][led].channel, 1);
		}
	}
	gpio_pad_select_gpio(RGB_LED2_RED_GPIO);
	gpio_set_direction(RGB_LED2_RED_GPIO, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(RGB_LED2_GREEN_GPIO);
	gpio_set_direction(RGB_LED2_GREEN_GPIO, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(RGB_LED2_BLUE_GPIO);
	gpio_set_direction(RGB_LED2_BLUE_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_level(RGB_LED2_RED_GPIO, 1);
	gpio_set_level(RGB_LED2_GREEN_GPIO, 1);
	gpio_set_level(RGB_LED2_BLUE_GPIO, 1);
	gpio_pad_select_gpio(RGB_LED4_RED_GPIO);
	gpio_set_direction(RGB_LED4_RED_GPIO, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(RGB_LED4_GREEN_GPIO);
	gpio_set_direction(RGB_LED4_GREEN_GPIO, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(RGB_LED4_BLUE_GPIO);
	gpio_set_direction(RGB_LED4_BLUE_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_level(RGB_LED4_RED_GPIO, 1);
	gpio_set_level(RGB_LED4_GREEN_GPIO, 1);
	gpio_set_level(RGB_LED4_BLUE_GPIO, 1);
	gpio_pad_select_gpio(RGB_LED6_RED_GPIO);
	gpio_set_direction(RGB_LED6_RED_GPIO, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(RGB_LED6_GREEN_GPIO);
	gpio_set_direction(RGB_LED6_GREEN_GPIO, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(RGB_LED6_BLUE_GPIO);
	gpio_set_direction(RGB_LED6_BLUE_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_level(RGB_LED6_RED_GPIO, 1);
	gpio_set_level(RGB_LED6_GREEN_GPIO, 1);
	gpio_set_level(RGB_LED6_BLUE_GPIO, 1);
	gpio_pad_select_gpio(RGB_LED8_RED_GPIO);
	gpio_set_direction(RGB_LED8_RED_GPIO, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(RGB_LED8_GREEN_GPIO);
	gpio_set_direction(RGB_LED8_GREEN_GPIO, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(RGB_LED8_BLUE_GPIO);
	gpio_set_direction(RGB_LED8_BLUE_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_level(RGB_LED8_RED_GPIO, 1);
	gpio_set_level(RGB_LED8_GREEN_GPIO, 1);
	gpio_set_level(RGB_LED8_BLUE_GPIO, 1);
}
