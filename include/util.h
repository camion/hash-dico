#ifndef _UTIL_
#define _UTIL_

/* We use a big hash size, to make hash function sprinkle better data in hash it's better to use primary number like 1597, 28657, 514229 */
#define HASH_SIZE 514229
#include "../include/word.h"

extern int verbose;

unsigned int hash_string(char *p);
List* init_hash_table();
void free_hash(List *hash);
int column_count();
int line_count();
char main_menu();
void usage(FILE *stream);
int hash_progress_bar(long position, long size);
long filesize(FILE* f);
void save_index(List index,char* basename);
void sub_main_command(int argc, char *argv[]);
#endif
