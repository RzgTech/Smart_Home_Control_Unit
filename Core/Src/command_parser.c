/*
 * command_parser.c
 *
 *  Created on: Jul 16, 2026
 *      Author: Vahid
 */
#include "main_app.h"
char* commands[] = {"help", "fan", "alarm", "light", "quit"};


void command_parser(uint8_t* data_buffer, uint8_t data_buffer_size)
{
	if (strcmp((char *)data_buffer, commands[0]) == 0)
	{
		printmsg("command is: help\r");

	}
	else if (strcmp((char *)data_buffer, commands[1]) == 0)
	{
		printmsg("command is: fan\r");

	}
	else if (strcmp((char *)data_buffer, commands[2]) == 0)
	{
		printmsg("command is: alarm\r");
	}
	else if (strcmp((char *)data_buffer, commands[3]) == 0)
	{
		printmsg("command is: light\r");
	}
	else if (strcmp((char *)data_buffer, commands[4]) == 0)
	{
		printmsg("command is: quit\r");
	}
	else
	{
		printmsg("Invalid command\r");
	}
}

void buffer_cleaner(uint8_t* buffer, uint8_t data_buffer_size)
{

	for (int i = 0; i < data_buffer_size; i++)
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
