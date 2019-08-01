#include <sys/unistd.h>
#include "SudukuChecker.h"
#include "ErrorManage.h"

int main(int argc, char* argv[]) {
	char suduku[SUDUKU_LENGTH];
	char result;
	int endOfFile;

	for (;;) {
		errorCheck(endOfFile = read(STDIN_FILENO, suduku, SUDUKU_LENGTH),READ_ERROR);
		if (endOfFile == 0)
			exit(0);
		result = argc > 1 ? sudukuCheck(suduku, *argv[1] - '0') : 0;
		errorCheck(write(STDOUT_FILENO, &result, 1),WRITE_ERROR);
	}
	return EXIT_SUCCESS;
}
