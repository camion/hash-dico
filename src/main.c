#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <getopt.h>

#include "../include/read.h"
#include "../include/util.h"
#include "../include/word.h"

void quit(){
    printf("\nctrl-c caught, exiting...\n");
    exit(1);
}

void usage(FILE *stream){
    fprintf(stream,"USAGE screen\n");
    fprintf(stream,"SYNOPSIS:\n Index [option] file\n");
}

int main(int argc, char* argv[]){
    const char* optstring=":ha:ld:D:";
    int option;

/* execute quit() on SIGINT (ctrl-c) */
    signal(SIGINT,quit);
/* check if root execute the program */
    if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);


    while (EOF!=(option=getopt(argc,argv,optstring))) {
	switch (option) {
	case 'h': usage(stdout); return 0;
	case 'l': printf("liste triee\n"); break;
	case 'a': printf("mot %s\n",optarg); break;
	case 'd': printf("mot comme prefix: %s\n",optarg); break;
	case 'D': printf("Save sorted list in %s.DICO\n",optarg); break;
	case 'i': printf("input %s\n",optarg); break;
	case ':': printf("arg missing for option %c\n",optopt); usage(stderr); return 1;
	case '?': printf("unknown option %c\n",optopt); break;
	}
    }




    List *hash_table=init_hash_table();
    FILE* text=fopen("Colomba.txt","r");
    parse_text(text, hash_table);

    print_sentences_containing(text,hash_table,"regrets");
    fclose(text);
    free_hash(hash_table);
    return 0;
}

