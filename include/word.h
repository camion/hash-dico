#ifndef _WORD_
#define _WORD_

#include <stdio.h>
#include "../include/list.h"
#define END_OF_WORD(c) ((c==' ' || c=='\t' || c=='\n')?1:0)
#define END_OF_PHRASE(c) ((c=='.' || c=='?' || c=='!')?1:0)

/* A word (string) go with a list of all its positions */
typedef struct celword{
	char *word;
	Listpos positions;
}Celword;

/* List of words  */
typedef struct cell{
	Celword *value;
	struct cell* next;
}Cell,*List;


int get_word(FILE* stream);

#endif
