#include <stdio.h>
#include <stdlib.h>
#include "llist.c"
#include "operations.c"

int getstrlen(char * a){
	int i;
	while(a[i] != '\0'){
		++i;
	}
	return i;
}

char * getword(){
	int length = 3;
	char * out;
	out = (char *)malloc(length * sizeof(char));
	int c = 99;
	int i = 1;
	while( (c = getchar()) >= 48  && c <= 58){
		out[i++] = c;	
		if(i == length){
			out = (char *)realloc(out, length * sizeof(char));
		}
	}
	while( c == ' '){
		c = getchar();
	}
	if(c >= 48 || c <= 48){
		fprintf(stderr, "Input error, no operator\n");
	}
	/* Currently assumes that after a number, the next character that isn't a number or a space is a valid operator*/
	out[0] = c; //stores next operator as first character
	out[i] = '\0';
	return out;
}
list_t * str2bin(char * str){
	list_t * ten;
	ten = dec2bin(10);

	list_t * currentNum;
	list_t * finalNum;
	int length = getstrlen(str);
	int i = 1;
	currentNum = mult( dec2bin(str[i]), power(ten, dec2bin(i - 1)) );
	finalNum = currentNum;
	while(i < length){
		++i;
		currentNum = mult( dec2bin(str[i]), power(ten, dec2bin(i - 1)) );
		finalNum = currentNum;
	}
	return currentNum;
}

			
int main(){
	char * number;
	number = getword();
	char operator = number[0];
	list_t * num1;
	num1 = str2bin(number);
	show(num1);


	return 0;
}
