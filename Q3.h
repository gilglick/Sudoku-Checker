#ifndef Q3_H_
#define Q3_H_

#include "SudukuChecker.h"

//defines final numbers
#define NUMBER_OF_THREADS 27

typedef struct {
	char suduku[SUDUKU_LENGTH];
	char threadsResult[NUMBER_OF_THREADS];
} data_t;

typedef struct {
	char index;
	char sudukuOffset;
	char checkOption;
} task_t;

//functions
void* checkSudukuByThread(void* task);

#endif /* Q3_H_ */
