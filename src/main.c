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

/* execute quit() on SIGINT (ctrl-c) */
    signal(SIGINT,quit);
/* check if root execute the program */
    if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);

    List *hash_table=init_hash_table();
    FILE* text=fopen("test","r");
    parse_text(text, hash_table);
    print_list_pos(search_word(hash_table,"regrets")->value->positions);

    free_hash(hash_table);
    return 0;
}

