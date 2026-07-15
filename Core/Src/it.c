/*
 * it.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */

void SysTick_Handler(void)
{

  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();

}
