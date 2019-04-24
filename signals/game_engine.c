#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void list_children();
int search_children(int pid);
void update_list();

int child_pids[100];

int main(void) {
	pid_t pid;
	pid_t parent_pid = getpid();
	int index = 0;
	signal(SIGCHLD, update_list);
	
	while(1) {
		int input; 
		printf("1. Spawn Player\n2. Injure Player\n3. Heal Player\n4. List Players\n5. Quit\n");
		scanf("%d", &input);

		switch(input) {

			case 1: 
				if((pid = fork()) == 0) {
					if(execl("./player", "player", NULL) == -1) {
						printf("%s\n", strerror(errno));
					}
				}
                                
				child_pids[index] = pid;

                                index++;			
			
				list_children();

				break;
	
			case 2:
				list_children();
				int c_pid;
				
				printf("Enter a Player Number to Injure:");
				scanf("%d", &c_pid);
				
				if(search_children(c_pid) == 1) {
					kill(c_pid, SIGUSR1);
				} else {
					printf("That player does not exist\n");
					continue;
				}

				sleep(1);

				break;

			case 3:
				list_children();
                                int c_pid2;

                                printf("Enter a Player Number to Heal:");
                                scanf("%d", &c_pid2);

                                if(search_children(c_pid2) == 1) {
                                        kill(c_pid2, SIGUSR2);
                                } else {
                                        printf("That player does not exist\n");
                                        continue;
                                }
				
				sleep(1);

				break;
			case 4:
				list_children();
				break;
	
			case 5:
				signal(SIGQUIT, SIG_IGN);
				kill(-parent_pid, SIGQUIT);
				printf("GAME OVER\n");
				exit(0);
				break;
			default: 
				exit(1);
		}

	}
}

void list_children() {
	int size = sizeof(child_pids) / sizeof(int);

	int i;

	printf("Current Players: ");

	for(i = 0; i < size; i++) {
		if(child_pids[i] != 0) {
			printf("%d ", child_pids[i]);
		} else {
			continue;
		}
	}
	
	putchar('\n');
}

int search_children(int pid) {

	int size = sizeof(child_pids) / sizeof(int);
	int result = 0;

	int i;

	for(i = 0; i < size; i++) {
		if(pid == child_pids[i]) {
			result = 1;
		}
	}
	
	return result;
}

void update_list() {

	int terminated_pid = wait(0);
	int size = sizeof(child_pids) / sizeof(int);
	int elements;
	int i;
	
	for(i = 0; i < size; i++) {
                if(child_pids[i] != 0) {
                        elements++;
                } else {
                        continue;
                }
        }

	int location;

	for(i = 0; i < elements; i++) {
                if(terminated_pid == child_pids[i]) {
                       	location = i;
                }
        }
	
	child_pids[location] = 0;
	
	list_children();
}
