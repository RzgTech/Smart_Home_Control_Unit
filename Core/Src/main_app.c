/*
 * main_app.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */


#include "main_app.h"
#include "authentication.h"

UART_HandleTypeDef huart2;
ADC_HandleTypeDef hadc1;
uint8_t data_buffer[DATA_BUFFER_SIZE];
uint8_t recv_data = 0;
uint16_t ADC_Read(void);
void ADC_Init(void);

int main(void)
{

	memset(data_buffer, 0, sizeof(data_buffer));

	HAL_Init();
	SystemClock_Config(SYS_CLOCK_FREQ_50_MHZ);
	UART_Init();
	ADC_Init();
	welcome();

	while(1)
	{
		uint16_t adc_value = ADC_Read();
		printmsg("ADC Value: %u\r\n", adc_value);
		HAL_Delay(1000);
	}


	HAL_UART_Receive_IT(&huart2, &recv_data, 1);

	while(1);

	return 0;
}

void welcome(void)
{
	printmsg("Control Home Unit\r\n");
	printmsg("To enable manual mode type: manual\r\n");
	printmsg("> ");
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

}

void ADC_Init(void)
{
	
	ADC_ChannelConfTypeDef sConfig = {0};

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

	sConfig.Channel = ADC_CHANNEL_0; // Select the desired channel
	sConfig.Rank = 1; // Set the rank for the conversion
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES; // Set the sampling time

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_handler();
	}
}

uint16_t ADC_Read(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    uint16_t value = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    return value;
}

void printmsg(char *format,...)
{
  char str[80];

  /*Extract the the argument list using VA apis */
  va_list args;
  va_start(args, format);
  vsprintf(str, format,args);
  HAL_UART_Transmit(&huart2,(uint8_t *)str, strlen(str), HAL_MAX_DELAY);
  va_end(args);
}

void Error_handler(void)
{
	while(1);
}
