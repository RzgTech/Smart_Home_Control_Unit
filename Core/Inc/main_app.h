/*
 * main_app.h
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */

#ifndef INC_MAIN_APP_H_
#define INC_MAIN_APP_H_

#include "stm32f4xx_hal.h"
#include "logger.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define DATA_BUFFER_SIZE			100

#define FALSE	0
#define TRUE	1


#define EVENT_EMPTY					(0U)
#define EVENT_UART_RX    			(1U << 0)
#define EVENT_TEMP_ADC_SAMPLE       (1U << 1)
#define EVENT_LIGHT_ADC_SAMPLE		(1U << 2)

typedef enum
{
    SENSOR_TEMPERATURE,
    SENSOR_LIGHT
} SensorType_t;

typedef struct
{
    SensorType_t type;

    union
    {
        float temperature;
        uint16_t light_value;
    } data;

} SensorData_t;

void welcome_task(void);

#endif /* INC_MAIN_APP_H_ */
