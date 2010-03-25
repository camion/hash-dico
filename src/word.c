#include "../include/word.h"

int get_word(FILE* stream){
	char c=0;
	int index=0;
	char word[50];
	while((c=fgetc(stream)) && c!=EOF && c!=' ' && c!= '!' && c!='?' && c!='\n' && c!='\t'){
		word[index]=c;
		index++;
	}
	if(index!=0){
		word[index]='\0';
		printf("%s\n",word);
	}
	if(c==EOF)
		return 0;
	return 1;
}
