/*
 * authentication.c
 *
 *  Created on: Jul 17, 2026
 *      Author: Vahid
 */
#include "main_app.h"
#include "authentication.h"
char* username = "admin";
char* password = "1234";
extern user_status user_auth_stat;

user_status authentication(uint8_t* data_buffer)
{
	switch (user_auth_stat)
	{
	case USER_NOT_AUTHENTICATED:

	    if (strcmp((char *)data_buffer, "manual") == 0)
	    {
	        user_auth_stat = USER_WAITING_AUTH_USERNAME;
	        printmsg("username: ");
	    }
	    break;

	case USER_WAITING_AUTH_USERNAME:

	    if (strcmp((char *)data_buffer, username) == 0)
	    {
	        user_auth_stat = USER_WAITING_AUTH_PASSWORD;
	        printmsg("password: ");
	    }
	    else
	    {
	        printmsg("Invalid username!\r");
	        printmsg("username: ");
	    }
	    break;

	case USER_WAITING_AUTH_PASSWORD:

	    if (strcmp((char *)data_buffer, password) == 0)
	    {
	        user_auth_stat = USER_AUTHENTICATED;
	    }
	    else
	    {
	        user_auth_stat = USER_WAITING_AUTH_USERNAME;
	        printmsg("Invalid password!\r");
	        printmsg("username: ");
	    }
	    break;

	default:
	    break;
	}

	return user_auth_stat;
}
