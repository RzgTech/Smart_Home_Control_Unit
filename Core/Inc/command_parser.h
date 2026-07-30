/*
 * command_parser.h
 *
 *  Created on: Jul 30, 2026
 *      Author: Vahid
 */

#ifndef INC_COMMAND_PARSER_H_
#define INC_COMMAND_PARSER_H_

void command_parser(uint8_t* data_buffer);

typedef struct
{
	char command[10];
	char option;
	int value;
}command_syntax_t;

#endif /* INC_COMMAND_PARSER_H_ */
