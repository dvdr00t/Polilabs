// gcc -m32 -fno-stack-protector -z execstack -o crackme1 crackme1.c

#include <stdio.h>

void func(int a, int b, int c){

	char buffer[128];
	int response = 0;

	gets(buffer);
	if(response ==42)
		printf("This is the answer!\n");
	else
		printf("…you don't know the answer!\n");

/* qualcosa con a,b,s*/

	return;
}

int main(){
	printf("Please insert a string: ");
	func(1,2,3);
}
