#ifndef QUESTION_H
#define QUESTION_H
/*============================================================================
 Name        : question.h
 Author      : Kathrin Holzmann
 Version     : Mai 25, 2011
 Project     : client
 =============================================================================
*/

void *question_thread(void *data);

void send_QR();

int getQuestion();
#endif