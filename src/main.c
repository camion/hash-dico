#include <stdio.h>
#include "../include/read.h"
#include "../include/util.h"
#include "../include/word.h"

int main(int argc, char* argv[]){
    List *hash_table=init_hash_table();
	parse_text("/tmp/test.txt", hash_table);
	print_list_word(hash_table[0]);
    free_hash(hash_table);
    return 0;
}

