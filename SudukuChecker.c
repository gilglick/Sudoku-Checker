#include <fcntl.h>
#include <sys/unistd.h>
#include "SudukuChecker.h"
#include "ErrorManage.h"

//suduku check for process mission (Q1-Q2)
int sudukuCheck(const char* suduku, char checkType) {
	int i, j, k, l;
	unsigned short result;
	switch (checkType) {

	case ROWS_CHECK:
		for (i = 0; i < ROWS_NUM; i++) {
			for (j = 0, result = 0; j < COLS_NUM; j++) {
				result |= 1 << suduku[(i * COLS_NUM) + j];
			}
			if (result != CORRECT_RESULT)
				return 0;
		}
		break;

	case COLUMNS_CHECK:
		for (i = 0; i < COLS_NUM; i++) {
			for (j = 0, result = 0; j < ROWS_NUM; j++) {
				result |= 1 << suduku[(j * ROWS_NUM) + i];
			}
			if (result != CORRECT_RESULT)
				return 0;
		}
		break;

	case BLOCK_CHECK:
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				for (k = 0, result = 0; k < 3; k++) {
					for (l = 0; l < 3; l++) {
						result |= 1 << suduku[i * 27 + j * 3 + k * 9 + l];
					}
				}
				if (result != CORRECT_RESULT)
					return 0;
			}

		}
		break;
	}
	return 1;
}

//suduku check for threads mission (Q3-Q4)
int sudukuPartCheck(const char* sudukuPart, char checkType) {
	int i, j;
	unsigned short result;

	switch (checkType) {
	case ROWS_CHECK:
		for (i = 0, result = 0; i < COLS_NUM; i++) {
			result |= 1 << sudukuPart[i];
		}
		break;

	case COLUMNS_CHECK:
		for (i = 0, result = 0; i < ROWS_NUM; i++) {
			result |= 1 << sudukuPart[i * COLS_NUM];
		}
		break;

	case BLOCK_CHECK:
		for (i = 0, result = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				result |= 1 << sudukuPart[i * COLS_NUM + j];
			}
		}
		break;
	}

	return result == CORRECT_RESULT;
}

void charToInt(const char* sudukuFromInput, char* suduku) {
	int i, j;
	for (i = 0, j = 0; i < INPUT_LENGTH; i += 2, j++) {
		suduku[j] = sudukuFromInput[i] - '0';
	}
}
