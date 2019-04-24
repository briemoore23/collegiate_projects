//Brie Moore and Laura Riffo
//February 4, 2019

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

void print_error(const char*, int);
void clean_up(char*, int);
void concat(char p[], char q[]);

int main(int argc, char* argv[]){
	int tokid = 0;
	char *filepath = "/tmp";
	key_t key;
	
	if((key=ftok(filepath, tokid))==-1){
		print_error("Cannot create token ", errno);
	}

	long bufsz = sysconf(_SC_PAGESIZE);

	int shmid;
	if((shmid=shmget(key, bufsz, IPC_CREAT | 0600))==-1){
		print_error("Cannot create shared memory ", errno);
	}

	char *shm = NULL;
	if((shm=shmat(shmid,0,0))==(void *)-1){
		print_error("Unable to attach to shared memory ", errno);
	}

    //begin chat room code
	printf("\nWelcome to the chat room. You should always check to see if there were previous messages for you by pressing 2 unless told otherwise by the chat room. Thanks, enjoy your chatting.\n");
    
	int input;             //user input variable
	char username[100];    //limit username to 100 chars 
	char user[100];        //limit user to 100 chars
    
    //ask user for name
	printf("\nEnter your username: ");
	fgets(username, 100, stdin);
   	username[strlen(username) - 1] = '\0'; 
    //place username into user
	concat(user, username);
    
	int shmlen = strlen(shm);      //length of memory area
    
    //first check to see if someone has already sent a message or if chat room is empty
	if(shmlen==0){
		printf("\nNo one is in the chat room. Please send a message to connect by pressing 1\n");
	}
	else if(shmlen>0){
		printf("\nSomeone is ready to chat. Press 2 to get message\n");
	}
	
    //infinite loop to communicate in shared memory
	while(1){

	//User selects option
	printf("1. Send Message\n2. Get Message\n3. Quit\n");
	scanf("%d\n", &input);
	shmlen = strlen(shm);
	        
	//if user wants to send message, write to shared memory
	if(input == 1){

	   char cbuf[100];     //limit cbuf to 100 chars
	
        //ask user for message
	    printf("Enter message: \n");
	    fgets(cbuf, 100, stdin);    
	    concat(username, cbuf);    //concantenate user name to message in memory to know who is chatting
	
	    int cbuflen = strlen(username);    //length of cbuf is length of username
	
	    if(shmlen + cbuflen+1<bufsz){
		      memcpy(shm+shmlen, username, cbuflen+1);
		      printf("\nMessage: %s\n", shm);
	     }
	     else{
              //if buffer is full, print buffer full, free memory, and exit program
		      printf("Buffer full\n");
		      clean_up(shm, shmid);
		      exit(0);
	     }

	        memset(username, 0, sizeof(username));
		strcpy(username, user);
		//printf("end of send message");
	 }
     //if user wants to receive message
	 else if(input==2){
		printf("inside 2");
	       //sleep(5);   //sleep for 5 seconds to receive message
		printf("\nMessage: %s\n", shm);     //print message
	 }
     //if user selects 3 or any other number, exit program, and freee memory
	 else {
        	clean_up(shm, shmid);
		exit(0);
	 }
 }//end of while
	
 exit(0);   //exit program if necessary
}

//prints error message with the error from errno
void print_error(const char* str, int code){
	printf("%s: %s\n", str, strerror(code));
	exit(-1);
}

//clean up the shared memory by detaching the pointer
void clean_up(char *shm, int shmid){
	if(shmdt(shm)==-1){
		print_error("Unable to detach shared memory ", errno);
	}
	if(shmctl(shmid, IPC_RMID, 0)==-1){
		print_error("Cannot validate shared memory ", errno);
	}
}

//concatenate two strings together and add a space between them
void concat(char p[], char q[]){
	int c,d;
	c=0;

	while(p[c]!='\0'){
		c++;
	}

	p[c] = ' ';
	c++;

	d=0;

	while(q[d] != '\0'){
		p[c] = q[d];
		d++;
		c++;
	}
	p[c] = ' ';
	c++;
	p[c] = '\0';
}
