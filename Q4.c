#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "SudukuChecker.h"
#include "ErrorManage.h"
#include "Q4.h"

mission_t mission;
data_t data;
pthread_mutex_t tasksLock, resultsLock;
pthread_cond_t cond;

int main(int argc, const char* argv[]) {
	char suduku[INPUT_LENGTH];
	int fd, i;
	pthread_t threads[NUMBER_OF_THREADS];
	mission.data = data;
	mission.result = 0;
	mission.taskIndex = 0;

	errorCheck((fd = argc > 1 ? open(argv[1], O_RDONLY) : STDIN_FILENO),
	OPEN);
	if (fd == STDIN_FILENO)
		printf("Please enter your suduku answer (81 numbers):\n");

	errorCheck(read(fd, suduku, INPUT_LENGTH), READ);
	charToInt(suduku, mission.data.suduku);

	for (i = 0; i < NUMBER_OF_TASKS / NUMBER_OF_CHECK_OPTIONS; i++) {
		task_t* rowsTask, *colTask, *blockTask;

		rowsTask = malloc(sizeof(task_t));
		colTask = malloc(sizeof(task_t));
		blockTask = malloc(sizeof(task_t));
		if (!rowsTask | !colTask | !blockTask)
			errorCheck(ERROR, MALLOC);

		rowsTask->index = i, colTask->index = i, blockTask->index = i;

		rowsTask->sudukuOffset = i * COLS_NUM;
		rowsTask->checkOption = ROWS_CHECK;
		mission.tasks[i + NUMBER_OF_TASKS / NUMBER_OF_CHECK_OPTIONS] = rowsTask;

		colTask->sudukuOffset = i;
		colTask->checkOption = COLUMNS_CHECK;
		mission.tasks[i] = colTask;

		blockTask->sudukuOffset = ((i / 3) * 27) + ((i % 3) * 3);
		blockTask->checkOption = BLOCK_CHECK;
		mission.tasks[i + 2 * (NUMBER_OF_TASKS / NUMBER_OF_CHECK_OPTIONS)] =
				blockTask;
	}

	errorCheck(pthread_mutex_init(&tasksLock, NULL), MUTEX_INIT);
	errorCheck(pthread_mutex_init(&resultsLock, NULL), MUTEX_INIT);
	errorCheck(pthread_cond_init(&cond, NULL), CONDITION_INIT);

	errorCheck(pthread_mutex_lock(&resultsLock), MUTEX_LOCK);
	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		errorCheck(pthread_create(&threads[i], NULL, checkSudukuByThread, NULL),
		THREAD);
	}
	errorCheck(pthread_cond_wait(&cond, &resultsLock),CONDITION_WAIT);
	errorCheck(pthread_mutex_unlock(&resultsLock), MUTEX_UNLOCK);

	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		errorCheck(pthread_join(threads[i], NULL), JOIN);
	}

	printf(
			mission.result == NUMBER_OF_TASKS ?
					"the suduku is legal\n" : "the suduku isn't legal\n");

	return EXIT_SUCCESS;
}

void* checkSudukuByThread(void* args) {
	int result,index;
	task_t* task;
	while (1) {
		errorCheck(pthread_mutex_lock(&tasksLock), MUTEX_LOCK);
		index = mission.taskIndex;
		if (mission.taskIndex >= NUMBER_OF_TASKS) {
			errorCheck(pthread_mutex_unlock(&tasksLock), MUTEX_UNLOCK);
			break;
		}
		task = mission.tasks[mission.taskIndex++];
		errorCheck(pthread_mutex_unlock(&tasksLock), MUTEX_UNLOCK);
		result = solveTask(task);
		errorCheck(pthread_mutex_lock(&resultsLock), MUTEX_LOCK);
		mission.result += result;
		errorCheck(pthread_mutex_unlock(&resultsLock), MUTEX_UNLOCK);
		if(!result || ++index >= NUMBER_OF_TASKS){
			errorCheck(pthread_cond_signal(&cond), CONDITION_SIGNAL);
			break;
		}
	}
	return NULL;
}

int solveTask(task_t* args) {
	int result;
	result = sudukuPartCheck(mission.data.suduku + args->sudukuOffset,
			args->checkOption);
	free(args);
	return result;
}
