#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "llist.c"

list_t * str2bin();

int isZero(list_t * a)
{
	while(a != NULL){
		if(a->value){ // if value is 1
			return 0;
		}
		a = a->next;
	}
	return 1;
}

int getlen(list_t * a){
	int i = 1;
	list_t * ptr = a;
	while(ptr->next != NULL){
		++i;
		ptr = ptr->next;
	}
	return i;
}

list_t * getLarger(list_t * a, list_t * b){
	int la = getlen(a);
	int lb = getlen(b);
	if (la >= lb){
		return a;
	}
	else{
		return b;
	}
}

list_t * getSmaller(list_t * a, list_t * b){
	int la = getlen(a);
	int lb = getlen(b);
	if (la >= lb){
		return b;
	}
	else{
		return a;
	}
}

int bin2dec(list_t * a){
	int out;
	int outOld = 0;
	list_t * ptr = a;
	out = ptr->value;
	int i = 1;
	int numToAdd;
	while(ptr->next != NULL){
		ptr = ptr->next;
		numToAdd = pow(2, i);
		out = (ptr->value * numToAdd) + out;
		if(outOld > out){
			return(INT_MAX);
		}
		outOld = out;
		++i;
	}
	return out;
}

list_t * dec2bin(int a){ // only single digit
	int * tmpstr;
	tmpstr = (int *)malloc(4 * sizeof(int));
	tmpstr[0] = 0;
	tmpstr[1] = 0;
	tmpstr[2] = 0;
	tmpstr[3] = 0;
	if(a >= 8){
		tmpstr[0] = 1;
		a-=8;
	}
	if(a >= 4){
		tmpstr[1] = 1;
		a-=4;
	}
	if(a >= 2){
		tmpstr[2] = 1;
		a-=2;
	}
	if(a >= 1){
		tmpstr[3] = 1;
	}
	// convert cstring to list_t

	list_t * outlist;
	outlist = make_node(tmpstr[3]);
	int i;
	for(i = 2; i>=0; --i){
		add_node(outlist, tmpstr[i]);
	}
	free(tmpstr);
	return outlist;
}

list_t * add(list_t * num1, list_t * num2){
	list_t * out;
	list_t * larger = copy_list(getLarger(num1, num2));
	list_t * l_ptr = larger;
	list_t * smaller = copy_list(getSmaller(num1, num2));
	list_t * s_ptr = smaller;

	out = make_node(smaller->value ^ larger->value);
	int carry = (smaller->value & larger->value);
	int value = 0;
	while(s_ptr->next != NULL){
		s_ptr = s_ptr->next;
		l_ptr = l_ptr->next;
		value = ( s_ptr->value + l_ptr->value + carry );
		add_node(out, !(value%2 == 0));

		carry = (value >= 2);
	}
	while( (l_ptr->next != NULL) && carry){ //while this loop runs, carry will always be 1
		l_ptr = l_ptr->next;
		if(l_ptr->value & carry){ //possible improvement: remember current place in list when adding values
			add_node(out, 0);
		}
		else{
			add_node(out, 1);
			carry = 0;
		}
	}
	if(carry){ //above loop ended because larger->next == NULL, not because carry == 1
		add_node(out, 1);
		free_list(larger);
		free_list(smaller);
		return out;
	}
	while (l_ptr->next != NULL){
		l_ptr = l_ptr->next;
		add_node(out, l_ptr->value);
	}

	shorten_list(out);
	free_list(larger);
	free_list(smaller);

	return out;
}


