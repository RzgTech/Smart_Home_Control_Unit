/*
 * light.h
 *
 *  Created on: Jul 30, 2026
 *      Author: Vahid
 */

#ifndef INC_LIGHT_RELAY_H_
#define INC_LIGHT_RELAY_H_

#define LIGHT_MODE_THRESHOLD	3000

#define RELAY_ON				1
#define RELAY_OFF				0

uint8_t light_relay_decision(uint16_t light_value);
void light_relay_config(uint8_t relay_state);
void light_relay_control_auto(uint16_t light_adc_value);



#endif /* INC_LIGHT_RELAY_H_ */
