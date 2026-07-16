/*
 * callbacks.c
 *
 *  Created on: Jul 15, 2026
 *      Author: Vahid
 */
#include "main_app.h"

extern UART_HandleTypeDef huart2;
extern uint8_t data_buffer[100];
extern uint8_t recv_data;
extern uint8_t count;
uint8_t data_buffer_len = sizeof(data_buffer)/sizeof(data_buffer[0]);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	data_buffer[count++] = recv_data;
	if (recv_data == '\r')
	{
		command_parser(data_buffer, data_buffer_len);
		count = 0;
		memset(data_buffer, 0, sizeof(data_buffer));
	}

	HAL_UART_Receive_IT(&huart2, &recv_data, 1);
}



