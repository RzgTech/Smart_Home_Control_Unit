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

#define FALSE	0
#define TRUE	1

void SystemClock_Config(uint8_t frequency);
void Error_handler(void);
void UART_Init(void);
void printmsg(char *format,...);
void welcome(void);
void command_parser(uint8_t* data_buffer, uint8_t data_buffer_size);
void buffer_cleaner(uint8_t* buffer, uint8_t data_buffer_size);




#endif /* INC_MAIN_APP_H_ */