list_t * sub(list_t * num1, list_t * num2){ //num1 - num2

	list_t * out;
	list_t * num1c = copy_list(num1);
	list_t * num2c = copy_list(num2);
	shorten_list(num1c);
	shorten_list(num2c);
	//padding to make the subtraction trick work

	list_t * ptr1 = num1c;
	list_t * ptr2 = num2c;

	while(ptr1->next != NULL && ptr2->next != NULL){
		ptr1 = ptr1->next;
		ptr2 = ptr2->next;
	}
	if(ptr1->next != ptr2->next){ //only false if both are pointing to NULL, where they will have the same length
		if(ptr1->next == NULL){ //in this case, ptr1 is the shorter one, so padding needs to be added untill it is the same length as ptr2
			while(ptr2->next != NULL){
				ptr1 = add_node(ptr1, 0); //optimization: add starts at the end
				ptr2 = ptr2->next;
			}
		}
		else{
			while(ptr1->next != NULL){
				ptr2 = add_node(ptr2, 0);
				ptr1 = ptr1->next;

			}
		}
	}

	//add a 0 in front of both for sub trick
	add_node(ptr1, 0);
	add_node(ptr2, 0);



	list_t * one;
	one = make_node(1);
	list_t * new2;
	invert(num2c);
	new2 = add(num2c, one);
	out = add(new2, num1c);

	free_list(new2);
	free_list(num2c);
	free_list(one);


	list_t * out_ptr = out;
	list_t * num1c_ptr = num1c;
	while((out_ptr != NULL) && (num1c_ptr != NULL))
	{
		out_ptr = out_ptr->next;
		num1c_ptr = num1c_ptr->next;
	}
	free_list(num1c);
	if (out_ptr == NULL) //both will be NULL
	{
		return NULL;
	}
	list_t * ptr = out;
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->value = 0;
	shorten_list(out);
	return out;
}

list_t * mult(list_t * num1, list_t * num2)
{
	shorten_list(num1);
	shorten_list(num2);

	list_t * smaller;
	list_t * temp_smaller = getSmaller(num1, num2);
	smaller = copy_list(temp_smaller);
	list_t * larger;
	if(num2 == temp_smaller){ // get larger and smaller length numbers for speed
		larger = copy_list(num1);
	}
	else{
		larger = copy_list(num2);
	}
	if (isZero(smaller))
	{
		free_list(larger);
		free_list(smaller);
		return(make_node(0));
	}

	list_t * out;
	list_t * new_out;
	list_t * num_to_add;
	list_t * padding = make_node(0);
	list_t * s_ptr = smaller->next; //smaller pointer
	list_t * l_ptr = larger; //larger pointer
	list_t * p_ptr = padding; //padding pointer

	if(smaller->value)
	{
		out = copy_list(larger);
	}
	else
	{
		out = make_node(0);
	}
	while (s_ptr != NULL)
	{
		num_to_add = make_node(l_ptr->value & s_ptr->value);
		l_ptr = l_ptr->next;
		while(l_ptr != NULL)
		{
			add_node(num_to_add, (l_ptr->value & s_ptr->value));
			l_ptr = l_ptr->next;
		}


		p_ptr->next = num_to_add;

		new_out = add(out, padding);
		free_list(out);
		out = new_out;

		free_list(num_to_add);

		p_ptr->next = make_node(0);
		p_ptr = p_ptr->next;

		l_ptr = larger;
		s_ptr = s_ptr->next;
	}
	free_list(padding);
	free_list(larger);
	free_list(smaller);
	return out;

}


list_t * old_mult(list_t * num1, list_t * num2){


	shorten_list(num1);
	shorten_list(num2);

	list_t * smaller;
	list_t * temp_smaller = getSmaller(num1, num2);
	smaller = copy_list(temp_smaller);
	list_t * larger;
	if(num2 == temp_smaller){ // get larger and smaller length numbers for speed
		larger = num1;
	}
	else{
		larger = num2;
	}

	if (isZero(smaller))
	{
		return(make_node(0));
	}

	list_t * out = copy_list(larger);
	list_t * max_int = make_node(1);
	int i;
	for(i = 0; i <= 30; ++i)
	{
		add_node(max_int, 1);
	}
	list_t * new_out;
	list_t * new_smaller;
	int dec2 = bin2dec(smaller);
	if(dec2 == 2147483647){
		new_smaller = sub(smaller, max_int);
		free_list(smaller);
		smaller = new_smaller;
	}
	else //dec2 is less than the max value, so loop only needs to run until it is 0
	{
		free_list(smaller);
		smaller = make_node(0);
	}
	--dec2; //out starts as a copy of larger
	while(dec2 > 0){
		new_out = add(out, larger);
		free_list(out);
		out = new_out;

		--dec2;
		if(!dec2)// if dec2 == 0
		{
			if(!isZero(smaller)) //smaller != 0
			{
				dec2 = bin2dec(smaller);
				if(dec2 == 2147483647)
				{
					new_smaller = sub(smaller, max_int);
					free_list(smaller);
					smaller = new_smaller;
				}
				else //dec2 is less than the max value, so loop only needs to run until it is 0
				{
					free_list(smaller);
					smaller = make_node(0);
				}
			}
		}
	}
	return out;
}

