/*
 * main_app.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */


#include "FreeRTOS.h"
#include "task.h"
#include "main_app.h"
#include "system_config.h"
#include "system_mode.h"
#include "fan.h"
#include "authentication.h"
#include "light_relay.h"
#include "alarm_light.h"
#include "logger.h"
#include "adc.h"

#define LOW_POWER_MODE TRUE

extern UART_HandleTypeDef huart2;
extern uint8_t system_mode;
extern cli_t cli;
static uint16_t light_counter = 0;
static uint16_t light_timer_sec = 1;
volatile uint32_t system_events = EVENT_EMPTY;  //avoid optimizing it by compiler
void test_task(void *pvParameters);

int main(void)
{
	system_Init();
	//RTC_Set_Time_Date(19, 10, 0, 2, RTC_MONTH_AUGUST, 2026); // Set time to 19:10:00 and date to 2nd August 2026
	xTaskCreate(welcome_task, "welcome_task", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(test_task, "Test_Task", 512, NULL, tskIDLE_PRIORITY + 1, NULL);

	vTaskStartScheduler();
	
	while(1);

	return 0;
}

void test_task(void *pvParameters)
{
	while(1)
	{
		printmsg("Test task running...\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
	}
}

void welcome_task(void)
{
	printmsg("Control Home Unit\r\n");
	printmsg("To enable manual mode type: manual\r\n");
	printmsg("> ");

	vTaskDelete(NULL);
}

