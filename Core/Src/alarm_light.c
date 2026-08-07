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
		log_wrn("ATTENTION!!! EXTREMELY HIGH TEMPERATURE");
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

	log_info("alarm light state changed to %d", alarm_status);
}

