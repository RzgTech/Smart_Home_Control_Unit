/*
 * system_mode.c
 *
 *  Created on: Jul 29, 2026
 *      Author: Vahid
 */
#include "system_config.h"
#include "main_app.h"
extern TIM_HandleTypeDef htim6;

void system_mode_transition(uint8_t system_mode)  //transition TO system_mode
{
	switch(system_mode)
	{
	case MANUAL:
		HAL_TIM_Base_Stop_IT(&htim6);
		break;
	case AUTOMATIC:
		HAL_TIM_Base_Start_IT(&htim6);
		break;

	}
}

