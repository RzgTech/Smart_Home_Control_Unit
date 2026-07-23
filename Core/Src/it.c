/*
 * it.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */

//without this, after the systick interrupt happens (here after 1ms) the app goes to the default weak systick_handler and gets stuck there
#include "main_app.h"
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim6;

void SysTick_Handler(void)
{

  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();

}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);

}

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim6);
}
