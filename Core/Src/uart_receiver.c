/*
 * callbacks.c
 *
 *  Created on: Jul 15, 2026
 *      Author: Vahid
 */
#include "main_app.h"
#include "authentication.h"
#include "command_parser.h"
#include "queue.h"

void buffer_cleaner(uint8_t* buffer);
extern UART_HandleTypeDef huart2;
extern uint8_t recv_data;
extern QueueHandle_t rx_uartQueue;
cli_t cli = {
		.count = 0,
		.overflow = FALSE,
		.user_auth_stat = USER_NOT_AUTHENTICATED
};


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if (huart->Instance == USART2)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        uint8_t received_byte = recv_data;

        HAL_UART_Receive_IT(&huart2, &recv_data, 1);

        xQueueSendFromISR(
            rx_uartQueue,
            &received_byte,
            &xHigherPriorityTaskWoken
        );

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

}

void buffer_cleaner(uint8_t* buffer)
{

	for (int i = 0; i < DATA_BUFFER_SIZE; i++)
	{
		if (buffer[i] == '\r' || buffer[i] == '\n')
		{
			buffer[i] = '\0';
			break;
		}
		if (buffer[i] == '\0')
		{
			break;
		}
	}
}





