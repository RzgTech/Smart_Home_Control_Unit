/*
 * logger.c
 *
 *  Created on: Aug 5, 2026
 *      Author: Vahid
 */
#include "logger.h"

void log_debug(char* message)
{
	char debug_message[LOG_MESSAGE_MAX_LEN];
	char timesatmp[RTC_DATETIME_STR_LEN] = {0};
	RTC_Get_Time_Date(timesatmp);
	sprintf(debug_message, "[DEBUG] %s: %s\r\n", timesatmp, message);
	printmsg(debug_message);
}

void log_info(char* message)
{
	char info_message[LOG_MESSAGE_MAX_LEN];
	char timesatmp[RTC_DATETIME_STR_LEN] = {0};
	RTC_Get_Time_Date(timesatmp);
	sprintf(info_message, "[INFO] %s: %s\r\n", timesatmp, message);
	printmsg(info_message);
}

void log_wrn(char* message)
{
	char wrn_message[LOG_MESSAGE_MAX_LEN];
	char timesatmp[RTC_DATETIME_STR_LEN] = {0};
	RTC_Get_Time_Date(timesatmp);
	sprintf(wrn_message, "[WARNING] %s: %s\r\n", timesatmp, message);
	printmsg(wrn_message);
}

void log_error(char* message)
{
	char err_message[LOG_MESSAGE_MAX_LEN];
	char timesatmp[RTC_DATETIME_STR_LEN] = {0};
	RTC_Get_Time_Date(timesatmp);
	sprintf(err_message, "[ERROR] %s: %s\r\n", timesatmp, message);
	printmsg(err_message);
}



