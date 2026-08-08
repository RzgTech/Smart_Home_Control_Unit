/*
 * logger.c
 *
 *  Created on: Aug 5, 2026
 *      Author: Vahid
 */
#include "logger.h"
#include "system_mode.h"
#include "main_app.h"
extern uint8_t system_mode;
extern UART_HandleTypeDef huart2;
extern const char *system_mode_str[];

static const char *log_level_str[] =
{
	[LOG_INFO]    = "INFO",
	[LOG_DEBUG]   = "DEBUG",
	[LOG_WARNING] = "WARNING",
	[LOG_ERROR]   = "ERROR"
};  //https://chatgpt.com/g/g-p-6a537f94b6f481918c5f72ad900a5271-smart-home-control-unit-project/c/6a73a959-f0a8-83ed-9cfe-1c2a0742e141

static log_level_t current_log_level = LOG_INFO; //configure this to get only the desired types of logs

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

    HAL_UART_Transmit(&huart2,(uint8_t *)log_message, strlen(log_message), HAL_MAX_DELAY);
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



