#include "../include/read.h"


/*
  Extract a word from a stream already opened
  The word is stored in dest
  Returns 3 values :
  1     : the word is extracted, everything is OK
  2     : the word extracted is the last word of the phrase
  EOF   : no more word to extract -> End Of File
*/
int get_word(FILE* stream, char* dest, size_t n){
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
    if(index>0)
        strncpy(dest,word,n);
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
int parse_text(char* file_name, List *hash_table){

    FILE* text;
    char word[WORD_BUFFER];
    int result=0;
    long offset;
    int key;

    if((text=fopen(file_name,"r"))==NULL){
        fprintf(stderr,"Error while opening %s\n",file_name);
        return 0;
    }
    offset=ftell(text);
    while(result!=EOF){
        if(result==2)
            offset=ftell(text);
        result=get_word(text,word,WORD_BUFFER);
        if(result==EOF)
            break;
        if(word[0]=='\0')
            continue;
        key=hash_string(word)%HASH_SIZE;
	insert_lexico_word(&(hash_table[key]), word, offset, 1);
    }
    fclose(text);
    return 1;
}

int word_is_in_text(List *hash, char *word){
    return ((search_word(hash[hash_string(word)%HASH_SIZE],word)!=NULL)?TRUE:FALSE);
}

