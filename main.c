#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "operations.c"

// constants
#define EOS	257
#define NUM	258

// prototypes
list_t * expr();
list_t * term();
list_t * expon();
list_t * factor();
void error( char * );
int get_token();
void match( int );


// global communication variables
int current_token;
list_t * current_attribute;
FILE *ifp;
FILE *ofp;

/* bounded memory calculator */
int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr, "Not enough arguments, please specify an input and output file:\n");
		fprintf(stderr, "./my-calc <in> <out>\n");
		return 1;
	}
	else if(argc > 3)
	{
		fprintf(stderr, "Too many arguments, you only need to specify an input and output file:\n");
		fprintf(stderr, "./my-calc <in> <out>\n");
		return 1;
	}
	ifp = fopen(argv[1], "r");
	ofp = fopen(argv[2], "w");

	list_t * value;

	current_token = get_token();
	while ( current_token != EOF ) {
		fprintf(stderr, "---Starting New Problem---\n");
		value = expr();
		output(value, ofp);
		current_token = get_token();
	}
}

/* calculator */

// handles addition
list_t * expr()
{
	list_t * value = term();
	while (1) {
		if ( current_token == '+' ) {
			match( '+' );
			fprintf(stderr, "Starting Addition\n");
			value = add(value, term());
			fprintf(stderr, "Finished\n");
		}
		else if ( current_token == '-' ) {
			match( '-' );
			value = sub(value, term());
			if (value == NULL)
			{
				error("Subtraction encountered a negative number.\n");
			}
		}
		else break;
	}
	return value;
}



// handles multiplication
list_t * term()
{
	list_t * value = expon();
	while (1) {
		if ( current_token == '*' ) {
			match( '*' );
			fprintf(stderr, "Starting Multiplication\n");
			value = mult(value, expon());
			fprintf(stderr, "Finished\n");
		}
		else break;
	}
	return value;
}

//handles exponentiation
list_t * expon()
{
	list_t * value = factor();
	while (1) {
		if ( current_token == '^' ) {
			match( '^' );
			fprintf(stderr, "Starting Exponentiation\n");
			value = power(value, factor());
			fprintf(stderr, "Finished\n");
		}
		else break;
	}
	return value;
}

// handles brackets and numbers
list_t * factor()
{
	list_t * value;

	if ( current_token == '(' ) {
		match( '(' );
		value = expr();
		match( ')' );
		return value;
	}
	else if ( current_token == NUM ) {
		value = current_attribute;
		match( NUM );
		return value;
	}
	else error( "Unexpected token in factor()" );
}

/* match expected token */
void match( int expected_token )
{
	if ( current_token == expected_token ) {
		current_token = get_token();
	}
	else {
		error("Unexpected token in match" );
	}
}


/* get next token */
int get_token()
{
	int c;		// current character from the stream
	char * str = (char *) malloc(2 * sizeof(char));
	int strlen = 2;
	int str_index = 0;

	//file specific stuff


	//fprintf(stderr, "%d", ifp);


	while (1) {

		switch ( c = fgetc(ifp) ) {
		case '+': case '-': case '*': case '^': case '(': case ')':
//#ifdef DEBUG
			//fprintf( stderr, "[OP:%c]", c );
//#endif
			return c;	// return operators and brackets as is
		case ' ': case '\t':
			continue;	// ignore spaces and tabs
		default:
			if ( isdigit(c) ) {
				//fprintf( stderr, "[NUM:%c]", c );
				str[str_index++] = c;
				while ( isdigit( c = fgetc(ifp) )) {
					if(str_index >= strlen)
					{
						strlen *= 2;
						str = (char *) realloc(str, strlen * sizeof(char));
					}
					str[str_index++] = c;
				}
				ungetc( c, ifp );
				str[str_index] = '\0';
//#ifdef DEBUG
				//fprintf( stderr, "[NUM:%s]", str );
//#endif
				current_attribute = str2bin(str);
				free(str);
				return NUM;
			}
			else if ( c == '\n' || c == -1) {
				c = fgetc(ifp);
				if( c == -1)
				{
					free(str);
					return EOF;
				}
				else
				{
					ungetc( c, ifp );
					free(str);
					return EOS;
				}
			}
			else {
				fprintf( stderr, "%d: {%c}", c, c );
				error( "Unknown token" );
			}
		}
	}
}

// error reporting function
void error( char *message )
{
	fprintf( stderr, "Error: %s\n", message );
	exit(1);
}
