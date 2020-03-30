#ifndef LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "llist.h"

list_t *make_node( bool x )
{
	list_t *ptr;

	ptr = (list_t *)malloc(sizeof(list_t));
	assert(ptr != NULL);
	ptr->value = x;
	ptr->next = NULL;

	return ptr;
}

/* destructor*/
void free_node (list_t *ptr){
		ptr->next = NULL;
		//fprintf(stderr, "freeing %d\n", ptr);
		free(ptr);
		ptr = NULL;

}

/* show linked list*/
void show(list_t *start){
		list_t *ptr;

		for (ptr = start; ptr != NULL; ptr = ptr->next){

				fprintf(stderr, "%d", ptr->value);
		}
}

list_t * add_node( list_t * node, bool value){
	list_t * ptr;
	ptr = node;
	while (ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = make_node(value);
	return ptr;
}

void free_list(list_t * head){
	list_t * ptr;
	ptr = head->next;
	if(ptr == NULL){
		//fprintf(stderr, "Freeing %d\n", head);
		free_node(head);
		return;
	}
	free_list(ptr);
	//fprintf(stderr, "freeing %d\n", head);
	free_node(head);
	return;
}

list_t * copy_list(list_t * a){
	list_t * out;
	out = make_node(a->value);
	while(a->next != NULL){
		a = a->next;
		add_node(out, a->value);
	}
	return out;
}

void invert(list_t * a){
	a->value = !(a->value);
	while(a->next != NULL){
		a = a->next;
		a->value = !(a->value);
	}
}

void shorten_list(list_t * a){
	if(a->next != NULL){
		if (shorten_list_helper(a->next)){//a->next is freed
			a->next = NULL;
		}
	}
}

void show_debug(list_t * a)
{
	fprintf(stderr, "value   address      next\n");
	while(a != NULL)
	{
		fprintf(stderr, "%d     %d      %d\n", a->value,a, a->next);
		a = a->next;
	}
}

bool shorten_list_helper(list_t * a){
	if( a->next == NULL){
		if(a->value == 0){
			free_node(a);
			return 1;
		}
		else{
			return 0;
		}
	}
	else {
		int choice = shorten_list_helper(a->next);
		if(choice){
			if(a->value == 0){
				free_node(a);
				return 1;
			}
			a->next = NULL;
		}
		return 0;
	}

}
#endif
/*
int main(){
	list_t * a = make_node(0);
	add_node(a, 0);
	add_node(a, 1);
	add_node(a, 0);
	add_node(a, 0);
	fprintf(stderr, "before:\n");
	show(a);
	shorten_list(a);
	fprintf(stderr, "after:\n");
	show(a);

	return 0;
}
*/
