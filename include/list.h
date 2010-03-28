#ifndef _LIST_
#define _LIST_
#include <stdio.h>
#include <stdlib.h>

/* List of positions */
typedef struct celpos{
	int position;
	struct celpos* next;
}Celpos,*Listpos;

#include "../include/list.h"

Listpos alloc_cell(int value);
int insert_head(Listpos *l, int value);
int insert_tail(Listpos *l, int value);
Listpos last_cell(Listpos l);
void print_list(Listpos l);

#endif
