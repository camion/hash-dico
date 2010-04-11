#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /*winsize*/
#include <sys/ioctl.h>
#include <string.h>

#include  "../include/util.h"

/*
    Calculates a fingerprint for a string
    and returns it.
    We can with this find a key for a word
    to store it into a hash table.
*/
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


/*
    Allocates memory for the hash
    and init all list with NULL (calloc)
*/
List* init_hash_table(){
    List* hash=(List*)calloc(HASH_SIZE,sizeof(Cell));
    if(hash==NULL){
        fprintf(stderr,"Error while allocating hash table\n");
        exit(1);
    }
    return hash;
}


/*
    Free entirely the hash and every sub-list contained in hash
*/
void free_hash(List *hash){
    int i;
    for(i=0;i<HASH_SIZE;i++){
        free_list_word(&hash[i]);
    }
    free(hash);
}


/*
    Save the index in a text file
    If there's no file_name, index is saved with a default_name
*/
void save_index(List index,char* basename){
    Listpos tmp;
    char file_name[strlen(basename)+6]; /* 6=strlen(".DICO")+'\0' */
    strcpy(file_name,basename);
    strcat(file_name,".DICO");
    int size_name=strlen(basename);
    /* if nothing to save, get out of here! */
    if(index==NULL)
        return;
        
    FILE* target;
    /* if there's a file name we create it */
    if(size_name!=0)
        target=fopen(file_name,"w");
    /* or we create a default file */
    else
        target=fopen("default.DICO","w");
    if(target==NULL){
        fprintf(stderr,"Unable to save index in %s\n",(size_name==0)?"default.DICO":file_name);
        return;
    }
    /* browses all words */
    for(;index!=NULL;index=index->next){
        /* print the word */
        fprintf(target,"%s ",index->value->word);
        /* browses all position */
        for(tmp=index->value->positions;tmp!=NULL;tmp=tmp->next)
            fprintf(target,"%ld ",tmp->position);
        fprintf(target,"\n");
    }
    fclose(target);
}


/*
    Count number of columns in the current prompt
    Useful for progress bar.
*/
int column_count(){
    struct winsize ws;
    ioctl(1, TIOCGWINSZ, &ws);
    return ws.ws_col;
}

/*
    Print usage of the program, how to use this program.
    Its printed in the stream given argument *stream.
*/
void usage(FILE *stream){
    int columns=column_count(), i;

    for(i=0; i<columns; ++i)fprintf(stream,"-");
    fprintf(stream,"\nSYNOPSIS:\nIndex [option] file\n");
    fprintf(stream,"\tor\nIndex\n");
    for(i=0; i<columns; ++i)fprintf(stream,"-");
    fprintf(stream,"\nExamples:\n");
    fprintf(stream,"Index -a word file\t| Check if word is in file.\n");
    fprintf(stream,"Index -p word file\t| Print word positions in file.\n");
    fprintf(stream,"Index -P word file\t| Print sentences containing word in file.\n");
    fprintf(stream,"Index -l word     \t| Print sorted list of text's words.\n");
    fprintf(stream,"Index -d word file\t| Print words begining with word in the text.\n");
    fprintf(stream,"Index -D out  file\t| Save sorted list of file's words in out.DICO\n");
    fprintf(stream,"Index -h out  file\t| Print this help\n");
    for(i=0; i<columns; ++i)fprintf(stream,"-");
}


/*
    Print the evolution of the hashing.
    Printed only if '-v' verbose option is called.
*/
int hash_progress_bar(long position, long size){
    char* begin = "Hashing [";
    char* end = "] ";
    int columns = column_count();
    int used = strlen(begin)+strlen(end)+4; /* column used with begin + end + percent */
    int progress = (position * (columns - used))/ size ;
    int percent =  (position * 100)/ size;
    int i;

    /* fseek(stdout,0,0); */
    while(fgetc(stdout)!=EOF)
	fseek(stdout, -1, SEEK_CUR);

    if(columns < used+progress)return 0;/*not enouth place*/
    printf("%s",begin);
    for(i=0; i<progress; ++i, ++used)printf("#");
    for(; used<columns; ++used)printf("-");
    printf("%s%3d%%\r", end, percent);

    fflush(stdout);
    return percent;
}


/*
    Calculates and returns size of a file
*/
long filesize(FILE* f) {
   long old_pos=ftell(f);
   long size;

   fseek(f,0,SEEK_END);
   size=1+ftell(f);
   fseek(f,old_pos,SEEK_SET);
   return size;
}
