/*
 * main_app.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */


#include "main_app.h"
#include "system_config.h"
#include "fan.h"
#include "authentication.h"
#include "command_parser.h"
#include "light_relay.h"
#include "alarm_light.h"
#include "time_management.h"

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
static uint16_t light_counter = 0;
static uint16_t light_timer_sec = 1;
volatile uint32_t system_events;  //avoid optimizing it by compiler
uint8_t curr_duty_cycle = 0U;
uint8_t curr_relay_state = RELAY_OFF;
uint8_t curr_alarm_state = ALARM_LIGHT_OFF;
uint8_t system_mode = AUTOMATIC;

int main(void)
{
	system_Init();
	welcome();
	//RTC_Set_Time_Date(19, 10, 0, 2, RTC_MONTH_AUGUST, 2026); // Set time to 19:10:00 and date to 2nd August 2026

	while(1)
	{
		if(system_mode == AUTOMATIC)
		{
			if (system_events & EVENT_TEMP_ADC_SAMPLE)
			{
				char current_time_date[RTC_DATETIME_STR_LEN] = {0};
				RTC_Get_Time_Date(current_time_date, RTC_DATETIME_STR_LEN);
				printmsg("%s: log will be here\r\n", current_time_date);
				system_events &= ~EVENT_TEMP_ADC_SAMPLE;
				float temperature = ADC_Convert_To_Temperature();
				printmsg("Temperature: %.2f °C\r\n", temperature);
				uint8_t new_alarm_state = alarm_light_decision(temperature);
				uint8_t new_duty_cycle = fan_decision(temperature);
				if (curr_alarm_state != new_alarm_state)
				{
					curr_alarm_state = new_alarm_state;
					alarm_light_config(new_alarm_state);
				}
				if (curr_duty_cycle != new_duty_cycle)
				{
					curr_duty_cycle = new_duty_cycle;
					fan_speed_config(new_duty_cycle);
				}
			}

			if (system_events & EVENT_LIGHT_ADC_SAMPLE)
			{
				system_events &= ~EVENT_LIGHT_ADC_SAMPLE;
				uint16_t light_adc_value = ADC_Convert_To_Light(ADC_CHANNEL_LIGHT);
				uint8_t relay_state = light_relay_decision(light_adc_value);
				if (curr_relay_state != relay_state)
				{
					curr_relay_state = relay_state;
					light_relay_config(relay_state);
				}
			}
		}
	}

	return 0;
}

void welcome(void)
{
	printmsg("Control Home Unit\r\n");
	printmsg("To enable manual mode type: manual\r\n");
	printmsg("> ");
}

uint16_t ADC_Read(uint32_t channel)
{

	ADC_Channel_config(channel);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    uint16_t value = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);
    return value;
}

float ADC_Convert_To_Temperature()
{
	uint16_t adc_value = ADC_Read(ADC_CHANNEL_TEMPERATURE);
	// Convert ADC value to temperature in Celsius
	float voltage = ((float)adc_value * 3.3f) / 4095.0f;
	return voltage * 100.0f; 
}

uint16_t ADC_Convert_To_Light()
{
	uint16_t adc_value = ADC_Read(ADC_CHANNEL_LIGHT);
	printmsg("Light Value: %u\r\n", adc_value);

	return adc_value;
}

void ADC_Channel_config(uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	sConfig.Channel = channel; // Select the desired channel
	sConfig.Rank = 1; // Set the rank for the conversion
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES; // Set the sampling time

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_handler();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6)
	{
		system_events |= EVENT_TEMP_ADC_SAMPLE;
		light_counter++;

		if (light_counter >= (light_timer_sec/1)) // 1 is the timer of temperature read
		{
			light_counter = 0;
			system_events |= EVENT_LIGHT_ADC_SAMPLE;
		}
	}
}

void printmsg(char *format,...)
{
  char str[80];

  /*Extract the the argument list using VA apis */
  va_list args;
  va_start(args, format);
  vsprintf(str, format,args);
  HAL_UART_Transmit(&huart2,(uint8_t *)str, strlen(str), HAL_MAX_DELAY);
  va_end(args);
}
