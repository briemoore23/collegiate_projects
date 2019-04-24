#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/time.h>

void decrease_health();
void increase_health();

int health_level = 3;

int main (void) {
	
	signal(SIGUSR1, decrease_health);
	signal(SIGUSR2, increase_health);
	
	while(1) {
		pause();
	}
}

void decrease_health() {
	if(health_level > 1) {
		health_level--;
		printf("HEALTH: %d\n", health_level);
	}
	else if (health_level == 1) {
		health_level--;
		printf("Player killed: %d\n", getpid());
		exit(0);
	}
}

void increase_health() {

	if(health_level < 5) {
		health_level++;
		printf("HEALTH: %d\n", health_level);
	}
	else {
		printf("HEALTH: %d\n", health_level);
	}
}
