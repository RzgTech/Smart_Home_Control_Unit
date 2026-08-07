/*
 * callbacks.c
 *
 *  Created on: Jul 15, 2026
 *      Author: Vahid
 */
#include "main_app.h"
#include "authentication.h"
#include "command_parser.h"

void buffer_cleaner(uint8_t* buffer);
extern UART_HandleTypeDef huart2;
extern uint8_t recv_data;
cli_t cli = {
		.count = 0,
		.overflow = FALSE,
		.user_auth_stat = USER_NOT_AUTHENTICATED
};


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (cli.overflow)
    {
        if (recv_data == '\r')
        {
            cli.overflow = FALSE;
            cli.count = 0;
            memset(cli.data_buffer, 0, DATA_BUFFER_SIZE);

            log_error("Input too long!");
            printmsg("#: ");
        }

        HAL_UART_Receive_IT(&huart2, &recv_data, 1);
        return;
    }

	if (cli.count < DATA_BUFFER_SIZE - 1)  //check for buffer overflow
	{
		cli.data_buffer[cli.count++] = recv_data;
		if (recv_data == '\r')
		{
			buffer_cleaner(cli.data_buffer);

			if (cli.user_auth_stat == USER_AUTHENTICATED)
			{
				command_parser(cli.data_buffer);
				printmsg("#: ");
			}
			else
			{
				if(authentication(&cli) == USER_AUTHENTICATED)
				{
					log_info("\rAuthentication successful");
					printmsg("#: ");
				}
			}

			cli.count = 0;
			memset(cli.data_buffer, 0, DATA_BUFFER_SIZE);
		}
	}

	else
	{
		 // Buffer is full, start discarding until Enter
		cli.overflow = TRUE;
	}

	HAL_UART_Receive_IT(&huart2, &recv_data, 1);
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





