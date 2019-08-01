#ifndef Q2_H_
#define Q2_H_

#include "SudukuChecker.h"

//defines final numbers
#define NUMBER_OF_CHILDS 3

//defines final messages
#define MMAP "Mmap didn't succeed!"
#define MUNMAP "Munmap didn't succeed!"

typedef struct {
	char suduku[SUDUKU_LENGTH];
	char childsResult[NUMBER_OF_CHILDS];
	char finish[NUMBER_OF_CHILDS];
	char parentRead;
} mmap_t;

#endif /* Q2_H_ */
