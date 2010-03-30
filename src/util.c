#include  "../include/util.h"

unsigned int hash_string(char *p){
/* maybe try with (i+1)*word[i] */
	unsigned int h=0,g;
	for(;*p;p++){
		h = (h<<4) + *p;
		if((g=h&0xf0000000)){
			h = h^(g>>24);
			h = h^g;
		}
	}
	return h;
}

List* init_hash_table(){
	List* hash=(List*)calloc(HASH_TABLE,sizeof(Cell));
	if(hash==NULL){
		fprintf(stderr,"Error while allocating hash table\n");
		exit(1);
	}
	return hash;
}

void free_hash(List *hash){
	int i;
	for(i=0;i<HASH_TABLE;i++){
		free_list_word(&hash[i]);
	}
	free(hash);
}

