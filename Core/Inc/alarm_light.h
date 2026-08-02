/*
 * alarm_light.h
 *
 *  Created on: Aug 1, 2026
 *      Author: Vahid
 */

#ifndef INC_ALARM_LIGHT_H_
#define INC_ALARM_LIGHT_H_

#include "main_app.h"

#define TEMPERATURE_THRESHOLD			50
#define ALARM_LIGHT_ON					1
#define ALARM_LIGHT_OFF					0

uint8_t alarm_light_decision(uint16_t temperature);
void alarm_light_config(uint8_t alarm_status);

#endif /* INC_ALARM_LIGHT_H_ */
