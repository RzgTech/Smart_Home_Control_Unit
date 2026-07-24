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

#define SYS_CLOCK_FREQ_25_MHZ		25
#define SYS_CLOCK_FREQ_50_MHZ		50
#define SYS_CLOCK_FREQ_80_MHZ		80
#define SYS_CLOCK_FREQ_180_MHZ		180
#define DATA_BUFFER_SIZE			100

#define FALSE	0
#define TRUE	1

#define ADC_CHANNEL_TEMPERATURE		ADC_CHANNEL_0
#define ADC_CHANNEL_LIGHT			ADC_CHANNEL_1

#define EVENT_UART_RX    			(1U << 0)
#define EVENT_TEMP_ADC_SAMPLE       (1U << 1)
#define EVENT_LIGHT_ADC_SAMPLE		(1U << 2)

void SystemClock_Config(uint8_t frequency);
void Error_handler(void);
void UART_Init(void);
void printmsg(char *format,...);
void welcome(void);
void command_parser(uint8_t* data_buffer);




#endif /* INC_MAIN_APP_H_ */
