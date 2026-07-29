/*
 * main_app.h
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */

#ifndef INC_MAIN_APP_H_
#define INC_MAIN_APP_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define DATA_BUFFER_SIZE			100

#define FALSE	0
#define TRUE	1

#define ADC_CHANNEL_TEMPERATURE		ADC_CHANNEL_0
#define ADC_CHANNEL_LIGHT			ADC_CHANNEL_1

#define EVENT_UART_RX    			(1U << 0)
#define EVENT_TEMP_ADC_SAMPLE       (1U << 1)
#define EVENT_LIGHT_ADC_SAMPLE		(1U << 2)

void Error_handler(void);
void printmsg(char *format,...);
void welcome(void);
void command_parser(uint8_t* data_buffer);
uint16_t ADC_Read(uint32_t channel);
void ADC_Channel_config(uint32_t channel);
float ADC_Convert_To_Temperature();

typedef enum
{
	AUTOMATIC,
	MANUAL
}system_mode_t;



#endif /* INC_MAIN_APP_H_ */
