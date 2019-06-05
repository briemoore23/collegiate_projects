/*
* Brie Moore 4/27/19
*/

#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define MAXADDRLEN 256
#define BUFLEN 128

extern int errno;
int sockfd;

struct result_struct {
	char address[16];
	int sum;
};

void print_error(char *);
void my_handler();

void* sum_runner(void *arg) {
	
	struct result_struct *result = (struct result_struct*) arg;
	int sum = 0;
	
	//ip octets variables
	int ip_0 = 0;
	int ip_1 = 0;
	int ip_2 = 0;
	int ip_3 = 0;

	int success = sscanf(result->address, "%i.%i.%i.%i", &ip_0, &ip_1, &ip_2, &ip_3);

	if(success < 4) {
		printf("did not parse correctly\n");
		exit(-1);
	}

	sum = ip_0 + ip_1 + ip_2 + ip_3;
	
	result->sum = sum; 

	pthread_exit(0);

}

int main(int argc, char *argv[]) {

	//handles ctrl-c end by user	
	if(signal(SIGINT, my_handler) < 0) {
		print_error("error registering signal");
		exit(-1);
	}
	
	struct addrinfo *host_ai;
	struct addrinfo hint;
	int err;
    
	if (argc != 2)
        	print_error("usage: client hostname");
    
	hint.ai_flags = 0;
    	hint.ai_family = AF_INET;
    	hint.ai_socktype = SOCK_STREAM;
    	hint.ai_protocol = 0;
    	hint.ai_addrlen = 0;
    	hint.ai_canonname = NULL;
    	hint.ai_addr = NULL;
    	hint.ai_next = NULL;

	if ((err = getaddrinfo(argv[1], "final", &hint, &host_ai)) != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(err));
		return 0;
	}

	printf("creating socket\n");
	if ((sockfd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("Error creating socket");
	}
	printf("socket created\n");

	printf("attempting Connection\n");
	if (connect(sockfd, host_ai->ai_addr, host_ai->ai_addrlen) != 0) {
		printf("can't connect to %s\n", argv[1]);
		print_error("Error connecting to server");
	}
	printf("connection made...\n"); 
	freeaddrinfo(host_ai);
	
	//create username variable
	char usr_name[8] = "";
	strcpy(usr_name, "bmoore");

	//sending username
	int len;
	len = send(sockfd, &usr_name, 8, 0);
		
	if(len < 8) {
                print_error("error sending username");
		close(sockfd);
		exit(-1);
        }

	//create ip_addr variable
	char ip_addr[16] = "";
		
	//receive ip_address
	int n;

	n = recv(sockfd, &ip_addr, 16, 0);

	if(n < 0) {
		print_error("recv error");
		close(sockfd);
		exit(-1);
	}

	printf("IP Address: %s\n", ip_addr);
	
	struct result_struct arg;
	memset(&arg.address, '\0', sizeof(arg.address));
	strcpy(arg.address, ip_addr);
	
	//Thread ID:
	pthread_t tid;

	//Create attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	if((err = pthread_create(&tid, &attr, sum_runner, &arg)) != 0) {
		printf("error creating thread: %d\n", err);
		close(sockfd);
		exit(-1);
	}		
	
	if((err = pthread_join(tid, NULL)) != 0) {
		printf("error joining thread: %d\n", err);
		close(sockfd);
		exit(-1);
	}

	int sum = arg.sum;

	len = send(sockfd, &sum, 4, 0);

	if(len < 4) {
		print_error("error sending sum");
		close(sockfd);
		exit(-1);
	}
	
	char quote[256] = "";

	n = recv(sockfd, &quote, 256, 0);

	if(n < 256) {
		print_error("error receiving quote");
		close(sockfd);
		exit(-1);
	}

	printf("message from server: %s\n", quote);
	
	close(sockfd); 

	return 0;
}

//signal handler for ctrl-c
void my_handler() {
	printf("closing gracefully\n");
	close(sockfd);
	exit(0);
}

//print error
void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
	exit(1);
}

// END OF FILE
