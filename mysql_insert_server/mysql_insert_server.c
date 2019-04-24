/*
 * server.c
 * derived from Advanced Programming in the UNIX Environment, 3rd Edition
 * 
 * Brie Moore 3/25/19
 */

#include <sys/types.h>	// getaddrinfo()
#include <netdb.h>	// getaddrinfo()
#include <sys/socket.h> // getaddrinfo()
#include <stdio.h>	// printf()
#include <stdlib.h> 	// exit()
#include <unistd.h>	// gethostname()
#include <arpa/inet.h>  // inet_ntop()
#include <string.h>	// strerror()
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include <mysql.h>
#define QLEN 128

#ifndef HOST_NAME_MAX 
	#define HOST_NAME_MAX 256 
#endif

extern int errno;
int host_fd;
int clfd;

void print_ip_addresses(struct addrinfo *ai);
void print_error(char *);
void my_handler();

int main(int argc, char *argv[]) 
{
	signal(SIGINT, my_handler);		//handles ctrl-c end by user

	char *host_name;

    	if (argc == 2) {
		host_name = argv[1];		
	}
	else {
		host_name = malloc(HOST_NAME_MAX);
		memset(host_name, 0, HOST_NAME_MAX);

		if (gethostname(host_name, HOST_NAME_MAX) < 0) {
			print_error("gethostname error");
			return(-1);
		}
	}

	printf("host name: %s\n", host_name);

	struct addrinfo *host_ai;
	struct addrinfo hint;

	hint.ai_flags = 0;
	hint.ai_family = 0;
        hint.ai_socktype = SOCK_STREAM;
        hint.ai_protocol = 0;
        hint.ai_addrlen = 0;
        hint.ai_canonname = NULL;
        hint.ai_addr = NULL;
        hint.ai_next = NULL;

	
	if ((getaddrinfo(host_name, "contacts", &hint, &host_ai)) != 0) { 
		//printf("STRERROR: %s\n", strerror(errno));
		print_error("getaddrinfo error");
        	exit(1);
	}
	
	print_ip_addresses(host_ai);

	if ((host_fd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("unable to create socket"); 
		return(-1);
	}
	printf("socket created [%d]\n", host_fd);
	
        if (bind(host_fd, host_ai->ai_addr, host_ai->ai_addrlen) < 0) {
                print_error("unable to bind to socket");
                exit(1);
        }
	printf("bind returned success\n");
	int option = 1;

	if(setsockopt(host_fd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char*)&option, sizeof(option)) < 0) {
		print_error("failure to set socket");
		close(host_fd);
		exit(1);
	}
 
	freeaddrinfo(host_ai);

        if (listen(host_fd, QLEN) < 0) {
                print_error("listen failed");
                exit(1);
        }
	printf("listen returned success\n");

	int last_client_ip_int = 0;

	struct sockaddr client_sockaddr;
	socklen_t client_sockaddr_len = sizeof(client_sockaddr);
	int token = 0;
	
	for (;;) {

		token = 0;
		printf("waiting for connection ...\n");
        	clfd = accept(host_fd, &client_sockaddr, &client_sockaddr_len);
        	
		if (clfd < 0) {
			print_error("accept error");
			exit(1); 
		}
		printf("accepted connection, socket [%d]\n", clfd);

		if (client_sockaddr.sa_family != AF_INET) {
			printf("Can not onnect with IPv6 addresses\n");
			printf("Sending -1\n");

			int mssg = -1;
			int len = send(clfd, &mssg, 4, 0);
			if (len < 0) {
				print_error("error sending data");
			}
			printf("sent %d bytes\n", len);

			close(clfd);
			continue;
		}

		//receiving the command to add, list or quit
		int buffer;

		int n;
		n = recv(clfd, &buffer, 4, 0);
		
		if(n<0){
			print_error("Error reading from socket");
			exit(1);
		}
		
		//inserting contact
		if(buffer == 1) {

			//receiving the first name from the client
			char fName[24] = "";
			n = recv(clfd, &fName, 24, 0);

			if(n<0){
				print_error("Error reading fName");
			}

			int i;
			int counter = 0;
			
			//checking if first name is null
			for(i=0; i<24; i++){
				if(fName[i]=='\0'){
					counter++;
				}
			}
			
			//sending error code to client	
			if(counter==24){
				token = 1;
				int len = send(clfd, &token, 4, 0);
				if(len<0){
					print_error("Error in sending token");
				}
				continue;
			}

			//receiving last name from client
			char lName[48] = "";
			n = recv(clfd, &lName, 48, 0);

			if(n<0){
				print_error("Error reading lName");
			}	

			counter = 0;

			//checking if last name is null
			for(i=0; i<48; i++){
				if(lName[i]=='\0'){
					counter++;
				}
			}
			
			//sending error code to client
			if(counter==48){
				token = 2;
				int len = send(clfd, &token, 4, 0);
				if(len<0){
					print_error("Error in sending token");
				}
				continue;
			}

			//receiving email from client
			char email[36] = "";
			n = recv(clfd, &email, 36, 0);
			
			if(n<0){
				print_error("Error reading email");
			}

			//receiving the phone from the client
			long phone = 0;
			n = recv(clfd, &phone, 8, 0);

			if(n<0){
				print_error("Error reading phone");
			}
			
			//connecting to database
			MYSQL *con = mysql_init(NULL);
			char *hostname = "rds-mysql-test.cjpohmk8t1d9.us-east-2.rds.amazonaws.com";
			char *username = "briemoore23";
			char *password = "Valles!2314";
			char *schema = "contacts";

			if (con == NULL){
				fprintf(stderr, "%s\n", mysql_error(con));
				exit(1);
			}

			if (mysql_real_connect(con, hostname, username, password, schema, 0, NULL, 0) == NULL){
				fprintf(stderr, "%s\n", mysql_error(con));
				mysql_close(con);
				exit(1);
			}
			
			char query[200];
			sprintf(query, "INSERT INTO Contacts_T VALUES('%s','%s','%s',%li)", fName,
			  lName, email, phone);

			if (mysql_query(con, query)){
				fprintf(stderr, "%s\n", mysql_error(con));
				token = 3;
				int len = send(clfd, &token, 4, 0);
				if(len<0){
					print_error("Error with sending token 0");
				}
				continue;
			}
			
			//sending success code
			token = 0; 
			int len = send(clfd, &token, 4, 0);
			if(len < 0) {
				print_error("Error with sending token 0");
			}

			mysql_close(con);
			close(clfd);
			continue;
		}
	
		//listing contacts
		if(buffer == 2) { 
			
			//connecting to database
			MYSQL *con = mysql_init(NULL);
			char *hostname = "rds-mysql-test.cjpohmk8t1d9.us-east-2.rds.amazonaws.com";
			char *username = "briemoore23";
			char *password = "Valles!2314";
			char *schema = "contacts";

			if (con == NULL){
				fprintf(stderr, "%s\n", mysql_error(con));
				exit(1);
			}

			if (mysql_real_connect(con, hostname, username, password, schema, 0, NULL, 0) == NULL){
				fprintf(stderr, "%s\n", mysql_error(con));
				mysql_close(con);
				exit(1);
			}
			
			if(mysql_query(con, "SELECT * FROM Contacts_T")){
				fprintf(stderr, "%s\n", mysql_error(con));
				mysql_close(con);
				exit(1);
			}
		
			MYSQL_RES *result = mysql_store_result(con);

			if (result == NULL) {
				fprintf(stderr, "%s\n", mysql_error(con));
				mysql_close(con);
				exit(1);
			}

			int num_records = mysql_num_rows(result);
			int num_fields = mysql_num_fields(result);
	
			int len = send(clfd, &num_records, 4, 0);	
			if(len < 0) {
				print_error("Error with sending");
			}
			
			MYSQL_ROW row;
			int counter = 0;

			//preparing data	
			while (counter < num_records) {
				row = mysql_fetch_row(result);
				char fn[24] = "";
				char ln[48] = "";
				char ea[36] = "";
				char phone[20] = "";
				long phone_num;
				char *ptr;

				for(int i = 0; i < num_fields; i++) {
					//strcat(data, row[i]);
					//strcat(data, " ");
					switch(i) {
						case 0:
							strcat(fn, row[i]);
							
							//sending data
							len = send(clfd, &fn, 24, 0);
							if(len < 0){
								print_error("error sending data");
							}
							break;
	
						case 1:
							strcat(ln, row[i]);
								
							//sending data
							len = send(clfd, &ln, 48, 0);
							if(len < 0){
								print_error("error sending data");
							}
							break;
				
						case 2:
							strcat(ea, row[i]);
					
							//sending data
							len = send(clfd, &ea, 36, 0);
							if(len < 0){
								print_error("error sending data");
							}
							break;

						case 3:
							strcat(phone, row[i]);
							phone_num = strtol(phone, &ptr, 10);
					
							//sending data
							len = send(clfd, &phone_num, 8, 0);
							if(len < 0){
								print_error("error sending data");
							}
							break;
						}
					}
				counter++;
			}

			mysql_close(con);
			close(clfd);
			continue;
		}

	}		
		close(clfd);	
		close(host_fd);
}
//prints IP address
void print_ip_addresses(struct addrinfo *host_ai)
{
	struct addrinfo	*next_ai; 
	unsigned long *ipv4_addr;
	unsigned char *ipv6_addr;

	char ip_str[INET6_ADDRSTRLEN];

	//printf("Hello from print_ip_addresses\n");

	for (next_ai = host_ai; next_ai != NULL; next_ai = next_ai->ai_next) {

		memset(ip_str, '\0', INET6_ADDRSTRLEN);

		if (next_ai->ai_addr->sa_family == AF_INET) {
			printf("IPv4 ");
			ipv4_addr  = (unsigned long*) &(((struct sockaddr_in*)next_ai->ai_addr)->sin_addr);
			inet_ntop(AF_INET, ipv4_addr, ip_str, sizeof(ip_str));
		}
		else {
			printf("IPv6 ");
			ipv6_addr = (unsigned char*) &(((struct sockaddr_in6*)next_ai->ai_addr)->sin6_addr);
			inet_ntop(AF_INET6, ipv6_addr, ip_str, sizeof(ip_str));
		}

		printf("IP address: %s\n", ip_str);
	}
}

//prints errno and error message
void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
}

//signal handler for ctrl-c
void my_handler() {
	printf("closing sockets\n");
	close(clfd);
	close(host_fd);
	exit(0);
}
