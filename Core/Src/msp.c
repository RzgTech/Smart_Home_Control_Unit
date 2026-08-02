/*
 * msp.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */

#include "msp.h"
#include <string.h>

void HAL_MspInit(void)
{
	//here we will do low level processor specific inits
	//1. set up priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); //this is not required bcs by default it is set to NVIC_PRIORITYGROUP_4
	//2. enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= (0x7 << 16);  //enabling mem,bus,usage fault exceptions
	//3. configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0); //making both the preempt and subpriority = 0
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	//Enabling clock
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//configuring GPIO PINs for UART
	gpio_uart.Alternate = GPIO_AF7_USART2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	//configuring interrupts
	//set priority
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);

	//enable interrupts
	HAL_NVIC_EnableIRQ(USART2_IRQn);

}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
	GPIO_InitTypeDef gpio_adc;

	//Enabling clock
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//configuring GPIO PINs for ADC
	gpio_adc.Mode = GPIO_MODE_ANALOG;
	gpio_adc.Pin = GPIO_PIN_0;
	gpio_adc.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &gpio_adc);

	gpio_adc.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &gpio_adc);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	//configuring interrupts
	//set priority
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);

	//enable interrupts
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//configuring GPIO for the fan
	GPIO_InitTypeDef tim2ch1_gpio;

	tim2ch1_gpio.Alternate = GPIO_AF1_TIM2;
	tim2ch1_gpio.Mode = GPIO_MODE_AF_PP;
	tim2ch1_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	tim2ch1_gpio.Pull = GPIO_NOPULL;
	tim2ch1_gpio.Pin = GPIO_PIN_5;

	HAL_GPIO_Init(GPIOA, &tim2ch1_gpio);

	//configuring interrupts
	//set priority
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);

	//enable interrupts
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	//use this in case the backup sram was changed
	/*// Enable PWR clock
	__HAL_RCC_PWR_CLK_ENABLE();

	// Enable access to backup domain
	HAL_PWR_EnableBkUpAccess();

	// Force backup domain reset
	__HAL_RCC_BACKUPRESET_FORCE();
	__HAL_RCC_BACKUPRESET_RELEASE();*/

	//reminder: RTC is inside backup domain so its clock enabling is different from other peripherals

	//1. Turn on the LSE
	RCC_OscInitTypeDef osc_config;
	RCC_PeriphCLKInitTypeDef clk_config;

	memset(&osc_config, 0, sizeof(osc_config));
	memset(&clk_config, 0, sizeof(clk_config));

	osc_config.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	osc_config.LSEState = RCC_LSE_ON;
	osc_config.PLL.PLLState = RCC_PLL_NONE;
	osc_config.HSEState = RCC_HSE_OFF;
	osc_config.HSIState = RCC_HSI_OFF;
	osc_config.LSIState = RCC_LSI_OFF;
	
	if (HAL_RCC_OscConfig(&osc_config) != HAL_OK)
	{
		Error_handler();
	}

	//2. Select LSE as the RTC clock source
	clk_config.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	clk_config.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	
	if (HAL_RCCEx_PeriphCLKConfig(&clk_config) != HAL_OK)
	{
		Error_handler();
	}

	//3. Enable the RTC peripheral clock
	__HAL_RCC_RTC_ENABLE();
	
}

