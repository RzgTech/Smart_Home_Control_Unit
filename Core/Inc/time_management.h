#include "main_app.h"

#define RTC_DATETIME_STR_LEN   	32
#define RTC_INIT_MARKER 		0xABCD  //arbitrary magic number to understand about if RTC is already initialized

char *RTC_Get_Time_Date(char *buffer, uint8_t buffer_len);
void RTC_Set_Time_Date(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year);
