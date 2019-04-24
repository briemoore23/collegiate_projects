#include <stdio.h>
#include <stdlib.h>
int strlen1(char *p);
char *strdup(char *p);
void strcpy1(char *p, char *q);
void strcpy2(char *p, char *q, int n);
int strcmp(char *p, char *q);
void strcat1(char *p, char *q);
int main(){
	//testing of strlen
	printf("Test of strlen\n");
	char *cp = "test";
	printf("Length: %d\n", strlen1(cp));
	/*testing of strcpy*/
	printf("Test of strcpy1\n");
	char *p=strdup("a message");
	char *q=strdup("hi");

	printf("Before: %s\n", p);
	strcpy1(p,q);
	printf("After: %s\n", p);
	
	//Attempt to break strcpy1
	printf("Attempt to break strcpy1\n");
	char *test="hi";
	char *test2="hello";
	strcpy1(test, test2);

	//test strdup
	printf("Testing strdup\n");
	char *x="hello";
	printf("Message: %s\n",x);
	printf("Original Address: %p\n",x);

	char *new=strdup(x);

	printf("New Address and Copied Message: %p %s\n", new,new);


	//test of strcpy2
	printf("Test of strcpy2\n");
	char *dest=strdup("hello");
	char *src=strdup("test");
	printf("Original Message: %s\n",dest);
	printf("The first 3 letters of %s\n",src);
	strcpy2(dest, src, 3);
	printf("New message: %s\n",dest);

	//Test strcmp
	printf("Test strcmp\n");
	printf("%d\n",strcmp(dest, src));

	//to test strcat
	printf("Test strcat\n");
	char *s=strdup("our group is ");
	char *t=strdup("awesome");

	int original_len=strlen1(s);
	
	printf("Message and Length: %s %d\n", s, original_len);

	strcat1(s,t);

	int new_len=strlen1(s);

	printf("Message and Length: %s %d\n",s,new_len);
	printf("We know the string has the null character\n");
	printf("because the strlen method does not count the null character\n");
	printf("so the length is actually 21, but we see 20 here.\n");
		
	return 0;
}
int strlen1(char *p) {
	int x;
	for (x = 0; *p !='\0'; p++)
	x++;
	return x;
}
char *strdup(char *p){
	int len=strlen1(p);
	char *newString;

	newString=malloc(sizeof(char)*len+1);

	if(newString==NULL){
		return((char*)NULL);
	}

	int i=0;
	while(*p){
		newString[i++]=*p;
		p++;
	}
	newString[i]='\0';

	return (newString);
}
void strcpy1(char *p, char *q){
	int plen=strlen1(p);
	int qlen=strlen1(q);

	if((plen<qlen) || plen==0){
		printf("p does not have sufficient space\n");
		return;
	}
	while(*p++=*q++);
}
void strcpy2(char *p, char *q, int n) {
	if((p||q)=='\0'){
		printf("%s\n","Does not have sufficient space");
	}
	int x=0;
	for(;x<=n;x++){
		if(x==n){
			*p='\0';
		}else{
			*p=*q;
			p++;
			q++;
		}
	}
}
void strcat1(char *p, char *q) {
	while(*p !='\0'){
		p++;
	}
	
	while(*p++=*q++);
	
	*p='\0';
}
int strcmp1(char *p, char *q){
	for( ;*p==*q; p++, q++){
		if(*p == '\0'){
			return 0;
		}
	}
	return *p-*q;
}
