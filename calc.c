#include <stdio.h>
#include <stdlib.h>




int expr();
int term();
int factor();
void error( char * );
int get_token();
void match( int );


int current_token;
int current_attribute;

int main()
{
		int value;

		current_token = get_token();
		while (current_token != EOS ) {
			value = expr();
			fprintf( stderr, "\nValue = %d\n", value);
		}
}



int expr()
{
		int value = term();
		while (1) {
			if (current_token == '+'){
