#ifndef LIST_H
#define LIST_H

typedef struct list_s{
	bool value;
	struct list_s *next;
}
list_t;

list_t *make_node( bool );
void free_node( list_t *);

void show( list_t *);
list_t * add_node( list_t *, bool );
void free_list(list_t *);
list_t * copy_list(list_t *);
void invert(list_t *);
void shorten_list(list_t *);
void show_debug(list_t *);
bool shorten_list_helper(list_t *);
#endif
