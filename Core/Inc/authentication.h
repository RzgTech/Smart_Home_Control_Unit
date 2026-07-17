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
	USER_IN_AUTHENTICATION,
	USER_NOT_AUTHENTICATED,
	USER_AUTHENTICATED
}user_status;

user_status authentication(uint8_t* data_buffer);

#endif /* INC_AUTHENTICATION_H_ */
