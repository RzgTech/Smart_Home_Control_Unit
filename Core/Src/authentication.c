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
	    break;

	case USER_WAITING_AUTH_USERNAME:

	    if (strcmp((char *)(cli->data_buffer), username) == 0)
	    {
	    	cli->user_auth_stat = USER_WAITING_AUTH_PASSWORD;
	        printmsg("password: ");
	    }
	    else
	    {
	        printmsg("Invalid username!\r");
	        printmsg("username: ");
	    }
	    break;

	case USER_WAITING_AUTH_PASSWORD:

	    if (strcmp((char *)(cli->data_buffer), password) == 0)
	    {
	    	cli->user_auth_stat = USER_AUTHENTICATED;
	    	system_mode = MANUAL;
	    	system_mode_transition(MANUAL);
	    }
	    else
	    {
	    	cli->user_auth_stat = USER_WAITING_AUTH_USERNAME;
	        printmsg("Invalid password!\r");
	        printmsg("username: ");
	    }
	    break;

	default:
	    break;
	}

	return cli->user_auth_stat;
}
