/*
 * authentication.c
 *
 *  Created on: Jul 17, 2026
 *      Author: Vahid
 */
#include "main_app.h"
#include "authentication.h"
const char* username = "admin";
const char* password = "1234";
extern uint8_t system_mode;

user_status authentication(cli_t *cli)
{
	switch (cli->user_auth_stat)
	{
	case USER_NOT_AUTHENTICATED:

	    if (strcmp((char *)(cli->data_buffer), "manual") == 0)
	    {
	    	cli->user_auth_stat = USER_WAITING_AUTH_USERNAME;
	        printmsg("username: ");
	    }
	    else
	    {
	    	log_error("Invalid command!");
	    }
	    break;

	case USER_WAITING_AUTH_USERNAME:

	    if (strcmp((char *)(cli->data_buffer), username) == 0)
	    {
	    	cli->user_auth_stat = USER_WAITING_AUTH_PASSWORD;
	        printmsg("password: ");
	    }
	    else
	    {
	        log_error("Invalid username!");
	        printmsg("username: ");
	    }
	    break;

	case USER_WAITING_AUTH_PASSWORD:

	    if (strcmp((char *)(cli->data_buffer), password) == 0)
	    {
	    	cli->user_auth_stat = USER_AUTHENTICATED;
	    	system_mode = MANUAL;
	    	log_info("system mode changed to manual mode");
	    	system_mode_transition(MANUAL);
	    }
	    else
	    {
	    	cli->user_auth_stat = USER_WAITING_AUTH_USERNAME;
	    	log_error("Invalid password!");
	        printmsg("username: ");
	    }
	    break;

	default:
	    break;
	}

	return cli->user_auth_stat;
}
