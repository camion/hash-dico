#ifndef _WORD_
#define _WORD_

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
void insert_head_word(List *w, char* word);
void insert_tail_word(List *w, char* word);
void insert_lexico_word(List *w, char* word, long offset);
void merge_list(List w1, List w2);
List create_sorted_list(List hash[]);
List last_cell_word(List w);
List search_word(List hash[],char* word);
void print_sentences_containing(FILE* stream, List hash[], char *word);
void print_list_word(List w);
void free_list_word(List *w);

#endif
