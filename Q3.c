#include <sys/unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "ErrorManage.h"
#include "SudukuChecker.h"
#include "Q3.h"

data_t data;

int main(int argc, char * argv[]) {
	char sudukuFromInput[INPUT_LENGTH];
	int i, result, fd = -1;
	pthread_t threads[NUMBER_OF_THREADS];

	errorCheck((fd = argc > 1 ? open(argv[1], O_RDONLY) : STDIN_FILENO), OPEN);
	if (fd == STDIN_FILENO)
		printf("Please enter your suduku answer (81 numbers):\n");

	errorCheck(read(fd, sudukuFromInput, INPUT_LENGTH), READ_ERROR);
	charToInt(sudukuFromInput, data.suduku);

	for (i = 0; i < NUMBER_OF_THREADS / NUMBER_OF_CHECK_OPTIONS; i++) {
		task_t* rowsTask, *colTask, *blockTask;

		rowsTask = malloc(sizeof(task_t));
		colTask = malloc(sizeof(task_t));
		blockTask = malloc(sizeof(task_t));
		if (!rowsTask | !colTask | !blockTask)
			errorCheck(ERROR, MALLOC);

		rowsTask->index = i, colTask->index = i, blockTask->index = i;

		rowsTask->sudukuOffset = i * COLS_NUM;
		rowsTask->checkOption = ROWS_CHECK;

		colTask->sudukuOffset = i;
		colTask->checkOption = COLUMNS_CHECK;

		blockTask->sudukuOffset = ((i / 3) * 27) + ((i % 3) * 3);
		blockTask->checkOption = BLOCK_CHECK;

		errorCheck(
				pthread_create(&threads[i], NULL, checkSudukuByThread,
						(void*) rowsTask), THREAD);
		errorCheck(
				pthread_create(
						&threads[i + NUMBER_OF_THREADS / NUMBER_OF_CHECK_OPTIONS],
						NULL, checkSudukuByThread, (void*) colTask), THREAD);
		errorCheck(
				pthread_create(
						&threads[i
								+ 2
										* (NUMBER_OF_THREADS
												/ NUMBER_OF_CHECK_OPTIONS)],
						NULL, checkSudukuByThread, (void*) blockTask), THREAD);

	}

	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		errorCheck(pthread_join(threads[i], NULL), JOIN);
	}

	for (i = 0, result = 1; i < NUMBER_OF_THREADS; i++) {
		result &= data.threadsResult[i];
	}

	printf(result ? "%s the suduku is legal\n" : "%s the suduku isn't legal\n",
			fd == STDIN_FILENO ? "std_in input" : argv[1]);
	if (fd > 0)
		errorCheck(close(fd), FD_CLOSE);

	return EXIT_SUCCESS;
}

void* checkSudukuByThread(void* task) {
	task_t* pTask = (task_t*) task;

	if (pTask->checkOption != ROWS_CHECK)
		pTask->index +=
				pTask->checkOption == COLUMNS_CHECK ?
						NUMBER_OF_THREADS / NUMBER_OF_CHECK_OPTIONS :
						2 * (NUMBER_OF_THREADS / NUMBER_OF_CHECK_OPTIONS);

	data.threadsResult[pTask->index] = sudukuPartCheck(
			data.suduku + pTask->sudukuOffset, pTask->checkOption);
	free(pTask);
	return NULL;
}
