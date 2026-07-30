/*
 * command_parser.c
 *
 *  Created on: Jul 16, 2026
 *      Author: Vahid
 */
#include "main_app.h"
#include "fan.h"
#include "command_parser.h"
#include "authentication.h"
char command[10];
command_syntax_t full_command = {0};
extern uint8_t system_mode;
extern cli_t cli;
void command_parser(uint8_t* data_buffer)
{
	if (strcmp((char *)data_buffer, "help") == 0)
	{
		printmsg("fan -d duty_cycle_value: 0 - 100\r");
		printmsg("light -s light_state_value: 0 - 1\r");
		printmsg("alarm -s Value: 0 - 1\r");
		printmsg("quit\r");

	}
	else if (sscanf((char *)data_buffer, "%9s -%c %d",
			full_command.command, &(full_command.option), &(full_command.value)) == 3) //to be changed
	{
		printmsg("inserted command: fan\r");
	    if (strcmp(full_command.command, "fan") == 0)
	    {
	    	uint8_t duty_cycle = (uint8_t)full_command.value;
	    	fan_speed_config(duty_cycle);
	    	printmsg("inserted duty cycle = %d\n", full_command.value);
	    }
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
		system_mode = AUTOMATIC;
		cli.user_auth_stat = USER_WAITING_AUTH_USERNAME;
		system_mode_transition(AUTOMATIC);
		printmsg("command is: quit\r");
	}
	else if (strcmp((char *)data_buffer, '\n') == 0)
	{
		//
	}
	else
	{
		printmsg("Invalid command\r");
	}
}
