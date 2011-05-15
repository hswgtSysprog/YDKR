/*
 * command.h
 *
 *  Created on: May 10, 2011
 *      Author: kathrin
 */

#ifndef COMMAND_H_
#define COMMAND_H_

void *command_thread(void *data);
void send_login(char* name);
void choose_questions();
void start_game();
void send_answer();
void end_game();
void shutdown_game();

#endif /* COMMAND_H_ */
