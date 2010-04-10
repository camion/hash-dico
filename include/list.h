#ifndef _LIST_
#define _LIST_

#include "../include/list.h"

/* List of positions */
typedef struct cellpos{
	long position;
	struct cellpos* next;
}Cellpos,*Listpos;


Listpos alloc_cell_pos(int value);
void insert_head_pos(Listpos *l, int value);
int count_list_pos(Listpos l);
void print_list_pos(Listpos l);
void free_list_pos(Listpos *l);

#endif
