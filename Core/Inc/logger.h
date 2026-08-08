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
#include "stdarg.h"

#define LOG_MESSAGE_MAX_LEN		200

typedef enum
{
	LOG_INFO,
	LOG_DEBUG,
	LOG_WARNING,
	LOG_ERROR
}log_level_t;

void log_debug(const char *format, ...);
void log_info(const char *format, ...);
void log_wrn(const char *format, ...);
void log_error(const char *format, ...);
void printmsg(char *format,...);


#endif /* INC_LOGGER_H_ */
