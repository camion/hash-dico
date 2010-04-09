#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

#include "../include/read.h"
#include "../include/util.h"
#include "../include/word.h"

void quit(){
    printf("\nctrl-c caught, exiting...\n");
    exit(1);
}

int main(int argc, char* argv[]){


/* execute quit() on ctrl-c */
    signal(SIGINT,quit);
/* check if root execute the program */
    if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);

    List *hash_table=init_hash_table();
    FILE* text=fopen("test","r");
    int r;
    char s[WORD_BUFFER];
    r=get_word(text,s,WORD_BUFFER);
    r=get_word(text,s,WORD_BUFFER);
    r=get_word(text,s,WORD_BUFFER);
    r=get_word(text,s,WORD_BUFFER);
    r=get_word(text,s,WORD_BUFFER);
    r=get_word(text,s,WORD_BUFFER);
    r=get_word(text,s,WORD_BUFFER);
    printf("r:%d s:%s\n",r,s);
    return 0;
}

