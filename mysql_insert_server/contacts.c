/*
* Brie Moore 3/25/19
*/

#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAXADDRLEN 256

extern int errno;

void print_error(char *);
int connect_to_server(char *);
void print_contact(char[116]);

int main(int argc, char *argv[]) {
	int sockfd;

	while(1) {

		char f_name[24] = "";
		char l_name[48] = "";
		char email_addr[36] = "";
		long phone_num;
		int input;
		int flag = 0; 

		printf("1. Insert new record\n2. List Contacts\n3. Exit\n");
		scanf("%d", &input);
		
		int code = 0;
		int len;

		switch(input) {
			case 1:
				flag = 1; 
				printf("Enter first name:\n");
				scanf("%s", &f_name);
				printf("Enter last name:\n");
				scanf("%s", &l_name);
				printf("Enter email address:\n");
				scanf("%s", &email_addr);	
				printf("Enter phone number:\n");
				scanf("%li", &phone_num);
				
				sockfd = connect_to_server(argv[1]);

				//sending information
				len = send(sockfd, &input, 4, 0);
				
				if(len < 0) {
					print_error("error sending data");
					printf("sent %d bytes\n", len);
				}	
		
				len = send(sockfd, &f_name, 24, 0);
				if(len < 0) {
                        		print_error("error sending data");
					printf("sent %d bytes\n", len);
                		}

				len = send(sockfd, &l_name, 48, 0);
                		if(len < 0) {
                        		print_error("error sending data");
					printf("sent %d bytes\n", len);
                		}
		
				len = send(sockfd, &email_addr, 36, 0);
                		if(len < 0) {
                        		print_error("error sending data");
					printf("sent %d bytes\n", len);
                		}

				len = send(sockfd, &phone_num, 8, 0);
                		if(len < 0) {
                        		print_error("error sending data");
					printf("sent %d bytes\n", len);
                		}
		
				//receiving data
				int n = 0;

				n = recv(sockfd, &code, 4, 0);

				if(n < 0) {
					print_error("recv error");
				}

				close(sockfd);
				break;
			case 2: 				
				sockfd = connect_to_server(argv[1]);

				//sending information
				len = send(sockfd, &input, 4, 0);
				
				if(len < 0) {
					print_error("error sending data");
					printf("sent %d bytes\n", len);
				}

				//receiving number of contacts
				int num_contacts;
				n = recv(sockfd, &num_contacts, 4, 0);
			
				if(n < 0) {
					print_error("recv error");
				}
	
				printf("Here are the contacts:\n");

				for(int i = 0; i < num_contacts; i++) {
					char fn[24] = "";
					char ln[48] = "";
					char ea[36] = "";
					long phone = 0;

					n = recv(sockfd, &fn, 24, 0);
					
					if(n < 0){
						print_error("recv error");
					}
					printf("%s ", fn);

					n = recv(sockfd, &ln, 48, 0);
					
					if(n < 0){
						print_error("recv error");
					}

					printf("%s ", ln);

					n = recv(sockfd, &ea, 36, 0);
					
					if(n < 0){
						print_error("recv error");
					}

					printf("%s ", ea);

					n = recv(sockfd, &phone, 8, 0);
					
					if(n < 0){
						print_error("recv error");
					}

					printf("%li\n", phone);
				}

				close(sockfd);
				break;	
				
			case 3:
				printf("Exiting gracefully\n");
				exit(0);
				break;
			default:
				exit(1);
				break;
		}
	
		//printing success or error code
		if(flag == 1) {
			switch(code) {
				case 0:
					printf("Success\n");
					break;
				case 1: 
					printf("Error - First name is null\n");
					break;
				case 2:
					printf("Error - Last name is null\n");
					break;
				case 3: 
					printf("Error - Record with first and last name already exist\n");
					break;
				default:
					exit(1);
			}
		}
	}
    	
	exit(1);
}

int connect_to_server(char* addr){

	struct addrinfo *host_ai;
	struct addrinfo hint;
	int sockfd, err;
    
	hint.ai_flags = 0;
    	hint.ai_family = AF_INET;
    	hint.ai_socktype = SOCK_STREAM;
    	hint.ai_protocol = 0;
    	hint.ai_addrlen = 0;
    	hint.ai_canonname = NULL;
    	hint.ai_addr = NULL;
    	hint.ai_next = NULL;
	
	if ((err = getaddrinfo(addr, "contacts", &hint, &host_ai)) != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(err));
		return 0;
	}

	if ((sockfd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("Error creating socket");
	}

	if (connect(sockfd, host_ai->ai_addr, host_ai->ai_addrlen) != 0) {
		printf("can't connect to %s\n", addr);
		print_error("Error connecting to server");
	}
	
	freeaddrinfo(host_ai);
	return sockfd;
}

void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
	exit(1);
}

// END OF FILE	
