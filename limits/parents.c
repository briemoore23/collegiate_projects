#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {

	pid_t pid;
	int status;

	if((pid = fork()) == 0) {
		struct rlimit resource; 

		//resource.rlim_cur = 10000; 
		resource.rlim_max = 10000; 
		
		if(setrlimit(RLIMIT_NPROC, &resource) < 0) {
			printf("%s\n", strerror(errno));
		}

		char *path = "./child";
		char *cmd = "child";
		char *args = "child 1";

		if(execl(path, cmd, args, NULL) == -1) {
			printf("%s\n", strerror(errno));
			exit(1);
		}
	}
	else {
		if(wait(&status) == -1) {
			printf("%s\n", strerror(errno));
			exit(1);
		}

	}
}
