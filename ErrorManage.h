#ifndef ERRORMANAGE_H_
#define ERRORMANAGE_H_

#include <stdio.h>
#include <stdlib.h>

//defines final numbers
#define ERROR -1

//static method for error management
static void errorCheck(int check, const char* errorMessage) {
	if (check < 0) {
		perror(errorMessage);
		exit(ERROR);
	}
}

#endif /* ERRORMANAGE_H_ */
