#ifndef _LIST_
#define _LIST_
#include <stdio.h>
#include <stdlib.h>

/* List of positions */
typedef struct cellpos{
	int position;
	struct cellpos* next;
}Cellpos,*Listpos;

#include "../include/list.h"

Listpos alloc_cell_pos(int value);
int insert_head_pos(Listpos *l, int value);
int insert_tail_pos(Listpos *l, int value);
Listpos last_cell_pos(Listpos l);
void print_list_pos(Listpos l);

#endif
