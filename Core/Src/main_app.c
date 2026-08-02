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

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;
void RTC_Init(void);
char *RTC_Get_Time_Date(char *buffer, uint8_t buffer_len);
void RTC_Set_Time_Date(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year);
#define RTC_DATETIME_STR_LEN   32
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
	RTC_Init();
	welcome();
	RTC_Set_Time_Date(19, 10, 0, 2, RTC_MONTH_AUGUST, 2026); // Set time to 19:10:00 and date to 2nd August 2026

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

void RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 0x7F;
	hrtc.Init.SynchPrediv = 0xFF;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW; //does not matter since output is disabled
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN; //does not matter since output is disabled

	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_handler();
	}
}

void RTC_Set_Time_Date(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	sTime.Hours = hours;
	sTime.Minutes = minutes;
	sTime.Seconds = seconds;
	sTime.SubSeconds = 0;
	sTime.TimeFormat = RTC_HOURFORMAT_24;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	sDate.Month = month;
	sDate.Date = day;
	sDate.Year = year - 2000; // Assuming year is in 2000s

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}
}

char *RTC_Get_Time_Date(char *buffer, uint8_t buffer_len)
{
	RTC_TimeTypeDef currTime = {0};
	RTC_DateTypeDef currDate = {0};

	if (HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	if (HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	snprintf(buffer,
			buffer_len,
			"%02d/%02d/%04d - %02d:%02d:%02d",
			currDate.Date,
			currDate.Month,
			2000 + currDate.Year,
			currTime.Hours,
			currTime.Minutes,
			currTime.Seconds);

	printmsg("Current Time: %02d:%02d:%02d\r\n", currTime.Hours, currTime.Minutes, currTime.Seconds);
	printmsg("Current Date: %02d/%02d/%04d\r\n", currDate.Date, currDate.Month, 2000 + currDate.Year);	

	return buffer;
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
