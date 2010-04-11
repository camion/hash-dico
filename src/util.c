#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /*winsize*/
#include <sys/ioctl.h>
#include <string.h>

#include  "../include/util.h"

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

List* init_hash_table(){
    List* hash=(List*)calloc(HASH_SIZE,sizeof(Cell));
    if(hash==NULL){
        fprintf(stderr,"Error while allocating hash table\n");
        exit(1);
    }
    return hash;
}

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
void save_index(List index,char* file_name){
    Listpos tmp;
    /* if nothing to save, get out of here! */
    if(index==NULL)
        return;
        
    FILE* target;
    /* if there's a file name we create it */
    if(strlen(file_name)!=0)
        target=fopen(file_name,"w");
    /* or we create a default file */
    else
        target=fopen("default_name.DICO","w");
    if(target==NULL){
        fprintf(stderr,"Unable to save index in %s\n",file_name);
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

int column_count(){
/* return the number of columns in current term */
    struct winsize ws;
    ioctl(1, TIOCGWINSZ, &ws);
    return ws.ws_col;
}

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

long filesize(FILE* f) {
   long old_pos=ftell(f);
   long size;

   fseek(f,0,SEEK_END);
   size=1+ftell(f);
   fseek(f,old_pos,SEEK_SET);
   return size;
}
