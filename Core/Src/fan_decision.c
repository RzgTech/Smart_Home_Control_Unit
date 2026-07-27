#include "fan.h"
uint16_t duty_state_token = 0xFF;

uint8_t fan_decision(uint16_t temperature)
{
	uint8_t duty_cycle = 0;

    if (temperature <= 10)
    {
    	duty_cycle = 0;
    	duty_state_token = 1U<<0;
    }
    else if (10 < temperature &&  temperature <= 15)
    {
    	duty_cycle = 10;
    	duty_state_token = 1U<<1;
    }
    else if (15 < temperature &&  temperature <= 20)
    {
    	duty_cycle = 20;
    	duty_state_token = 1U<<2;
    }
    else if (20 < temperature &&  temperature <= 25)
    {
    	duty_cycle = 30;
    	duty_state_token = 1U<<3;
    }
    else if (25 < temperature &&  temperature <= 35)
    {
    	duty_cycle = 40;
    	duty_state_token = 1U<<4;
    }
    else if (35 < temperature &&  temperature <= 45)
    {
    	duty_cycle = 50;
    	duty_state_token = 1U<<5;
    }
    else if (45 < temperature &&  temperature <= 55)
    {
    	duty_cycle = 60;
    	duty_state_token = 1U<<6;
    }
    else if (55 < temperature &&  temperature <= 70)
    {
    	duty_cycle = 70;
    	duty_state_token = 1U<<7;
    }
    else if (70 < temperature &&  temperature <= 80)
    {
    	duty_cycle = 80;
    	duty_state_token = 1U<<8;
    }
    else if (80 < temperature &&  temperature <= 90)
    {
    	duty_cycle = 90;
    	duty_state_token = 1U<<9;
    }
    else
    {
    	duty_cycle = 100;
    	duty_state_token = 1U<<10;
    }

    
    return duty_cycle;
}