list_t * power(list_t * in1, list_t * in2){ //num1 ^ num2

	list_t * num1 = copy_list(in1);
	list_t * num2 = copy_list(in2);


	if(bin2dec(num2) == 0)
	{
		free_list(num1);
		free_list(num2);
		return (make_node(1));
	}
	list_t * out;
	list_t * new_out;
	list_t * one;
	list_t * new_num2;
	one = make_node(1);
	out = copy_list(num1);
	int dec;
	for(dec = bin2dec(num2); dec > 1; dec = bin2dec(num2)){
		new_num2 = sub(num2, one);
		free_list(num2);
		num2 = new_num2;

		new_out = mult(out, num1);
		free_list(out);
		out = new_out;
	}
	free_list(one);
	free_list(num2);
	free_list(num1);
	return out;
}

list_t * str2bin(char * str)
{
	int length;
	for(length = 0; str[length] != '\0'; ++length);
	int str_index = length;
	int current_int;
	list_t * out = make_node(0);
	list_t * new_out;
	list_t * ten = make_node(0);
	add_node(ten, 1);
	add_node(ten, 0);
	add_node(ten, 1);
	list_t * litr = make_node(0);
	list_t * new_litr;
	list_t * one = make_node(1);
	list_t * current_int_l;
	list_t * place = make_node(1);
	list_t * new_place;
	list_t * mult_l;
	list_t * add_l;

	int i = 0;

	for(str_index = 0; str_index < length; ++str_index)
	{
		current_int = str[length - str_index -1] - '0';

		current_int_l = dec2bin(current_int);

		mult_l = mult(current_int_l, place );
		new_out = add(out, mult_l);//add the current number * 10 ^ whavever place to the output.
		out = new_out;
		new_place = mult(place, ten);
		free(place);
		place = new_place;

		new_litr = add(litr, one);
		free_list(litr);
		litr = new_litr;

		free_list(current_int_l);
		free_list(mult_l);
	}
	free_list(ten);
	return out;
}

void output(list_t * num, FILE * ofp)
{
	fprintf(stderr, "Starting Output\n");
	list_t * num_to_subtract = make_node(1);
	int arr_len = 2;
	int arr_index = 0;
	list_t ** num_to_subtract_arr = (list_t **) malloc(arr_len * sizeof(list_t *));
	num_to_subtract_arr[arr_index] = num_to_subtract;
	++arr_index;

	list_t * in = copy_list(num);
	list_t * new_in; //to do copys
	list_t * tmp;
	list_t * ptr = in;
	list_t * ten = make_node(0);
	add_node(ten, 1);
	add_node(ten, 0);
	add_node(ten, 1);

	num_to_subtract = mult(num_to_subtract, ten);
	num_to_subtract_arr[arr_index] = num_to_subtract;
	tmp = sub(in, num_to_subtract);
	fprintf(stderr, "\n");
	//get maximum number [multiple of 10] that goes into input
	while(tmp != NULL)
	{
		//manage string size
		++arr_index;
		if(arr_index == arr_len)
		{
			arr_len *= 2;
			num_to_subtract_arr = (list_t **) realloc(num_to_subtract_arr, arr_len * sizeof(list_t *));
		}
		//update num_to_subtract
		num_to_subtract = mult(num_to_subtract, ten);
		num_to_subtract_arr[arr_index] = num_to_subtract;


		//update tmp
		free_list(tmp);
		tmp = sub(in, num_to_subtract);

	}

	tmp = sub(in, num_to_subtract_arr[arr_index-1]);

	char out_str[arr_len+1];
	int out_index = 0;
	int current_int = 0; //current int to add to out string


	while(arr_index > 0)
	{
		free_list(num_to_subtract_arr[arr_index]);
		--arr_index;
		tmp = sub(in, num_to_subtract_arr[arr_index]);
		while(tmp != NULL)
		{
			new_in = sub(in, num_to_subtract_arr[arr_index]);
			free_list(in);

			in = new_in;
			tmp = sub(in, num_to_subtract_arr[arr_index]);

			++current_int;

		}

		out_str[out_index] = current_int + '0';
		current_int = 0;
		++out_index;
	}
	free_list(num_to_subtract_arr[arr_index]);
	free(num_to_subtract_arr);

	out_str[out_index] = '\0';
	fprintf(ofp, "%s\n", out_str);
	return;
}

