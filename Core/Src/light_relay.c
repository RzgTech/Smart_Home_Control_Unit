/*
 * light.c
 *
 *  Created on: Jul 30, 2026
 *      Author: Vahid
 */
#include "main_app.h"
#include "light_relay.h"

uint8_t curr_relay_state = RELAY_OFF;

uint8_t light_relay_decision(uint16_t light_value)
{
	uint8_t light_relay_status;
	if (light_value < LIGHT_MODE_THRESHOLD)
	{
		light_relay_status = RELAY_OFF;
	}
	else
	{
		light_relay_status = RELAY_ON;
	}

	return light_relay_status;
}

void light_relay_config(uint8_t relay_state)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, relay_state);

	log_info("light relay state changed to %d", relay_state);
}

void light_relay_control_auto(uint16_t light_adc_value)
{
	uint8_t relay_state = light_relay_decision(light_adc_value);
	if (curr_relay_state != relay_state)
	{
		curr_relay_state = relay_state;
		light_relay_config(relay_state);
	}
}


