all: clean Q1 Q2 Q1b SudukuCheckerExec

Q1:
	gcc -o Q1 Q1.c SudukuChecker.c
Q2:
	gcc -o Q2 Q2.c SudukuChecker.c
Q1b:
	gcc -o Q1b Q1b.c -lrt SudukuChecker.c

SudukuCheckerExec: 
	gcc -o SudukuCheckerExec SudukuCheckerExec.c SudukuChecker.c

clean:
	rm -f Q1 Q2 Q1b SudukuCheckerExec