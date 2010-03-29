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
	int size_dest=0;
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
	size_dest=strlen(dest);
	strncpy(dest,word,(index<=size_dest?index:size_dest));
	if(c==EOF)
		return EOF;
	else if(END_OF_PHRASE(c))
		return 2;
	else
		return 1;
	return 0;
}

/*
	Analyses all the text and indexes each word
	Fills the hash table with lists of words
*/
int parse_text(char* file_name, List *hash){
	FILE* text;
	List tmp_word;
	char word[WORD_BUFFER];
	int result=0;
	int offset;
	int key;
	if((text=fopen(file_name,"r"))==NULL){
		fprintf(stderr,"Error while opening %s\n",file_name);
		return 0;
	}
	offset=ftell(text);
	while(result!=EOF){
		if(result==2)
			offset=ftell(text);
		result=get_word(text,word);
		if(word[0]=='\0')
			continue;
		key=hash_string(word)%HASH_TABLE;
		tmp_word=search_word(hash[key],word);
		/* Not in hash table -> we add it */
		if(tmp_word==NULL){
			insert_lexico_word(&hash[key],word);
			insert_head_pos(&(hash[key]->value->positions),offset);
		}
		/* if already in hash table -> we add its position */
		else{
			if(tmp_word->value->positions->position!=offset){
				insert_head_pos(&(hash[key]->value->positions),offset);
			}
		}
	}
	fclose(text);
	return 1;
}

