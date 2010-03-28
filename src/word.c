#include "../include/word.h"


/*
	Returns 3 values :
	1	: the word is extracted, everything is OK
	2	: the word extracted is the last word of the phrase
	EOF	: no more word to extract -> End Of File
*/
int get_word(FILE* stream){
	char c=0;
	int index=0;
	char word[256];
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
