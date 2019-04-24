/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <string.h>

char* strstr2(const char *, const char *);
int count(const char*, const char*);
void print_matrix(int[3][3]);

int main()
{
    //to test strstr
    const char *haystack = "www.google.com";
    const char *needle = "google"; 
    
    printf("Find google in www.google.com: %s\n", strstr2(haystack, needle));
    
    //to test if string is same
    const char *same1 = "BrianaMoore";
    const char *same2 = "BrianaMoore";
    
    printf("Strings are equivalent: %s\n", strstr2(same1, same2));
    
    //to test NULL
    const char *haystack2 = "Bridgewater College";
    const char *needle2 = "EMU"; 
    
    printf("Strings are not equivalent: %s\n", strstr2(haystack2, needle2));
    
    printf("--------------------------\n");
    
    //to test count
    const char *str1 = "harharharharharhar";
    const char *str2 = "harhar";
    
    printf("Number of instances with overlapping: %d\n", count(str1, str2)); 
    
    const char *str3 = "Briana";
    const char *str4 = "a";
    
    printf("Number of instances with character: %d\n", count(str3, str4));
    
    const char *str5 = "z";
    
    printf("Number of instances letter not found: %d\n", count(str3, str5));
    
     printf("--------------------------\n");
    
    //to test print_matrix
    int arr[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
    int arr2[3][3] = {{3,6,9}, {12,15,18}, {21,24,27}};
    
    printf("Printing matrix of ints:\n");
    print_matrix(arr);
    
    printf("Printing matrix to test null:\n");
    print_matrix(arr2);
    
    return 0;
}

char* strstr2(const char *haystack, const char *needle) {
    
    int i = 0;
    
    while(*haystack) {
        for(i = 0; needle[i]; i++) {
            if(haystack[i] != needle[i])
                break;
        }
        
        if(!needle[i])
            return (char*) haystack;
        haystack++;
    }
    
    return NULL;
}
int count(const char *str1, const char *str2) {
    
    int found = 0;
    const char *result = str1;
    
    while((result = strstr2(result, str2)) != NULL) {
        found++;
        result++;
    }
    
    return found;

}

void print_matrix(int arr[3][3]) {
    
    int i, j;
    
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            printf("%d,", arr[i][j]);
        }
        printf("\n");
    }
    
}









