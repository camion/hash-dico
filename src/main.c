#include <stdio.h>
#include "../include/read.h"
#include "../include/util.h"
#include "../include/word.h"

int main(int argc, char* argv[]){
	/* List *hash=init_hash_table(); */
	/* if(!parse_text(argv[1],hash)){ */
	/* 	return 0; */
	/* } */
	/* TEMP */
	/* char chaine[256]; */
	/* List tmpword; */
	/* Listpos tmppos; */
	/* printf("Hash table filled succesfully !\n"); */
	/* printf("Type the word you search :\n"); */
	/* scanf("%s",chaine); */
	/* if((tmpword=search_word(hash[hash_string(chaine)%HASH_TABLE],chaine))!=NULL){ */
	/* 	printf("Search:%s\n",tmpword->value->word); */
	/* 	printf("Appear in (bytes) : "); */
	/* 	for(tmppos=tmpword->value->positions;tmppos!=NULL;tmppos=tmppos->next) */
	/* 		printf("%d ",tmppos->position); */
	/* 	printf("\n"); */
	/* } */
	/* TEMP */
    char s[]="aa";
    char s2[]="aa";
    int cle=hash_string(s);
   int cle2=hash_string(s2);
    printf("%s %d\n",s,cle);
    printf("%s %d\n",s2,cle2);
    return 0;
}

