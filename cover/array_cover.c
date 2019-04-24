//Brie Moore
//January 17, 2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//functions
void print_matrix(int* arr, int k); 

int main(int argc, char *argv[])
{
    //check to make sure command line arguments are present
    if(argc < 3) {
	printf("missing command line arguments\n");
	return 1;
    }

    int k;  //dimension 
    int i;  //index of matrix 
    int id; //value to place in array

    //take arguments off of command line and covert to ints     
    k = atoi(argv[1]);
    i = atoi(argv[2]);
    id = atoi(argv[3]); 

    //check to make sure that dimension is not negative or 0
    if(k <= 0) {
	printf("please enter a positive, nonzero dimension\n");
	return 1;
    }

    //check to make sure that i is in the bounds of the matrix
    if(i < 0 || i > (k * k - 1)) {
        printf("please enter a dimension in the bounds of [0, k*k)\n");
        return 1;
    }
  
    int t; //index variable
    int rc_count = k; //row count of matrix
    
    int l_bounds[rc_count]; //array to hold lefthand bounds
    
    //initializing array with lefthand bounds
    for(t = 0; t < rc_count; t++) {
        l_bounds[t] = t * rc_count;
    }

    int r_bounds[rc_count]; //array to hold righthand bounds

    //initializing array with righthand bounds
    for(t = 0; t < rc_count; t++) {
        r_bounds[t] = t * rc_count + (rc_count - 1);
    } 

    //allocating memory for array
    int * arr = (int *) malloc(k * k * sizeof(int)); 

    //setting all indexes to 0
    memset(arr, 0, k * k * sizeof(int));
    
    //setting integer i index to value of id
    arr[i] = id;
    
    //printing the array as a matrix through print_matrix
    print_matrix(arr, k);
    printf("______________\n"); 
    
    int flag = 0; //flag to recognize whether i is a bound or not
    
    //loops through bounds arrays to see if i is a right bound and turns flag on if it is
    for(t = 0; t < rc_count; t++) {
        if(r_bounds[t] == i) {
            flag = 1;
        }
    } 
    
    //loops through bounds arrays to see if i is a left bound and turns flag on if it is
    for(t = 0; t < rc_count; t++) {
        if(l_bounds[t] == i) {
            flag = 2;
        }
    } 
    
    //all code below fills the row where the starting index is
    
    int index = i;  //sets index of loop to i
    
    int flag2 = 0;  //flag to signify bound
    
    //fills the matrix from index to right bound if i is a left bound
    if(flag == 2) {
        do{
            //condition to check flag2 to make sure it hasn't reached a right bound
            for(t = 0; t < rc_count; t++) {
                if(r_bounds[t] == index) {
                    flag2 = 1;
                }
            }
            
            //if element is id, increment index and continue with next iteration
            if(*(arr + index) == id) {
                 index++;
                 continue;
            }
            
            //if element is not id, make sure one of the adjacent cells has the id, then change element
            if(*((arr + index) - 1) == id || *((arr + index) + k) == id || *((arr + index) - k) == id) {
                *(arr + index) = id;
            }
            
            //print matrix upon element change
            print_matrix(arr, k);
            
            //increment index
            index++;
            
        } while(flag2 != 1); //continue while program hasn't reached bound
    }
    
    int flag3 = 0; //flag to signify bound
    
    //fills the matrix from index to left bound, if i is a right bound
    if(flag == 1) {
        do{
            //condition to check flag3 to make sure it hasn't reached a left bound
            for(t = 0; t < rc_count; t++) {
                if(l_bounds[t] == index) {
                    flag3 = 1;
                }
            }
            
            //if element is id, decrement index and continue with next iteration
            if(*(arr + index) == id) {
                 index--;
                 continue;
            }
            
            //if element is not id, make sure one of the adjacent cells has the id, then change element
            if(*((arr + index) + 1) == id || *((arr + index) + k) == id || *((arr + index) - k) == id) {
                *(arr + index) = id;
            }
            
            //print matrix upon element change
            print_matrix(arr, k);
            
            //decrement index
            index--;
            
        } while(flag3 != 1); //continue while program hasn't reached bound
    }
    
    int flag4 = 0; //flag to signify bound
    int flag5 = 0; //flag to signify bound
    
    //fills the matrix from index to left bound, then from index to
    //right bound, if i is not a right or left bound
    if(flag == 0) {
        //fill to right bound
        do{
            //condition to check flag4 to make sure it hasn't reached a right bound
            for(t = 0; t < rc_count; t++) {
                if(r_bounds[t] == index) {
                    flag4 = 1;
                }
            }
            
            //if element is id, increment index and continue with next iteration
            if(*(arr + index) == id) {
                 index++;
                 continue;
            }
            
            //if element is not id, make sure one of the adjacent cells has the id, then change element
            if(*((arr + index) - 1) == id || *((arr + index) + k) == id || *((arr + index) - k) == id) {
                *(arr + index) = id;
            }
            
            //print matrix upon element change
            print_matrix(arr, k);
            
            //increment index
            index++;
            
        } while(flag4 != 1); //continue while program hasn't reached bound
        
        index = i; //sets index of loop to i
        
        //fill to left bound
        do{
            //condition to check flag3 to make sure it hasn't reached a left bound
            for(t = 0; t < rc_count; t++) {
                if(l_bounds[t] == index) {
                    flag5 = 1;
                }
            }
            
            //if element is id, decrement index and continue with next iteration
            if(*(arr + index) == id) {
                 index--;
                 continue;
            }
            
            //if element is not id, make sure one of the adjacent cells has the id, then change element
            if(*((arr + index) + 1) == id || *((arr + index) + k) == id || *((arr + index) - k) == id) {
                *(arr + index) = id;
            }
            
            //print matrix upon element change
            print_matrix(arr, k);
            
            //decrement index
            index--;
            
        } while(flag5 != 1); //continue while program hasn't reached bound
        
    }
    //end of fill row code
    
     index = i; //set index back to i
    
     //fill rest of array forward because now all fill should be adjacent above and below row
     for(; index < (k * k); index++) {
         
          //if element is id, decrement index and continue with next iteration
          if(*(arr + index) == id) {
            continue;
          }
          
          //if element is not id, make sure one of the adjacent cells has the id, then change element
          if(*((arr + index) + 1) == id || *((arr + index) - 1) == id || *((arr + index) + k) == id || *((arr + index) - k) == id) {
            *(arr + index) = id;
          }
          
          //print matrix upon element change
          print_matrix(arr, k); 
        
     }
     
     index = i; //set index back to i
     
     //fill rest of array backwards
     for(; index > -1; index--) {
         
         //if element is id, decrement index and continue with next iteration
         if(*(arr + index) == id) {
            continue;
          }
          
          //if element is not id, make sure one of the adjacent cells has the id, then change element
          if(*((arr + index) + 1) == id || *((arr + index) - 1) == id || *((arr + index) + k) == id || *((arr + index) - k) == id) {
            *(arr + index) = id;
          }
        
          //print matrix upon element change
          print_matrix(arr, k); 
         
     }

     free(arr);

     return 0;
} //end of main

//prints the matrix as a k*k arrray
void print_matrix(int *arr, int k) {
    int m; //index variable
    int n; //index variable
    
    //loops to print array as a matrix
    for(m = 0; m < k; m++) {
        for(n = 0; n < k; n++) {
            printf("[%d]", *(arr + (k * m + n)));
        }
        putchar('\n');
    }
    putchar('\n');
}

//end of file 



