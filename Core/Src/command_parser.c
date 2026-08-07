/*
 * command_parser.c
 *
 *  Created on: Jul 16, 2026
 *      Author: Vahid
 */
#include "main_app.h"
#include "fan.h"
#include "light_relay.h"
#include "command_parser.h"
#include "authentication.h"
#include "alarm_light.h"
#include "logger.h"

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

	    if (strcmp(full_command.command, "fan") == 0)
	    {
	    	log_debug("inserted command: fan");
	    	if (full_command.option == 'd')
	    	{
		    	uint8_t duty_cycle = (uint8_t)full_command.value;
		    	if (duty_cycle <= 100 || duty_cycle >= 0)
		    	{
					fan_speed_config(duty_cycle);
		    	}
		    	else
		    	{
		    		log_error("fan duty cycle must be between 0 and 100");
		    	}
	    	}
	    	else
	    	{
	    		log_error("Invalid command: type help");
	    	}

	    }
	    else if (strcmp(full_command.command, "light") == 0)
		{
	    	if (full_command.option == 's')
	    	{
	    		log_debug("inserted command: light");
				uint8_t light_state = (uint8_t)full_command.value;
				if (light_state == RELAY_OFF || light_state == RELAY_ON)
				{
					light_relay_config(light_state);
				}
				else
				{
					log_error("Invalid command: light state can be 0 or 1");
				}
	    	}
	    	else
	    	{
	    		log_error("Invalid command: type help");
	    	}
	    }
	    else if (strcmp(full_command.command, "alarm") == 0 && full_command.option == 's')
		{
	    	if (full_command.option == 's')
	    	{
	    		log_debug("inserted command: alarm");
				uint8_t alarm_state = (uint8_t)full_command.value;
				if (alarm_state == ALARM_LIGHT_OFF || alarm_state == ALARM_LIGHT_ON)
				{
					alarm_light_config(alarm_state);
				}
				else
				{
					log_error("Invalid command: light alarm state can be 0 or 1");
				}
	    	}
	    	else
	    	{
	    		log_error("Invalid command: type help");
	    	}
		}
	    else
	    {
	    	log_error("Invalid command\r");
	    }
	}
	else if (strcmp((char *)data_buffer, "quit") == 0)
	{
		system_mode = AUTOMATIC;
		log_info("system mode changed to auto mode");
		cli.user_auth_stat = USER_WAITING_AUTH_USERNAME;
		system_mode_transition(AUTOMATIC);
		log_debug("command is: quit");
	}
	else if (strcmp((char *)data_buffer, '\n') == 0)
	{
		//
	}
	else
	{
		log_error("Invalid command\r");
	}
}
