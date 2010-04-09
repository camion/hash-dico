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

    /* parse_text("./Colomba.txt", hash_table); */
    /* printf("%p\n",search_word(hash_table,"regrets")->value->positions); */
    /* print_list_word(hash_table[0]); */
    List w=NULL;
    insert_lexico_word(&,"camion", (long)45);
    insert_lexico_word(&w,"camion", (long)55);
    insert_lexico_word(&w,"camzion", (long)55);
    print_list_pos(w->value->positions);



    free_hash(hash_table);

    return 0;
}

