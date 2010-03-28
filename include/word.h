#ifndef _WORD_
#define _WORD_

#include <stdio.h>
#include <string.h>
#include "../include/list.h"
#define END_OF_WORD(c) ((c==' ' || c=='\t' || c=='\n')?1:0)
#define END_OF_PHRASE(c) ((c=='.' || c=='?' || c=='!')?1:0)

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


int get_word(FILE* stream);
List alloc_cell_word(char* word);
int insert_head_word(List *w, char* word);
void print_list_word(List w);

#endif
