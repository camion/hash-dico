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
    fprintf(stream,"Index -P word file\t| Print santances containing word in file.\n");
    fprintf(stream,"Index -l word     \t| Print sorted list of text word's.\n");
    fprintf(stream,"Index -d word file\t| Print words begining with word in the text.\n");
    fprintf(stream,"Index -D out  file\t| Save sorted list of file's words in out.DICO\n");
    fprintf(stream,"Index -h out  file\t| Print this help\n");
    for(i=0; i<columns; ++i)fprintf(stream,"-");
}

void progress_bar(long position, long size){
    char* begin = "Progress [";
    char* end = "] ";
    int columns = column_count();
    int used = strlen(begin)+strlen(end)+4; /* column used with begin + end + percent */
    int progress = (position * (columns - used))/ size ;
    int percent =  (position * 100)/ size;
    int i;

    /* fseek(stdout,0,0); */
    while(fgetc(stdout)!=EOF)
	fseek(stdout, -1, SEEK_CUR);

    if(columns < used+progress)return;/*not enouth place*/
    printf("%s",begin);
    for(i=0; i<progress; ++i, ++used)printf("#");
    for(; used<columns; ++used)printf("-");
    printf("%s%3d%%\r", end, percent);

    fflush(stdout);
}
