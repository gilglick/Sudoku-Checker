#include <sys/unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "SudukuChecker.h"
#include "ErrorManage.h"
#include "Q1b.h"

int main(int argc, const char* argv[]) {
	int pid, i, j, length, sud_in;
	mqd_t mq_out_id, mq_in_id;
	struct mq_attr in_attr = { 0 }, out_attr = { 0 };
	char result, final_result, suduku[INPUT_LENGTH], buff[SUDUKU_LENGTH];

	for (i = 0; i < NUMBER_OF_CHILDS; i++) {
		if (!(pid = fork())) {
			in_attr.mq_msgsize = SUDUKU_LENGTH;
			out_attr.mq_msgsize = 1;
			in_attr.mq_maxmsg = 10;
			out_attr.mq_maxmsg = 10;
			errorCheck(
					(mq_out_id = mq_open(QUEUE_FILE, O_RDONLY | O_CREAT, 0666,
							&in_attr)), "QUEUE_OPEN_ERR");
			errorCheck(
					(mq_in_id = mq_open(QUEUE_RES_FILE, O_WRONLY | O_CREAT,
							0666, &out_attr)), "QUEUE_OPEN_ERR");
			for (;;) {
				errorCheck(
						(length = mq_receive(mq_out_id, buff, SUDUKU_LENGTH, 0)),
						QUEUE_RECEIVE);
				if (!(*buff))
					break;
				result = sudukuCheck(buff, i);
				errorCheck(mq_send(mq_in_id, &result, 1, 0),
				QUEUE_SEND);
			}
			errorCheck(mq_close(mq_out_id), QUEUE_CLOSE);
			errorCheck(mq_close(mq_in_id), QUEUE_CLOSE);
			exit(0);
		}
		errorCheck(pid, FORK);
	}
	out_attr.mq_msgsize = SUDUKU_LENGTH;
	in_attr.mq_msgsize = 1;
	in_attr.mq_maxmsg = 10;
	out_attr.mq_maxmsg = 10;
	errorCheck(
			(mq_in_id = mq_open(QUEUE_RES_FILE, O_RDONLY | O_CREAT, 0666,
					&in_attr)), QUEUE_OPEN);
	errorCheck(
			(mq_out_id = mq_open(QUEUE_FILE, O_WRONLY | O_CREAT, 0666,
					&out_attr)), QUEUE_OPEN);
	if (argc <= 1) {
		argc = 2;
		sud_in = STDIN_FILENO;
	}
	for (i = 1; i < argc; i++) {
		if (sud_in != STDIN_FILENO)
			errorCheck((sud_in = open(argv[i], O_RDONLY)), OPEN);
		errorCheck(read(sud_in, suduku, INPUT_LENGTH), READ);
		if (sud_in != STDIN_FILENO)
			errorCheck(close(sud_in), CLOSE);
		charToInt(suduku, buff);
		for (j = 0; j < NUMBER_OF_CHILDS; j++)
			errorCheck(mq_send(mq_out_id, buff, SUDUKU_LENGTH, 0),
			QUEUE_SEND);
		for (j = 0, final_result = 1; j < NUMBER_OF_CHILDS; j++) {
			errorCheck(mq_receive(mq_in_id, &result, 1, 0),
			QUEUE_SEND);
			final_result &= result;
		}
		printf(final_result ? "%s is legal\n" : "%s is not legal\n",
				(sud_in != STDIN_FILENO) ? argv[i] : "STD_ID");
	}
	for (i = 0, buff[0] = 0; i < NUMBER_OF_CHILDS; i++)
		errorCheck(mq_send(mq_out_id, buff, 1, 0), QUEUE_SEND);
	errorCheck(mq_unlink(QUEUE_RES_FILE), QUEUE_UNLINK);
	errorCheck(mq_unlink(QUEUE_FILE), QUEUE_UNLINK);
	errorCheck(mq_close(mq_out_id), QUEUE_CLOSE);
	errorCheck(mq_close(mq_in_id), QUEUE_CLOSE);
}
