/*
 * command_parser.c
 *
 *  Created on: Jul 16, 2026
 *      Author: Vahid
 */
const char* commands[] = {"help", "fan", "alarm", "light", "quit"};
#include "main_app.h"

void command_parser(uint8_t* data_buffer)
{
	if (strcmp((char *)data_buffer, "help") == 0)
	{
		printmsg("command is: help\r");

	}
	else if (strcmp((char *)data_buffer, "fan") == 0)
	{
		printmsg("command is: fan\r");

	}
	else if (strcmp((char *)data_buffer, "alarm") == 0)
	{
		printmsg("command is: alarm\r");
	}
	else if (strcmp((char *)data_buffer, "light") == 0)
	{
		printmsg("command is: light\r");
	}
	else if (strcmp((char *)data_buffer, "quit") == 0)
	{
		printmsg("command is: quit\r");
	}
	else
	{
		printmsg("Invalid command\r");
	}
}
