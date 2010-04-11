#ifndef _READ_
#define _READ_

#define WORD_BUFFER 256
#define END_OF_WORD(c) ((c==' ' || c==',' || c=='\n' || c=='\t' || c==';' || c=='*' || c=='-' || c=='\''))
#define END_OF_PHRASE(c) ((c=='.' || c=='?' || c=='!'))
#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include "../include/word.h"

int get_word(FILE* stream, char* dest, size_t n);
void parse_text(FILE* text, List *hash);
int search_word_text(FILE* f, char* word);

#endif
