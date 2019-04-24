#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>

void print_stats();

int main(void) {
	
	int index = 1; 

	char *ptr = malloc(index * sizeof(char));
	
	index++; 

	while(1) {

		ptr = realloc(ptr, index * sizeof(char));
		print_stats();
		index++;
	}

        return 0;
}

void print_stats() {

        struct rlimit resource;

        if(getrlimit(RLIMIT_NPROC, &resource) < 0) {
                printf("%s\n", strerror(errno));
        }

        printf("Current max resident set size: (%lld)\n", (long long int) resource.rlim_cur);
        printf("Max data segment size: (%lld)\n", (long long int) resource.rlim_max);

}
