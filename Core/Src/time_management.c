
#include "time_management.h"

extern RTC_HandleTypeDef hrtc;

void RTC_Set_Time_Date(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	sTime.Hours = hours;
	sTime.Minutes = minutes;
	sTime.Seconds = seconds;
	sTime.SubSeconds = 0;
	sTime.TimeFormat = RTC_HOURFORMAT_24;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	sDate.Month = month;
	sDate.Date = day;
	sDate.Year = year - 2000; // Assuming year is in 2000s

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}
}

char *RTC_Get_Time_Date(char *buffer, uint8_t buffer_len)
{
	RTC_TimeTypeDef currTime = {0};
	RTC_DateTypeDef currDate = {0};

	if (HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	if (HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	snprintf(buffer,
			buffer_len,
			"%02d/%02d/%04d - %02d:%02d:%02d",
			currDate.Date,
			currDate.Month,
			2000 + currDate.Year,
			currTime.Hours,
			currTime.Minutes,
			currTime.Seconds);

	return buffer;
}