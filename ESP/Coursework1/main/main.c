#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include <rom/ets_sys.h>

//Pulse Train Pararmeters
#define a 1000
#define b 1200
#define c 11
#define d 6500

//Output GPIO pins definitions (bitmask)
#define GPIO_OUTPUT_DATA GPIO_NUM_1
#define GPIO_OUTPUT_SYNC GPIO_NUM_2
#define GPIO_OUTPUT_PIN_SELECT (1ULL << GPIO_OUTPUT_DATA | 1ULL << GPIO_OUTPUT_SYNC)

//Input GPIO pins definitions (bitmask)
#define GPIO_INPUT_ENABLE GPIO_NUM_5
#define GPIO_INPUT_SELECT GPIO_NUM_6
#define GPIO_INPUT_PIN_SELECT (1ULL << GPIO_INPUT_ENABLE | 1ULL << GPIO_INPUT_SELECT)

//macros
#define get_param_C c - (OUTPUT_SELECT.state ? 3 : 0)
#define get_param_T(n) a + ((n-1) * 50)
#define flip(x) x = x ? 0 : 1

struct button_state_t
{
  uint8_t pin;
  bool isOn;
  bool state;
};
typedef struct button_state_t button_state_t;

//prototype definitions
void create_pulse_train();
void create_sync_pulse();
void create_data_pulse(int onDuration, int offDuration);
void checkInputPin(button_state_t *state);


//variable sturct setups
struct button_state_t OUTPUT_ENABLE = {
    .pin = GPIO_INPUT_ENABLE,
    .isOn = 0,
    .state = 1};

struct button_state_t OUTPUT_SELECT = {
    .pin = GPIO_INPUT_SELECT,
    .isOn = 0,
    .state = 0};

gpio_config_t config_input = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_INPUT,
    .pin_bit_mask = GPIO_INPUT_PIN_SELECT,
    .pull_down_en = 1,
    .pull_up_en = 0};

gpio_config_t config_output = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = GPIO_OUTPUT_PIN_SELECT,
    .pull_down_en = 0,
    .pull_up_en = 0};

void app_main(void)
{
  //setup
  gpio_config(&config_input);
  gpio_config(&config_output);

  //loop
  while (1)
  {
    //check if inputs have changed before starting new pulse train
    checkInputPin(&OUTPUT_ENABLE);
    checkInputPin(&OUTPUT_SELECT);
    if (OUTPUT_ENABLE.state)
    {
      create_pulse_train();
    }
  }
}

void create_pulse_train()
{
  create_sync_pulse();
  for (int i = 1; i <= get_param_C; i++)
  {
    create_data_pulse(get_param_T(i), b);
  }
  gpio_set_level(GPIO_OUTPUT_DATA, 0);
  ets_delay_us(d);
}

void create_sync_pulse()
{
  gpio_set_level(GPIO_OUTPUT_SYNC, 1);
  ets_delay_us(50);
  gpio_set_level(GPIO_OUTPUT_SYNC, 0);
}

void create_data_pulse(int onDuration, int offDuration)
{
  gpio_set_level(GPIO_OUTPUT_DATA, 1);
  ets_delay_us(onDuration);
  gpio_set_level(GPIO_OUTPUT_DATA, 0);
  ets_delay_us(offDuration);
}

void checkInputPin(button_state_t *state){
  bool input = gpio_get_level(state->pin);
  if (input){
    flip(state->state);
  }

  //pause input until button is unpressed to avoid infinite toggling / faulty buttons
  while(gpio_get_level(state->pin)){
    continue;
  }
}
