/*
 * fan_controller.c
 *
 *  Created on: Jul 26, 2026
 *      Author: Vahid
 */

#include "fan.h"
extern TIM_HandleTypeDef htim2;


void fan_speed_config(uint8_t duty_cycle)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (htim2.Init.Period + 1) * duty_cycle / 100);

	printmsg("fan duty cycle changed to %d\n\r", duty_cycle);
}

