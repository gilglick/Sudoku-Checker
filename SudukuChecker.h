#ifndef SUDUKUCHECKER_H_
#define SUDUKUCHECKER_H_

//general suduku defines
#define OFFSET 2
#define SUDUKU_LENGTH 81
#define INPUT_LENGTH SUDUKU_LENGTH*OFFSET
#define ROWS_NUM 9
#define COLS_NUM 9
#define BLOCK_NUM 9
#define BLOCK_SIZE 3

//suduku check defines
#define ROWS_CHECK 0
#define COLUMNS_CHECK 1
#define BLOCK_CHECK 2
#define NUMBER_OF_CHECK_OPTIONS 3
#define CORRECT_RESULT 0x3FE

//pipe defines
#define READ 0
#define WRITE 1

//process defines
#define ERROR -1
#define CHILD 0
#define PARENT default

//defines final messages
#define STDIN_MESSAGE "Please enter your suduku answer (81 numbers):\n"
#define OPEN "Open file descriptor didn't succeed"
#define READ_ERROR "Read didn't succeed"
#define WRITE_ERROR "Write didn't succeed"
#define FD_CLOSE "File descriptor close didn't succeed"
#define FORK "Fork didn't succeed!"
#define THREAD "Thread create didn't succeed!"
#define MALLOC "Malloc didn't succeed!"
#define JOIN "Thread join didn't succeed!"

//functions
int sudukuCheck(const char* suduku, char checkType);
int sudukuPartCheck(const char* sudukuPart, char checkType);
void charToInt(const char* sudukuFromFile, char* suduku);

#endif /* SUDUKUCHECKER_H_ */
