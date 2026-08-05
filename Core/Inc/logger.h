/*
 * logger.h
 *
 *  Created on: Aug 5, 2026
 *      Author: Vahid
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "time_management.h"
#include "main_app.h"

#define LOG_MESSAGE_MAX_LEN		200

typedef enum
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
}log_level_t;

void log_debug(char* message);
void log_info(char* message);
void log_wrn(char* message);
void log_error(char* message);


#endif /* INC_LOGGER_H_ */
