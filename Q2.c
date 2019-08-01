#include <sys/unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "ErrorManage.h"
#include "SudukuChecker.h"
#include "Q2.h"

int main(int argc, char* argv[]) {

	char sudukuFromInput[INPUT_LENGTH];
	char childResult;
	int i, j, id, result, fd = -1;

	mmap_t* pMmap = mmap(NULL, sizeof(mmap_t), PROT_READ | PROT_WRITE,
	MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(pMmap <= 0)
		errorCheck(ERROR,MMAP);

	for (i = 0; i < NUMBER_OF_CHILDS; i++) {
		if (!(id = fork()))
			break;
	}

	switch (id) {

	case ERROR:
		errorCheck(ERROR, FORK);
		break;

	case CHILD:
		for (;;) {
			while (pMmap->parentRead) {
				if (pMmap->parentRead == -1)
					exit(0);
				pMmap->childsResult[i] = sudukuCheck(pMmap->suduku, i);
				pMmap->finish[i] = 1;
				while (pMmap->parentRead == 1)
					;
			}
		}
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
			charToInt(sudukuFromInput, pMmap->suduku);
			pMmap->parentRead = 1;

			for (j = 0, result = 1; j < NUMBER_OF_CHILDS; j++) {
				while (pMmap->finish[j] == 0)
					;
				result &= pMmap->childsResult[j];
				pMmap->finish[j] = 0;
			}

			memset(pMmap, 0, sizeof(mmap_t));

			printf(
					result ?
							"%s the suduku is legal\n" :
							"%s the suduku isn't legal\n",
					fd == STDIN_FILENO ? "std_in input" : argv[i]);
			if (fd > 0)
				errorCheck(close(fd), FD_CLOSE);
		}
		pMmap->parentRead = -1;

		for (j = 0; j < NUMBER_OF_CHILDS; j++) {
			wait(NULL);
		}
		errorCheck(munmap(pMmap,sizeof(mmap_t)),MUNMAP);
	}

	return EXIT_SUCCESS;
}
