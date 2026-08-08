/*
 * system_mode.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Vahid
 */

#ifndef INC_SYSTEM_MODE_H_
#define INC_SYSTEM_MODE_H_

#include "main_app.h"
typedef enum
{
	STARTUP,
	AUTOMATIC,
	MANUAL
}system_mode_t;

void system_mode_transition(uint8_t system_mode);

#endif /* INC_SYSTEM_MODE_H_ */
