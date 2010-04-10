#include <string.h>
#include <ctype.h>
#include "../include/read.h"
#include "../include/util.h"

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
            word[index]=tolower(c);
            index++;
        }
    }
    word[index]='\0';
    if(index>0)
        strncpy(dest,word,n);
    if(c==EOF && index==0)
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
void parse_text(FILE* text, List *hash_table){

    char word[WORD_BUFFER];
    int result=0;
    long offset;
    long file_size;
    int key;

    /* get the file size */
    fseek(text, 0, SEEK_END);
    file_size = ftell(text);
    rewind(text);

    offset=ftell(text);
    while(result!=EOF){
        if(result==2){/* end of sentence */
            offset=ftell(text);
	    if(offset%1000 == 1)/* just sometimes */
		progress_bar(offset, file_size);
	}
        result=get_word(text,word,WORD_BUFFER);
        /* if(result==EOF) */
        /*     break; */
        if(word[0]=='\0')
            continue;
        key=hash_string(word)%HASH_SIZE;
    	insert_lexico_word(&(hash_table[key]), word, offset);
    }
    progress_bar(1, 1);
    printf("\n");
}

