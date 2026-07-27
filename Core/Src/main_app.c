/*
 * main_app.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Vahid
 */


#include "main_app.h"
#include "fan.h"
#include "authentication.h"

UART_HandleTypeDef huart2;
ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim2;
uint8_t recv_data = 0;
static uint16_t light_counter = 0;
static uint16_t light_timer_sec = 20;
volatile uint32_t system_events;  //avoid optimizing it by compiler
uint8_t curr_duty_cycle = 0U;

int main(void)
{

	HAL_Init();
	SystemClock_Config(SYS_CLOCK_FREQ_50_MHZ);
	TIM6_init();
	TIM2_init();
	UART_Init();
	ADC_Init();
	welcome();

	HAL_TIM_Base_Start_IT(&htim6);

	HAL_UART_Receive_IT(&huart2, &recv_data, 1);

	while(1)
	{
		if (system_events & EVENT_TEMP_ADC_SAMPLE)
		{
			system_events &= ~EVENT_TEMP_ADC_SAMPLE;
			float temperature = ADC_Convert_To_Temperature();
			printmsg("Temperature: %.2f °C\r\n", temperature);
			uint8_t new_duty_cycle = fan_decision(temperature);
			if (curr_duty_cycle != new_duty_cycle)
			{
				curr_duty_cycle = new_duty_cycle;
				fan_speed_config(new_duty_cycle);
			}
		}

		if (system_events & EVENT_LIGHT_ADC_SAMPLE)
		{
			system_events &= ~EVENT_LIGHT_ADC_SAMPLE;
			ADC_Read(ADC_CHANNEL_LIGHT);
		}
	}

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

uint16_t ADC_Read(uint32_t channel)
{

	ADC_Channel_config(channel);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    uint16_t value = HAL_ADC_GetValue(&hadc1);
    if(channel == ADC_CHANNEL_LIGHT)
	{
    	printmsg("Light Value: %u\r\n", value);
	}

    HAL_ADC_Stop(&hadc1);
    return value;
}

float ADC_Convert_To_Temperature()
{
	uint16_t adc_value = ADC_Read(ADC_CHANNEL_TEMPERATURE);
	// Convert ADC value to temperature in Celsius
	float voltage = ((float)adc_value * 3.3f) / 4095.0f;
	return voltage * 100.0f; 
}

void ADC_Channel_config(uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	sConfig.Channel = channel; // Select the desired channel
	sConfig.Rank = 1; // Set the rank for the conversion
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES; // Set the sampling time

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_handler();
	}
}

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

}

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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6)
	{
		system_events |= EVENT_TEMP_ADC_SAMPLE;
		light_counter++;

		if (light_counter >= (light_timer_sec/5)) // 5 is the timer of temperature read
		{
			light_counter = 0;
			system_events |= EVENT_LIGHT_ADC_SAMPLE;
		}
	}
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
