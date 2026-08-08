/*
 * alarm_light.c
 *
 *  Created on: Aug 1, 2026
 *      Author: Vahid
 */

#include "alarm_light.h"
#include "adc.h"

uint8_t curr_alarm_state = ALARM_LIGHT_OFF;

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

void alarm_light_control_auto(uint16_t temperature)
{
	uint8_t new_alarm_state = alarm_light_decision(temperature);
	if (curr_alarm_state != new_alarm_state)
	{
		curr_alarm_state = new_alarm_state;
		alarm_light_config(new_alarm_state);
	}
}

