/*
 * msp.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */

#include "msp.h"

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
}
