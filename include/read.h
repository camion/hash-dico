#ifndef _READ_
#define _READ_

#define WORD_BUFFER 256
#define END_OF_WORD(c) ((c==' ' || c=='\t' || c=='\n')?1:0)
#define END_OF_PHRASE(c) ((c=='.' || c=='?' || c=='!')?1:0)
#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <string.h>
#include "../include/word.h"
#include "../include/util.h"

int get_word(FILE* stream, char* dest);
int parse_text(char* file_name, List *hash);

#endif