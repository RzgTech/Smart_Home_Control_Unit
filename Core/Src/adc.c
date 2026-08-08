/*
 * adc.c
 *
 *  Created on: Aug 8, 2026
 *      Author: Vahid
 */
#include "main_app.h"
#include "adc.h"

extern ADC_HandleTypeDef hadc1;

uint16_t ADC_Read(uint32_t channel)
{

	ADC_Channel_config(channel);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    uint16_t value = HAL_ADC_GetValue(&hadc1);

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

uint16_t ADC_Convert_To_Light()
{
	uint16_t adc_value = ADC_Read(ADC_CHANNEL_LIGHT);
	log_debug("Light Value is: %u", adc_value);

	return adc_value;
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
