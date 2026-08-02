/*
 * alarm_light.c
 *
 *  Created on: Aug 1, 2026
 *      Author: Vahid
 */

#include "alarm_light.h"

uint8_t alarm_light_decision(uint16_t temperature)
{
	uint8_t alarm_light_status;

	if (temperature > TEMPERATURE_THRESHOLD)
	{
		printmsg("ATTENTION!!! EXTREMELY HIGH TEMPERATURE\n\r");
		alarm_light_status = ALARM_LIGHT_ON;
	}
	else
	{
		alarm_light_status = ALARM_LIGHT_OFF;
	}

	return alarm_light_status;
}

void alarm_light_config(uint8_t alarm_status)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, alarm_status);

	printmsg("alarm light state changed to %d\n\r", alarm_status);
}

