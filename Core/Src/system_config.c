/*
 * system_config.c
 *
 *  Created on: Jul 28, 2026
 *      Author: Vahid
 */

#include "system_config.h"
#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart2;
ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim2;

uint8_t recv_data = 0;

void system_Init(void)
{
	HAL_Init();
	SystemClock_Config(SYS_CLOCK_FREQ_50_MHZ);
	RELAY_GPIO_Init();
	TIM6_init();
	TIM2_init();
	UART_Init();
	ADC_Init();
}

void SystemClock_Config(uint8_t frequency)
{
	RCC_OscInitTypeDef osc_config;
	RCC_ClkInitTypeDef clk_config;
	uint8_t FLatency = 0;

	osc_config.HSEState = RCC_HSE_BYPASS;
	osc_config.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_config.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	osc_config.PLL.PLLState = RCC_PLL_ON;

	switch(frequency)
	{
	case SYS_CLOCK_FREQ_25_MHZ:
	{
		FLatency = FLASH_ACR_LATENCY_0WS;
		osc_config.PLL.PLLM = 8;
		osc_config.PLL.PLLN = 100;
		osc_config.PLL.PLLP = 4;
		osc_config.PLL.PLLR = 2;
		osc_config.PLL.PLLQ = 2;
		clk_config.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_config.APB1CLKDivider = RCC_HCLK_DIV1;
		clk_config.APB2CLKDivider = RCC_HCLK_DIV1;
		clk_config.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 \
				| RCC_CLOCKTYPE_PCLK2;
		clk_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		break;
	}

	case SYS_CLOCK_FREQ_50_MHZ:
	{
		FLatency = FLASH_ACR_LATENCY_1WS;
		osc_config.PLL.PLLM = 8;
		osc_config.PLL.PLLN = 100;
		osc_config.PLL.PLLP = 2;
		osc_config.PLL.PLLR = 2;
		osc_config.PLL.PLLQ = 2;
		clk_config.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_config.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_config.APB2CLKDivider = RCC_HCLK_DIV2;
		clk_config.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 \
				| RCC_CLOCKTYPE_PCLK2;
		clk_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		break;
	}

	case SYS_CLOCK_FREQ_80_MHZ:
	{

		FLatency = FLASH_ACR_LATENCY_2WS;
		osc_config.PLL.PLLM = 8;
		osc_config.PLL.PLLN = 320;
		osc_config.PLL.PLLP = 4;
		osc_config.PLL.PLLR = 2;
		osc_config.PLL.PLLQ = 2;
		clk_config.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_config.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_config.APB2CLKDivider = RCC_HCLK_DIV1;
		clk_config.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 \
				| RCC_CLOCKTYPE_PCLK2;
		clk_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		break;
	}

	case SYS_CLOCK_FREQ_180_MHZ:
	{

		//enable the clock for the power controller
		__HAL_RCC_PWR_CLK_ENABLE();

		//set regulator voltage scale as 1
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		//turn on the over driver mode of the voltage regulator
		__HAL_PWR_OVERDRIVESWITCHING_ENABLE();

		FLatency = FLASH_ACR_LATENCY_5WS;
		osc_config.PLL.PLLM = 8;
		osc_config.PLL.PLLN = 360;
		osc_config.PLL.PLLP = 2;
		osc_config.PLL.PLLR = 2;
		osc_config.PLL.PLLQ = 2;
		clk_config.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_config.APB1CLKDivider = RCC_HCLK_DIV4;
		clk_config.APB2CLKDivider = RCC_HCLK_DIV4;
		clk_config.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 \
				| RCC_CLOCKTYPE_PCLK2;
		clk_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		break;
	}


	default:
		printf("Invalid clock frequency\n");
		return;
	}

	if (HAL_RCC_OscConfig(&osc_config) != HAL_OK)
	{
		Error_handler();
	}

	if (HAL_RCC_ClockConfig(&clk_config, FLatency) != HAL_OK)
	{
		Error_handler();
	}


	//Systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  //systick generates an interrupt every 1 ms
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);  //configuring systick timer

}

void UART_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_handler();
	}

	HAL_UART_Receive_IT(&huart2, &recv_data, 1);

}

void ADC_Init(void)
{
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_handler();
	}
}

//used for temperature sensor readings
void TIM6_init(void)
{
	htim6.Instance = TIM6;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	//timer of 1 sec
	htim6.Init.Prescaler = 49999;
	htim6.Init.Period = 999;

	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		Error_handler();
	}

	HAL_TIM_Base_Start_IT(&htim6);
}

//used for generating PWM signal for fan speed control
void TIM2_init(void)
{
	TIM_OC_InitTypeDef tim2pwm_config;
	memset(&tim2pwm_config, 0, sizeof(tim2pwm_config));

	htim2.Instance = TIM2;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Prescaler = 49999;
	htim2.Init.Period = 9;  //fan period

	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_handler();
	}

	tim2pwm_config.OCMode = TIM_OCMODE_PWM1;
	tim2pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim2pwm_config.Pulse = 0;

	if (HAL_TIM_PWM_ConfigChannel(&htim2, &tim2pwm_config, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

}

void RELAY_GPIO_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	//configuring GPIO for the light relay
	GPIO_InitTypeDef relay_gpio;

	relay_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	relay_gpio.Pin = GPIO_PIN_0;
	relay_gpio.Pull = GPIO_NOPULL;
	relay_gpio.Speed = GPIO_SPEED_LOW;

	HAL_GPIO_Init(GPIOB, &relay_gpio);
}

void Error_handler(void)
{
	while(1);
}





