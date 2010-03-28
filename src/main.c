#include <stdio.h>
#include "../include/read.h"

int main(int argc, char* argv[]){
	char s[WORD_BUFFER];
	int result;
	FILE* f=fopen(argv[1],"r");
	if(f==NULL){
		fprintf(stderr,"Error opening file\n");
		return 0;
	}
	while((result=get_word(f,s))!=EOF)
		printf("%d:%s\n",result,s);
	fclose(f);
	return 0;
}

