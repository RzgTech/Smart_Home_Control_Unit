#include "fan.h"

extern TIM_HandleTypeDef htim2;

uint8_t fan_decision(uint16_t temperature)
{
	uint8_t duty_cycle = 0;

    if (temperature <= 10)
    {
    	duty_cycle = FAN_OFF;
    }
    else if (10 < temperature &&  temperature <= 15)
    {
    	duty_cycle = 10*FAN_10;
    }
    else if (15 < temperature &&  temperature <= 20)
    {
    	duty_cycle = 10*FAN_20;
    }
    else if (20 < temperature &&  temperature <= 25)
    {
    	duty_cycle = 10*FAN_30;
    }
    else if (25 < temperature &&  temperature <= 35)
    {
    	duty_cycle = 10*FAN_40;
    }
    else if (35 < temperature &&  temperature <= 45)
    {
    	duty_cycle = 10*FAN_50;
    }
    else if (45 < temperature &&  temperature <= 55)
    {
    	duty_cycle = 10*FAN_60;
    }
    else if (55 < temperature &&  temperature <= 70)
    {
    	duty_cycle = 10*FAN_70;
    }
    else if (70 < temperature &&  temperature <= 80)
    {
    	duty_cycle = 10*FAN_80;
    }
    else if (80 < temperature &&  temperature <= 90)
    {
    	duty_cycle = 10*FAN_90;
    }
    else
    {
    	duty_cycle = 10*FAN_100;
    }

    
    return duty_cycle;
}

void fan_speed_config(uint8_t duty_cycle)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (htim2.Init.Period + 1) * duty_cycle / 100);

	log_info("fan duty cycle changed to %d", duty_cycle);
}
