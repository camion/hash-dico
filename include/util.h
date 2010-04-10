#ifndef _UTIL_
#define _UTIL_

/* We use a big hash size, to make hash function sprinkle better data in hash */
#define HASH_SIZE 2053
#include "../include/word.h"

unsigned int hash_string(char *p);
List* init_hash_table();
void free_hash(List *hash);
int column_count();
void usage(FILE *stream);
void progress_bar(long position, long size);

#endif
