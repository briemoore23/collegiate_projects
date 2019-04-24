#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(void) {

	pid_t pid;
	struct rlimit resource;
	
	if(getrlimit(RLIMIT_NPROC, &resource) < 0) {
		printf("error: %s\n", strerror(errno));
	}

	printf("Parent before setrlimit: rlim_cur (%lld), rlim_max (%lld)\n", resource.rlim_cur, resource.rlim_max);	

	resource.rlim_cur = 7; 
	
	if(setrlimit(RLIMIT_NPROC, &resource) < 0) {
		printf("error: %s\n", strerror(errno));
	}

	printf("Parent after setrlimit: rlim_cur (%lld), rlim_max (%lld)\n", resource.rlim_cur, resource.rlim_max);

	int counter = 0;
	char buff[8];

	while(1) {
		snprintf(buff, sizeof(buff), "%d", ++counter);

		printf("Parent: creating child (%d)\n", counter);

		if((pid = fork()) == 0) {

			printf("Child %d: before execl\n", counter);
			execl("./child", "child", buff, NULL);			

		} else if(pid > 0) {

			printf("Parent: child (%d) successfully created with pid: (%lld)\n", counter, pid);
		
		} else if(pid == -1) {

			printf("Parent: error creating child:%s\n", strerror(errno));
			sleep(2);
			exit(0);
		
		} 
	}

	return 0;
}
