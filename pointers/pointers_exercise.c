// Compile with
// gcc -Wall -Wextra -pedantic strings.c -o strings

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void change_value(int *ptr, int new_value) {
	// print the value of the variable stored at the address in ptr.
	
	printf("change_value original value: %d\n", *ptr);

	// change the value of the variable stored at ptr to new_value.
	
	*ptr = new_value;
	
	// print the value of the variable stored at the address in ptr.
	
	printf("change_value new value: %d\n", *ptr);
}

 
int main() {

	// Declare an integer in a variable named i and intialize it to the value 10.
	
	int i = 10;	
	
	// Print the value stored in the variable i.

	printf("Value at i: %d\n", i);

	// Use the placeholder %p and the unary operator & to print the address of i.
	
	printf("Address of i: %p\n",(void*) &i);

	// Declare a second integer variable named j and initialize it to the value 3.
	// Print the value of j and the address of j.

	int j = 3;

	printf("Value of j: %d\n", j);
	printf("Address of j: %p\n", (void*) &j);


	// Declare a variable named pj that holds the address of the variable j.

	int *pj = &j;
	
	// Print the address stored in the variable pj.

	printf("Address in pj: %p\n", (void*) pj);

	// Declare a variable named k and store the character 'A' in it.
	// Print the value in k and the address of k.

	char k = 'A';

	printf("Value of k: %c\n", k);
	printf("Address of k: %p\n", (void*) &k);

	// Declare a variable named l and store in it the value in j using the dereference
	// operator * and pj.  Print the value in l.

	int l = *pj;

	printf("Value of l: %d\n", l);

	/*****************************************/	

	// Declare an uninitialized array of integers named arr of length 10. 

	int arr[10];

	// Set the first, second, and third elements in the array to 10, 11, and 12, respectively.

	arr[0] = 10;
	arr[1] = 11;
	arr[2] = 12; 

	// Print the address of the first element in the array using the name of the array.
	
	printf("Address of first element of arr: %p\n", (void*) arr);

	// Print the value of the first element in the array by using the dereference operator
	// * and the name of the array.

	printf("Value of first element of arr: %d\n", *arr);

	// Print the addresses of the first, second, and third elements in the array without
	// using [].

	printf("Address of first element: %p\n", (void*) arr);
	printf("Address of second element: %p\n",(void*) (arr + 1));
	printf("Address of third element: %p\n", (void*) (arr + 2));

	// Print the values of the first, second, and third elements in the array without using [] 

	printf("Value of first element: %d\n", *arr);
        printf("Value of second element: %d\n", *(arr + 1));
        printf("Value of third element: %d\n", *(arr + 2));

	// Print all of the elements in arr using an for-loop, and without using []

	int index;

	for(index = 0; index < 10; index++) {
		printf("%d\n", *(arr + index));
	}

	// What do you notice about the elements in arr?

	// arr[0] is shorthand to *arr
	// arr[1] is shortnand to *(arr+1) 
	// arr[2] is shorthand to *(arr+2) 
	
	// Print all of the elements in arr using an for-loop and []

	for(index = 0; index < 10; index++) {
                printf("%d\n",  arr[index]);
        }

	/*****************************************/	

	// Declare an integer named m and initialize it to the value 100. Print its value.
	
	int m = 100;
	printf("Value of m: %d\n", m);

	// Declare a variable named pm and store in it the address of m.

	int *pm = &m;

	// Print the values of m using pm. 

	printf("Value of m using pm: %d\n", *pm);

	// Change the value of m using the dereference operator * and pm.
	
	*pm = 200;

	// Print the value of m.	

	printf("Value of m: %d\n", m);
	
	// Fill in the body of the function named change_value found at the top of this file.
	// Call change_value so that after the function is executed the value of m is 300.

	int new_value = 300;
	change_value(pm, new_value);

	// Print the value of m.

	printf("value of m: %d\n", m);
	
	/*****************************************/	

	// Initialize an array named arr2 using {} with the values 9,8,7,6, and 5 (in that order).

	int arr2[] = {9,8,7,6,5};
	
	// Print the values of arr2 using a for-loop.

	for(index = 0; index < 5; index++) {
                printf("%d\n",  arr2[index]);
        }
	printf("__________________\n");

	/*****************************************/	

	// Declare an array of integers named arr3 on the heap using malloc().
	// malloc() is in <stdlib.h>.  See the man page for details.

	int *arr3;
	
	arr3 = (int *)malloc(sizeof(int) * 10);

	// Print the values in arr3 using a for-loop.

	for(index = 0; index < 10; index++) {
                printf("%d\n",  arr3[index]);
        }
	
	printf("__________________\n");

	// What do you notice about the values in arr3?
	
	//all values are zero currently

	// Intialize the values in arr3 using memset().
	// memset() is found in <string.h>.  See the man page for details.
 
	memset(arr3, 0, sizeof(int) * 10);

	// Print the values in arr3 using a for-loop.
	
	for(index = 0; index < 10; index++) {
                printf("%d\n",  arr3[index]);
        }

	printf("__________________\n");

	// Use [] to change the values of the elements at indices 0,1,and 2 to the value 1.
	
	arr3[0] = 1;
	arr3[1] = 1;
	arr3[2] = 1;

	// Use the dereference operator * to change the values of the elements at 
	// indices 3,4, and 5 to the value 1.
	
	*(arr3 + 3) = 1;
	*(arr3 + 4) = 1;
	*(arr3 + 5) = 1;

	// Print the values of the elements in arr3 using a for-loop.

	 for(index = 0; index < 10; index++) {
                printf("%d\n",  arr3[index]);
        }

	// Free the heap memory used by arr3 using free().
	// free() is found in <stdlib.h>.  See the man page for details.
	
	free(arr3);

	/*****************************************/	

	// Declare an uninitialized 3x3 array of integers named arr4. 
	
	int arr4[3][3];

	// Print the values in the array named arr4 using nested for-loops and [][].
	
	printf("__________________\n");
	for(i = 0; i < 3; i++) {
	   for(j = 0; j < 3; j++) {
		printf("%d%c", arr4[i][j], (j==2) ? '\n' : ' ');
	   }
	}

	// What do you observe about the values stored in arr4?
	
	//whatever was in memory

	// Declare and intialize a 3x3 array of integers named arr5 using {} with the following
	// values: 1,2, and 3 in the first row, 4,5, and 6 in the second, and 7,8, and 9 in the third. 

	int arr5[][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
	
	// Print the values in the array named arr5 using nested for-loops and [][].
	
	printf("__________________\n");
        for(i = 0; i < 3; i++) {
           for(j = 0; j < 3; j++) {
                printf("%d", arr5[i][j]);
           }
	   putchar('\n');
        }

	putchar('\n');
	/*****************************************/	

	// A 2D array is an array of arrays.
	// arr5 is actually of type int**, a pointer to an int* (the first array). To get
	// the address of the first array we need to dereference arr5. 
	// By adding 1 to arr5 and dereferencing, we get the pointer to the second array. 
	// By adding 2 to arr5 and dereferencing, we get the pointer to the third array. 

	// Declare 3 variables named row1, row2, and row3 that hold pointers to each of 
	// the arrays.

	int* row1 = *arr5;
	int* row2 = *(arr5 + 1);
	int* row3 = *(arr5 + 2);

	// Using the variables row1, row2, and row3 and the dereference operator * print
	// the first element in ech array. 

	printf("first element 1st array: %d\n", *row1);
	printf("first element 2nd array: %d\n", *row2);
	printf("first element 3rd array: %d\n", *row3);

	// Using the variable row1 and [], print the elements in the first row.
	/*
	printf("elements in first row: ");
	printf("%d", row1[0]);
	printf("%d", row1[1]);
	printf("%d\n", row1[2]);
	*/

	for(index = 0; index < 3; index++) {
	   printf("%3d", row1[index]);
	}

	putchar('\n');

	// Usign the variable row2 and the dereference operator *, print the elements in the second row.
	/*
	printf("elements in second row: ");
        printf("%d", *row2);
        printf("%d", *(row2 + 1));
        printf("%d\n", *(row2 + 2));
	*/
	for(index = 0; index < 3; index++) {
	   printf("%3d", *(row2 + index));
	}

	putchar('\n');

	// Using the variable arr5 and the dereference operator *, print the elements in the
	// third row.
	/*	
	printf("elements in third row: ");
	printf("%d", *(arr5 + 2));
	printf("%d", *(arr5 + 5));
	printf("%d\n", *(arr5 + 8));
	*/
	for(index = 0; index < 3; index++) {
	   printf("%3d", *(*(arr5 + 2) + index));
	}

	putchar('\n');
	
	/*****************************************/	

	// Declare a variable named nrows and intialize it to the number of rows in arr5 
	// using sizeof and arr5.

	int nrows = sizeof(arr5) / sizeof(*arr5);

	// Print the value in nrows.
	
	printf("nrows: %d\n", nrows);

	// Declare an integer named ncols and intialize it to the number of columns in arr5 
	// using sizeof and arr5.

	int ncols = sizeof(*arr5) / sizeof(**arr5);

	// Print the value in ncols.

	printf("ncols: %d\n", ncols);

	// Print the values in arr5 using nrows and ncols and a nested for-loop.

	for(i = 0; i < nrows; i++) {
	   for(j = 0; j < ncols; j++) {
		printf("%3d", arr5[i][j]);
	   }
	putchar('\n');
	}
	
	printf("___________________\n");
	/*****************************************/	

	// Allocate memory on the heap for an array named arr6 that can hold 9 integers. 
	
	int* arr6;
	arr6 = (int *)malloc(sizeof(int) * 9);

	// Intialize all of the elements in arr6 to the value 0 using memset().

	 memset(arr6, 0, sizeof(int) * 9);	

	// Print the elements in arr6 as a 3x3 array using a nested for-loop, the dereference 
	// operator * and arr6.

	for(i = 0; i < 3; i++) {
	   for(j = 0; j < 3; j++) {
	   	printf("%d", *(arr6 + i * j));
	   }
	printf("\n");
	} 

	return 0;
}

// END OF FILE
