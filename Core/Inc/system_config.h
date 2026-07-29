/*
 * system_config.h
 *
 *  Created on: Jul 28, 2026
 *      Author: Vahid
 */

#ifndef INC_SYSTEM_CONFIG_H_
#define INC_SYSTEM_CONFIG_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define SYS_CLOCK_FREQ_25_MHZ		25
#define SYS_CLOCK_FREQ_50_MHZ		50
#define SYS_CLOCK_FREQ_80_MHZ		80
#define SYS_CLOCK_FREQ_180_MHZ		180

void SystemClock_Config(uint8_t frequency);
void UART_Init(void);
void ADC_Init(void);
void TIM6_init(void);
void TIM2_init(void);
void system_Init(void);
void Error_handler(void);


#endif /* INC_SYSTEM_CONFIG_H_ */
