/*
 * main_app.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */


#include "main_app.h"
#include "system_config.h"
#include "system_mode.h"
#include "fan.h"
#include "authentication.h"
#include "command_parser.h"
#include "light_relay.h"
#include "alarm_light.h"
#include "time_management.h"
#include "logger.h"
#include "adc.h"

extern UART_HandleTypeDef huart2;
extern uint8_t system_mode;
extern cli_t cli;
static uint16_t light_counter = 0;
static uint16_t light_timer_sec = 1;
volatile uint32_t system_events = EVENT_EMPTY;  //avoid optimizing it by compiler
uint8_t curr_relay_state = RELAY_OFF;
uint8_t curr_alarm_state = ALARM_LIGHT_OFF;

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
				system_events &= ~EVENT_TEMP_ADC_SAMPLE;
				float temperature = ADC_Convert_To_Temperature();
				uint8_t new_alarm_state = alarm_light_decision(temperature);
				if (curr_alarm_state != new_alarm_state)
				{
					curr_alarm_state = new_alarm_state;
					alarm_light_config(new_alarm_state);
				}
				fan_control_auto();
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

		//going to sleep
		if (system_events == EVENT_EMPTY)
		{
			if (system_mode == AUTOMATIC && cli.user_auth_stat != USER_AUTHENTICATED)
			{
				log_info("Entering sleep mode...");
				HAL_SuspendTick();
				__WFI();
				HAL_ResumeTick();
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
