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
#include "queue.h"


#define LOW_POWER_MODE TRUE

extern UART_HandleTypeDef huart2;
extern uint8_t system_mode;
extern cli_t cli;
volatile uint32_t system_events = EVENT_EMPTY;  //avoid optimizing it by compiler
QueueHandle_t sensorQueue;
void sensor_task(void *pvParameters);
void control_task(void *pvParameters);

int main(void)
{
	system_Init();
	//RTC_Set_Time_Date(19, 10, 0, 2, RTC_MONTH_AUGUST, 2026); // Set time to 19:10:00 and date to 2nd August 2026
	sensorQueue = xQueueCreate(10, sizeof(SensorData_t));
	if (sensorQueue == NULL)
	{
		Error_handler();
	}

	xTaskCreate(
		welcome_task, 
		"welcome_task", 
		512, 
		NULL, 
		tskIDLE_PRIORITY + 1, 
		NULL);

	xTaskCreate(
        sensor_task,
        "Sensor",
        512,
        NULL,
        2,
        NULL
    );

	xTaskCreate(
		control_task,
		"Control",
		512,
		NULL,
		3,
		NULL
	);

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

void sensor_task(void *pvParameters)
{
    TickType_t last_temp_sample = xTaskGetTickCount();
    TickType_t last_light_sample = xTaskGetTickCount();

    const TickType_t temp_period = pdMS_TO_TICKS(1000);
    const TickType_t light_period = pdMS_TO_TICKS(1000);

    while (1)
    {
        // Temperature sampling
        if ((xTaskGetTickCount() - last_temp_sample) >= temp_period)
        {
            float temperature = ADC_Convert_To_Temperature();

            SensorData_t message;

			message.type = SENSOR_TEMPERATURE;
			message.data.temperature = ADC_Convert_To_Temperature();

			xQueueSend(sensorQueue, &message, portMAX_DELAY);

            last_temp_sample += temp_period;
        }

        // Light sampling
        if ((xTaskGetTickCount() - last_light_sample) >= light_period)
        {
            uint16_t light_value = ADC_Convert_To_Light();

            SensorData_t message;

			message.type = SENSOR_LIGHT;
			message.data.light_value = ADC_Convert_To_Light();

			xQueueSend(sensorQueue, &message, portMAX_DELAY);

            last_light_sample += light_period;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void control_task(void *pvParameters)
{
    SensorData_t received_data;

    while (1)
    {
        if (xQueueReceive(sensorQueue,
                          &received_data,
                          portMAX_DELAY) == pdPASS)
        {
            switch (received_data.type)
            {
                case SENSOR_TEMPERATURE:

                    fan_control_auto(
                        (uint16_t)received_data.data.temperature
                    );

                    alarm_light_control_auto(
                        (uint16_t)received_data.data.temperature
                    );

                    break;

                case SENSOR_LIGHT:

                    light_relay_control_auto(
                        received_data.data.light_value
                    );

                    break;

                default:
                    break;
            }
        }
    }
}

