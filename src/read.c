#include "../include/read.h"


/*
  Extract a word from a stream already opened
  The word is stored in dest
  Returns 3 values :
  1     : the word is extracted, everything is OK
  2     : the word extracted is the last word of the phrase
  EOF   : no more word to extract -> End Of File
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

/*
  Analyses all the text and indexes each word
  Fills the hash table with lists of words
*/
int parse_text(char* file_name, List *hash){
    FILE* text;
    List tmp_word;
    Listpos tmp_pos;
    char word[WORD_BUFFER];
    int result=0;
    int offset;
    int key;
    if((text=fopen(file_name,"r"))==NULL){
        fprintf(stderr,"Error while opening %s\n",file_name);
        return 0;
    }
    offset=0;
    while(result!=EOF){
        if(result==2)
            offset=ftell(text);
        result=get_word(text,word);
        if(word[0]=='\0')
            continue;
        key=hash_string(word)%HASH_TABLE;
        /* Not in hash table -> we add it */
        if((tmp_word=search_word(hash[key],word))==NULL){
            insert_head_word(&hash[key],word);/*TODO insert lexico*/
            insert_head_pos(&(hash[key]->value->positions),offset);
        }
        /* if already in hash table -> we add its position */
        else{
            if(hash[key]->value->positions->position==offset)
                insert_head_pos(&(hash[key]->value->positions),offset)
                    }
    }
    fclose(text);
    return 1;
}

