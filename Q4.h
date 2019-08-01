#ifndef Q4_H_
#define Q4_H_

#include "SudukuChecker.h"

//defines final numbers
#define NUMBER_OF_TASKS 27
#define NUMBER_OF_THREADS 7

//defines final messages
#define MUTEX_INIT "Mutex init didn't succeed!"
#define MUTEX_LOCK "Mutex lock didn't succeed!"
#define MUTEX_UNLOCK "Mutex unlock didn't succeed!"
#define CONDITION_INIT "Condition init didn't succeed!"
#define CONDITION_WAIT "Condition wait didn't succeed!"
#define CONDITION_SIGNAL "Condition signal didn't succeed!"

typedef struct {
	char suduku[SUDUKU_LENGTH];
	char threadsResult[NUMBER_OF_THREADS];
} data_t;

typedef struct {
	char index;
	char sudukuOffset;
	char checkOption;
} task_t;

typedef struct {
	int taskIndex;
	task_t* tasks[NUMBER_OF_TASKS];
	data_t data;
	int result;
	int finished;
} mission_t;

//functions
void* checkSudukuByThread(void* task);
int solveTask(task_t* args);

#endif /* Q4_H_ */
