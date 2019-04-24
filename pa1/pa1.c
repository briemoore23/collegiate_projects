#include <stdio.h>

int htoi(char s[]);
unsigned setbits(unsigned x, int p, int n, unsigned y);
unsigned invert(unsigned x, int p, int n);



int main()
{
    //to test htoi
    int value; 
    //char test[] = "7af3"; 
    char test[] = "0x7af3";
    //char test[] = "F";

    value = htoi(test);
    printf("%d\n", value);

    //to test setbits
    unsigned int value2;
    
    unsigned x =5;
    int p = 2;
    int n = 3; 
    unsigned y = 10;

    value2 = setbits(x, p, n, y);
    printf("%u\n", value2);

    //to test invert       
    unsigned int value3;
    
    unsigned xx = 15;
    int pp =2;
    int nn = 3;

    value3 = invert(xx, pp, nn);
    printf("%u\n", value3);

    return 0;
}

int htoi (char s[]) {
    
    int i, n;

    i = n = 0; 
    
    if(s[i] == '0') {
        ++i;
        if(s[i] == 'x' || s[i] == 'X')
            ++i; 
    }

    int d; 
    
    for(i; s[i] != '\0'; ++i) {
        
        if(s[i] >= '0' &&  s[i] <= '9')
            d = s[i] - '0';
        else if (s[i] >= 'a' &&  s[i] <= 'f')
            d = s[i] - 'a' + 10; 
        else if (s[i] >= 'A' &&  s[i] <= 'F')
            d = s[i] - 'A' + 10; 
        else
            break; 
            
        if (d != '\0')
            n = 16 * n + d; 
    }

    return n;
}

unsigned setbits(unsigned x, int p, int n, unsigned y) {
    int num_shift = p + 1 - n;
    unsigned bin_mask = ~(~0<<n);
    return (x & ~(bin_mask << num_shift)) | ((y & bin_mask) << num_shift);
}

unsigned invert(unsigned x, int p, int n){
     int num_shift = p + 1 - n;
     unsigned bin_mask = ~(~0<<n);
     return x ^ (bin_mask << num_shift);
}

