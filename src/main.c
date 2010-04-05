#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "../include/read.h"
#include "../include/util.h"
#include "../include/word.h"

void quit(){
    printf("\nctrc-c caught, exiting...\n");
    exit(1);
}

int main(int argc, char* argv[]){


/* execute quit() on ctrl-c */
    signal(SIGINT,quit);
/* check if root execute the program */
    if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);

    List *hash_table=init_hash_table();

    parse_text("./Colomba.txt", hash_table);
    print_list_word(hash_table[0]);
    free_hash(hash_table);

    return 0;
}

