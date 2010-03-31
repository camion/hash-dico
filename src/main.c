#include <stdio.h>
#include "../include/read.h"
#include "../include/util.h"
#include "../include/word.h"

int main(int argc, char* argv[]){
	List *hash=init_hash_table();
	if(!parse_text(argv[1],hash)){
		free_hash(hash);
		return 2;
	}
	free_hash(hash);
	/* check result with valgrind =) */
	return 0;
}

