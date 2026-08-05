/*
 * logger.c
 *
 *  Created on: Aug 5, 2026
 *      Author: Vahid
 */
#include "logger.h"

static const char *log_level_str[] =
{
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

static log_level_t current_log_level = LOG_DEBUG; //configure this to get only the desired types of logs

static void log_message(const log_level_t level, const char *message) //this is a static function, if you declare it in logger.h file
{																      //it gives you a warning bcs it must be private only to the .c file
	if (level < current_log_level)
	{
		return;
	}

    char log_message[LOG_MESSAGE_MAX_LEN];
    char timestamp[RTC_DATETIME_STR_LEN];

    RTC_Get_Time_Date(timestamp);

    snprintf(log_message,
             sizeof(log_message),
             "[%s] %s: %s\r\n",
			 log_level_str[level],
             timestamp,
             message);

    printmsg(log_message);
}

void log_debug(char* message)
{
	log_message(LOG_DEBUG, message);
}

void log_info(char* message)
{
	log_message(LOG_INFO, message);
}

void log_wrn(char* message)
{
	log_message(LOG_WARNING, message);
}

void log_error(char* message)
{
	log_message(LOG_ERROR, message);
}



