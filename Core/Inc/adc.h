/*
 * adc.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Vahid
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#define ADC_CHANNEL_TEMPERATURE		ADC_CHANNEL_0
#define ADC_CHANNEL_LIGHT			ADC_CHANNEL_1

uint16_t ADC_Read(uint32_t channel);
void ADC_Channel_config(uint32_t channel);
float ADC_Convert_To_Temperature();
uint16_t ADC_Convert_To_Light();

#endif /* INC_ADC_H_ */
