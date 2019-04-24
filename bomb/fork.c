#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {

	int infinite = 0;

	while(infinite == 0) {
		fork();
	}
}
