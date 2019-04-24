/***************
 * 
 * gladiator.c 
 *
 * Eric McGregor
 ****************/

/****************
* Briana Moore
* February 13, 2018
****************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sem_helper.h"
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>

int check_array_full(int semid, int id, int arena_len);
void check_own_spots(int semid, int id, int arena_len);
int next_index_middle(int gladiator_id, int arena_len, int current_index);
int next_index_top(int gladiator_id, int arena_len, int current_index);
int next_index_right(int gladiator_id, int arena_len, int current_index);
int next_index_left(int gladiator_id, int arena_len, int current_index);
int next_index_bottom(int gladiator_id, int arena_len, int current_index);
int next_index_tl(int gladiator_id, int arena_len, int current_index);
int next_index_bl(int gladiator_id, int arena_len, int current_index);
int next_index_tr(int gladiator_id, int arena_len, int current_index);
int next_index_br(int gladiator_id, int arena_len, int current_index);

int main(int argc, char* argv[]) 
{
	int semid;
        int gladiator_id;
	int arena_len;

        if (argc != 4) {
                printf("usage: %s semid gladiator_id arena_len\n", argv[0]);
                return 0;
        }

        semid = atoi(argv[1]);
        gladiator_id = atoi(argv[2]);
	arena_len = atoi(argv[3]);

	/* Battle */

	//set up for battle
	int t;                        //index variable
  	int rc_count = arena_len;     //row count of matrix

  	int l_bounds[rc_count];       //array to hold lefthand bounds

  	//initializing array with lefthand bounds
  	for (t = 0; t < rc_count; t++){
		l_bounds[t] = t * rc_count;
    	}

	int r_bounds[rc_count];       //array to hold righthand bounds

  	//initializing array with righthand bounds
  	for (t = 0; t < rc_count; t++){
		r_bounds[t] = t * rc_count + (rc_count - 1);
    	}

	int t_bounds[rc_count];	       //array to hold top bounds

  	//initializing array with top bounds
  	for (t = 0; t < rc_count; t++){
		t_bounds[t] = t;
    	}
	
	int b_bounds[rc_count];		//array to hold bottom bounds

  	int num = ((arena_len * arena_len) - 1);

  	//initializing array with top bounds
 	for (t = 0; t < rc_count; t++){
		b_bounds[t] = num;
		num--;
    	}

	//get gladiator start position
	int i; 
	int semValue = 0;
	int start = 0;

	for(i = 0; i < (arena_len * arena_len); i++) {
		if((semValue = semctl(semid, i, GETVAL)) == -1) {
			print_error("cannot get semaphore value", errno);
			return -1;
		}
		
		if(semValue == gladiator_id) {
			start = i;
			break;
		}
	}
	
	int brk = 0;			//leave the while loop
	int brk2 = 0;			//leave the while loop
  	int current_index = start;	//current index
 	int flag = 0;			//type of the current index

	 union semun {
                int val;
                struct semid_ds *buf;
                ushort *array;
        } arg;

        arg.val = gladiator_id;
	
	do{
		//checks to see if index is a bound

		//top bound
    		for (t = 0; t < rc_count; t++) {
          		if (t_bounds[t] == current_index) {
              			flag = 1;
           		}
        	}

      		//bottom bound
    		for (t = 0; t < rc_count; t++) {
      			if (b_bounds[t] == current_index) {
              			flag = 4;
            		}
        	}

      		//right bound
    		for (t = 0; t < rc_count; t++) {
          		if (r_bounds[t] == current_index){
              			flag = 2;
           		}
        	}

		//left bound
    		for (t = 0; t < rc_count; t++) {
          		if (l_bounds[t] == current_index) {
              			flag = 3;
            		}
         	}

    		// left corner
    		if (current_index == l_bounds[0]) {
          		flag = 5;
        	}

    		if (current_index == l_bounds[arena_len - 1]) {
          		flag = 6;
        	}

    		//right corner
    		if (current_index == r_bounds[0]) {
          		flag = 7;
        	}

    		if (current_index == r_bounds[arena_len - 1]) {
          		flag = 8;
        	}
		
		switch(flag) {
			//middle element on inside of bounds
			case 0:
				//if already equal to gladiator_id
                		if((semValue = semctl(semid, current_index, GETVAL)) == -1) {
                                        print_error("cannot get semaphore value", errno);
                                        return -1;
                                }
                                if(semValue == gladiator_id) {
						
					if((semValue = semctl(semid, current_index + 1, GETVAL)) == -1) {
                                        	print_error("cannot get semaphore value", errno);
                                        	return -1;
                                	}
					if(semValue != gladiator_id) {
						if(semctl(semid, current_index + 1, SETVAL, arg)) {
        						print_error("cannot set semaphore value", errno);
							return -1;
        					}
					}
					if((semValue = semctl(semid, current_index - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                        	if(semctl(semid, current_index - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
					}
					if((semValue = semctl(semid, current_index + arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                        	if(semctl(semid, current_index + arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
					}
					if((semValue = semctl(semid, current_index - arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                        	if(semctl(semid, current_index - arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
					}
					if((semValue = semctl(semid, (current_index + arena_len) - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index + arena_len) - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index + arena_len) + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index + arena_len) + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index - arena_len) - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index - arena_len) - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index - arena_len) + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index - arena_len) + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }	
					
					current_index = next_index_middle(gladiator_id, arena_len, current_index); 
				}
				
				break;
			//top element			
			case 1:
				//if already equal to gladiator_id
                                if((semValue = semctl(semid, current_index, GETVAL)) == -1) {
                                        print_error("cannot get semaphore value", errno);
                                        return -1;
                                }

                                if(semValue == gladiator_id) {

                                        if((semValue = semctl(semid, current_index + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                        	if(semctl(semid, current_index + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
					}
                                        if((semValue = semctl(semid, current_index - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index + arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index + arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index + arena_len) - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index + arena_len) - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, (current_index + arena_len) + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index + arena_len) + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					
					current_index = next_index_top(gladiator_id, arena_len, current_index);
				}

				break;
			//right element
			case 2:
				//if already equal to gladiator_id
                                if((semValue = semctl(semid, current_index, GETVAL)) == -1) {
                                        print_error("cannot get semaphore value", errno);
                                        return -1;
                                }

                                if(semValue == gladiator_id) {
                                        
					if((semValue = semctl(semid, current_index - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index - arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index + arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index + arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index - arena_len) - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index - arena_len) - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index + arena_len) - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index + arena_len) - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }

					current_index = next_index_right(gladiator_id, arena_len, current_index);
				}

				break;
			//left bound	
			case 3:
				 //if already equal to gladiator_id
                                if((semValue = semctl(semid, current_index, GETVAL)) == -1) {
                                        print_error("cannot get semaphore value", errno);
                                        return -1;
                                }

                                if(semValue == gladiator_id) {

                                        if((semValue = semctl(semid, current_index + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index - arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index + arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index + arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index + arena_len) + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index + arena_len) + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index - arena_len) + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index - arena_len) + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                	current_index = next_index_left(gladiator_id, arena_len, current_index);
				}

				break;
			//bottom bound				
			case 4:
				//if already equal to gladiator_id
                                if((semValue = semctl(semid, current_index, GETVAL)) == -1) {
                                        print_error("cannot get semaphore value", errno);
                                        return -1;
                                }

                                if(semValue == gladiator_id) {

                                        if((semValue = semctl(semid, current_index + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index - arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index - arena_len) - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index - arena_len) - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, (current_index - arena_len) + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index - arena_len) + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                	current_index = next_index_bottom(gladiator_id, arena_len, current_index);
				}

                                break;
			//top left corner
			case 5:
				//if already equal to gladiator_id
                                if((semValue = semctl(semid, current_index, GETVAL)) == -1) {
                                        print_error("cannot get semaphore value", errno);
                                        return -1;
                                }
                                if(semValue == gladiator_id) {

                                        if((semValue = semctl(semid, current_index + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index + arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index + arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index + arena_len) + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index + arena_len) + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
        				current_index = next_index_tl(gladiator_id, arena_len, current_index);                        
				}
	
                                break;

			//bottom left corner
			case 6:
				//if already equal to gladiator_id
                                if((semValue = semctl(semid, current_index, GETVAL)) == -1) {
                                        print_error("cannot get semaphore value", errno);
                                        return -1;
                                }

                                if(semValue == gladiator_id) {

                                        if((semValue = semctl(semid, current_index + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index - arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index - arena_len) + 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index - arena_len) + 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                	current_index = next_index_bl(gladiator_id, arena_len, current_index);
				}

                                break;
			//top right corner
			case 7:
				//if already equal to gladiator_id
                                if((semValue = semctl(semid, current_index, GETVAL)) == -1) {
                                        print_error("cannot get semaphore value", errno);
                                        return -1;
                                }

                                if(semValue == gladiator_id) {

                                        if((semValue = semctl(semid, current_index - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index + arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index + arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index + arena_len) - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index + arena_len) - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
        				current_index = next_index_tr(gladiator_id, arena_len, current_index);                        
				}
	
                                break;

			//bottom right corner
			case 8:
				//if already equal to gladiator_id
                                if((semValue = semctl(semid, current_index, GETVAL)) == -1) {
                                        print_error("cannot get semaphore value", errno);
                                        return -1;
                                }

                                if(semValue == gladiator_id) {

                                        if((semValue = semctl(semid, current_index - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                        if((semValue = semctl(semid, current_index - arena_len, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, current_index - arena_len, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
					if((semValue = semctl(semid, (current_index - arena_len) - 1, GETVAL)) == -1) {
                                                print_error("cannot get semaphore value", errno);
                                                return -1;
                                        }
                                        if(semValue != gladiator_id) {
                                                if(semctl(semid, (current_index - arena_len) - 1, SETVAL, arg)) {
                                                        print_error("cannot set semaphore value", errno);
                                                        return -1;
                                                }
                                        }
                                	current_index = next_index_br(gladiator_id, arena_len, current_index);
				}

                                break;

			default:
				return -1;
                }

		check_own_spots(semid, gladiator_id, arena_len);

	} while((brk = check_array_full(semid, gladiator_id, arena_len)) != 1);
	
	exit(0);

	return 0;
}
int check_array_full(int semid, int id, int arena_len){
	int i;
	int semValue;

	for(i = 0; i < arena_len * arena_len; i++) {
               if((semValue = semctl(semid, i, GETVAL)) == -1) {
                       print_error("cannot get semaphore value", errno);
                       exit(-1);
                }
		if(semValue != id) {
			return 0;
		}
	}
	
	return 1;
}
void check_own_spots(int semid, int id, int arena_len) {
	int i;
        int semValue;
	int count = 0;

        for(i = 0; i < arena_len * arena_len; i++) {
               if((semValue = semctl(semid, i, GETVAL)) == -1) {
                       print_error("cannot get semaphore value", errno);
                       exit(-1);
                }
                if(semValue != id && semValue != 0) {
                        count++;
                }
        }
	
 
	if(count == arena_len * arena_len) {
		printf("BRIE23M SURRENDERS\n");
		exit(0);
	}
}

int next_index_middle(int gladiator_id, int arena_len, int current_index){
	int random_choice;
	int next_index;

	srand( time(NULL) );
	random_choice = rand() % 8;

	switch(random_choice) {
		case 0:
			next_index = current_index + 1;
			break; 
		case 1:
			next_index = current_index - 1;
			break;
		case 2:
			next_index = current_index + arena_len;
			break;
		case 3:
			next_index = current_index - arena_len;
			break;
		case 4:
			next_index = (current_index + arena_len) + 1;
			break;
		case 5:
			next_index = (current_index + arena_len) - 1;
			break;
		case 6:
			next_index = (current_index - arena_len) + 1;
			break;
		case 7:
			next_index = (current_index - arena_len) - 1;
			break;
		default:
			return -1;
	}
	
	return next_index;
	
}
int next_index_top(int gladiator_id, int arena_len, int current_index){
        int random_choice;
        int next_index;

        srand( time(NULL) );
        random_choice = rand() % 5;

        switch(random_choice) {
                case 0:
                        next_index = current_index + 1;
                        break;
                case 1:
                        next_index = current_index - 1;
                        break;
                case 2:
                        next_index = current_index + arena_len;
                        break;
                case 3:
                        next_index = (current_index + arena_len) + 1;
                        break;
                case 4:
                        next_index = (current_index + arena_len) - 1;
                        break;
                default:
                        return -1;
        }

        return next_index;

}
int next_index_right(int gladiator_id, int arena_len, int current_index){
        int random_choice;
        int next_index;

        srand( time(NULL) );
        random_choice = rand() % 5;

        switch(random_choice) {
                case 0:
			next_index = current_index - 1;
			break;
                case 1:
                        next_index = current_index + arena_len;
                        break;
                case 2:
                        next_index = current_index - arena_len;
                        break;
                case 3:
                        next_index = (current_index + arena_len) - 1;
                        break;
                case 4:
                        next_index = (current_index - arena_len) - 1;
                        break;
                default:
                        return -1;
        }

        return next_index;

}
int next_index_left(int gladiator_id, int arena_len, int current_index){
        int random_choice;
        int next_index;

        srand( time(NULL) );
        random_choice = rand() % 5;

        switch(random_choice) {
                case 0:
                        next_index = current_index + 1;
                        break;
                case 1:
                        next_index = current_index - arena_len;
                        break;
                case 2:
                        next_index = current_index + arena_len;
                        break;
                case 3:
                        next_index = (current_index + arena_len) + 1;
                        break;
                case 4:
                        next_index = (current_index - arena_len) + 1;
                        break;
                default:
                        return -1;
        }

        return next_index;

}
int next_index_bottom(int gladiator_id, int arena_len, int current_index){
        int random_choice;
        int next_index;

        srand( time(NULL) );
        random_choice = rand() % 5;

        switch(random_choice) {
                case 0:
                        next_index = current_index + 1;
                        break;
                case 1:
                        next_index = current_index - 1;
                        break;
                case 2:
                        next_index = current_index - arena_len;
                        break;
                case 3:
                        next_index = (current_index - arena_len) + 1;
                        break;
                case 4:
                        next_index = (current_index - arena_len) - 1;
                        break;
                default:
                        return -1;
        }

        return next_index;

}
int next_index_tl(int gladiator_id, int arena_len, int current_index){
        int random_choice;
        int next_index;

        srand( time(NULL) );
        random_choice = rand() % 3;

        switch(random_choice) {
                case 0:
                        next_index = current_index + 1;
                        break;
                case 1:
                        next_index = current_index + arena_len;
                        break;
                case 2:
                        next_index = (current_index + arena_len) + 1;
                        break;
                default:
                        return -1;
        }

        return next_index;

}
int next_index_bl(int gladiator_id, int arena_len, int current_index){
        int random_choice;
        int next_index;

        srand( time(NULL) );
        random_choice = rand() % 3;

        switch(random_choice) {
                case 0:
                        next_index = current_index + 1;
                        break;
                case 1:
                        next_index = current_index - arena_len;
                        break;
                case 2:
                        next_index = (current_index - arena_len) + 1;
                        break;
                default:
                        return -1;
        }

        return next_index;

}
int next_index_tr(int gladiator_id, int arena_len, int current_index){
        int random_choice;
        int next_index;

        srand( time(NULL) );
        random_choice = rand() % 3;

        switch(random_choice) {
                case 0:
                        next_index = current_index - 1;
                        break;
                case 1:
                        next_index = current_index + arena_len;
                        break;
                case 2:
                        next_index = (current_index + arena_len) - 1;
                        break;
                default:
                        return -1;
        }

        return next_index;

}
int next_index_br(int gladiator_id, int arena_len, int current_index){
        int random_choice;
        int next_index;

        srand( time(NULL) );
        random_choice = rand() % 3;

        switch(random_choice) {
                case 0:
                        next_index = current_index - 1;
                        break;
                case 1:
                        next_index = current_index - arena_len;
                        break;
                case 2:
                        next_index = (current_index - arena_len) - 1;
                        break;
                default:
                        return -1;
        }

        return next_index;
}
//  END OF FILE
