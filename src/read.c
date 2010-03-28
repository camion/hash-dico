#include "../include/read.h"


/*
	Extract a word from a stream already opened
	The word is stored in dest
	Returns 3 values :
	1	: the word is extracted, everything is OK
	2	: the word extracted is the last word of the phrase
	EOF	: no more word to extract -> End Of File
*/
int get_word(FILE* stream, char* dest){
	char c;
	int index=0;
	int flag_eow=FALSE;
	char word[WORD_BUFFER];
	while((c=fgetc(stream)) && c!=EOF && !END_OF_PHRASE(c) && index < WORD_BUFFER){
		if(END_OF_WORD(c) && index==0)
			continue;
		if(index>0 && END_OF_WORD(c))
			flag_eow=TRUE;
		if(flag_eow==TRUE){
			if(!END_OF_WORD(c)){
				fseek(stream,-1,SEEK_CUR);
				break;
			}
		}
		else{
			word[index]=c;
			index++;
		}
	}
	word[index]='\0';
	strcpy(dest,word);
	if(c==EOF)
		return EOF;
	else if(END_OF_PHRASE(c))
		return 2;
	else
		return 1;
	return 0;
}

