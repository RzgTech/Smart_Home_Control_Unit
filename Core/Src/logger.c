/*
 * logger.c
 *
 *  Created on: Aug 5, 2026
 *      Author: Vahid
 */
#include "logger.h"
extern uint8_t system_mode;
static const char *log_level_str[] =
{
	[LOG_INFO]    = "INFO",
	[LOG_DEBUG]   = "DEBUG",
	[LOG_WARNING] = "WARNING",
	[LOG_ERROR]   = "ERROR"
};  //https://chatgpt.com/g/g-p-6a537f94b6f481918c5f72ad900a5271-smart-home-control-unit-project/c/6a73a959-f0a8-83ed-9cfe-1c2a0742e141

static const char *system_mode_str[] =
{
	[AUTOMATIC] = "AUTO",
	[MANUAL]    = "MANUAL"
};

static log_level_t current_log_level = LOG_DEBUG; //configure this to get only the desired types of logs

static void log_message(const log_level_t level, const char* format, va_list args) //this is a static function, if you declare it in logger.h file
{																      //it gives you a warning bcs it must be private only to the .c file
	if (level < current_log_level)
	{
		return;
	}

    char log_message[LOG_MESSAGE_MAX_LEN];
    char in_message[LOG_MESSAGE_MAX_LEN];
    char timestamp[RTC_DATETIME_STR_LEN];


	vsnprintf(in_message,
	             sizeof(in_message),
				 format,
				 args);

    RTC_Get_Time_Date(timestamp);

    snprintf(log_message,
    		sizeof(log_message),
             "[%s] [%s] %s: %s\r\n",
			 log_level_str[level],
			 system_mode_str[system_mode],
             timestamp,
			 in_message);


    printmsg(log_message);
}

void log_info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
	log_message(LOG_INFO, format, args);
	va_end(args);
}

void log_debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
	log_message(LOG_DEBUG, format, args);
	va_end(args);
}


void log_wrn(const char *format, ...)
{
    va_list args;
    va_start(args, format);
	log_message(LOG_WARNING, format, args);
	va_end(args);
}

void log_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
	log_message(LOG_ERROR, format, args);
	va_end(args);
}



