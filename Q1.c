#include <sys/unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "ErrorManage.h"
#include "SudukuChecker.h"
#include "Q1.h"

int main(int argc, char* argv[]) {

	char suduku[SUDUKU_LENGTH], sudukuFromInput[INPUT_LENGTH];
	char childsResult[NUMBER_OF_CHILDS];
	char* checkOption[NUMBER_OF_CHECK_OPTIONS] = { "0", "1", "2" };
	int i, j, id, result, fd = -1;
	int outPipes[NUMBER_OF_OUT_PIPE] = { 0 },
			inPipes[NUMBER_OF_IN_PIPE] = { 0 };

	errorCheck(pipe(inPipes), OPEN_IN_PIPE);
	for (i = 0; i < NUMBER_OF_CHILDS; i++) {
		errorCheck(pipe(&outPipes[i * 2]), OPEN_OUT_PIPE);
		if ((id = fork()))
			errorCheck(close(outPipes[i * 2 + READ]), CLOSE_OUT_PIPE);
		else
			break;
	}

	switch (id) {

	case ERROR:
		errorCheck(ERROR, FORK);
		break;

	case CHILD:
		errorCheck(close(outPipes[i * 2 + WRITE]), CLOSE_OUT_PIPE);
		errorCheck(close(inPipes[READ]), CLOSE_IN_PIPE);
		if (outPipes[i * 2 + READ] != STDIN_FILENO) {
			errorCheck(dup2(outPipes[i * 2 + READ], STDIN_FILENO), DUP);
			errorCheck(close(outPipes[i * 2 + READ]), CLOSE_OUT_PIPE);
		}
		if (inPipes[WRITE] != STDOUT_FILENO) {
			errorCheck(dup2(inPipes[WRITE], STDOUT_FILENO), DUP);
			errorCheck(close(inPipes[WRITE]), CLOSE_IN_PIPE);
		}
		execlp("./SudukuCheckerExec", "SudukuCheckerExec", checkOption[i],
		NULL);
		errorCheck(ERROR, EXECLP);
		break;

	PARENT:
		if (argc <= 1) {
			fd = STDIN_FILENO;
			argc = 2;
		}
		for (i = 1; i < argc; i++) {
			if (fd == STDIN_FILENO)
				printf(STDIN_MESSAGE);
			else
				fd = open(argv[i], O_RDONLY);

			errorCheck(fd, OPEN);
			errorCheck(read(fd, sudukuFromInput, INPUT_LENGTH), READ_ERROR);
			charToInt(sudukuFromInput, suduku);

			for (j = 0; j < NUMBER_OF_CHILDS; j++)
				errorCheck(
						write(outPipes[j * 2 + WRITE], suduku, SUDUKU_LENGTH),
						WRITE_ERROR);

			for (j = 0, result = 1; j < NUMBER_OF_CHILDS; j++) {
				errorCheck(read(inPipes[READ], &childsResult[j], 1),
				READ_ERROR);
				result &= childsResult[j];
			}
			printf(
					result ?
							"%s the suduku is legal\n" :
							"%s the suduku isn't legal\n",
					fd == STDIN_FILENO ? "std_in input" : argv[i]);
			if (fd > 0)
				errorCheck(close(fd), FD_CLOSE);
		}
	}
	errorCheck(close(inPipes[READ]), CLOSE_IN_PIPE);

	for (j = 0; j < NUMBER_OF_CHILDS; j++) {
		errorCheck(close(outPipes[j * 2 + WRITE]), CLOSE_OUT_PIPE);
	}

	for (j = 0; j < NUMBER_OF_CHILDS; j++) {
		wait(NULL);
	}

	return EXIT_SUCCESS;
}

