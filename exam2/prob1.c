#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

void print_error(const char*, int);

void my_handler() {
	exit(0);
}

void main() {
	signal(SIGINT, my_handler);

	int tokid = 0;
	char *filepath = "/tmp";
	key_t key;
	int semid;

	if((key = ftok(filepath, tokid)) == -1) {
		print_error("cannot create token", errno);
	}

	if((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
		print_error("cannot create semaphore", errno);
	}

	union semun {
		int val;
		struct semid_ds *buf;
		ushort *array;
	}arg;

	arg.val = 5;

	if((semctl(semid, 0, SETVAL, arg)) == -1) {
		print_error("error setting semaphore", errno);
	}

	while(1) {
		int semValue = 0;
		
		if((semValue = semctl(semid, 0, GETVAL)) == -1) {
			print_error("error getting sem value", errno);
		}

		if(semValue > 0) {
			struct sembuf op[1];
			op[0].sem_num = 0;
			op[0].sem_op = -1;
			op[0].sem_flg = 0;
			
			if(semop(semid, op, 1) == -1) {
				print_error("cannot perform operation", errno);
			}

			printf("consumed\n");
		}
		
		sleep(1);
	}
}
void print_error(const char *str, int code) {
	printf("%s: %s\n", str, strerror(code));
	exit(-1);
}
