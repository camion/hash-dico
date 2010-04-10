#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
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

/* execute quit() on SIGINT (ctrl-c) */
    signal(SIGINT,quit);
/* check if root execute the program */
    if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);
    
    List *hash_table=init_hash_table();
    FILE* text=fopen("Colomba.txt","r");
    parse_text(text, hash_table);
    
    print_sentences_containing(text,hash_table,"regrets");
    fclose(text);
    free_hash(hash_table);
    return 0;
}

