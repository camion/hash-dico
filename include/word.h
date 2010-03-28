#ifndef _WORD_
#define _WORD_

#include <stdio.h>
#include <string.h>
#include "../include/list.h"

/* A word (string) go with a list of all its positions */
typedef struct cellword{
	char* word;
	Listpos positions;
}Cellword;

/* List of words  */
typedef struct cell{
	Cellword *value;
	struct cell* next;
}Cell,*List;


List alloc_cell_word(char* word);
int insert_head_word(List *w, char* word);
int insert_tail_word(List *w, char* word);
List last_cell_word(List w);
List search_word(List w,char* word);
void print_list_word(List w);

#endif
