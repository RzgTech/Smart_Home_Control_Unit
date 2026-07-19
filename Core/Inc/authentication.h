/*
 * authentication.h
 *
 *  Created on: Jul 17, 2026
 *      Author: Vahid
 */

#ifndef INC_AUTHENTICATION_H_
#define INC_AUTHENTICATION_H_

typedef enum
{
	USER_WAITING_AUTH_USERNAME,
	USER_WAITING_AUTH_PASSWORD,
	USER_NOT_AUTHENTICATED,
	USER_AUTHENTICATED
}user_status;

typedef struct
{
	uint8_t overflow;
	uint8_t count;
	uint8_t data_buffer[DATA_BUFFER_SIZE];
	user_status user_auth_stat;
}cli_t;


user_status authentication(cli_t *cli);

#endif /* INC_AUTHENTICATION_H_ */
