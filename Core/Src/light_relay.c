/*
 * light.c
 *
 *  Created on: Jul 30, 2026
 *      Author: Vahid
 */
#include "main_app.h"
#include "light_relay.h"

uint8_t light_relay_status;

uint8_t light_relay_decision(uint16_t light_value)
{
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

	printmsg("light relay state changed to %d\n\r", relay_state);
}


