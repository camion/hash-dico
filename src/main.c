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


int main(int argc, char* argv[]){
    const char* optstring=":a:p:P:ld:D:h";
    int option;

    /* execute quit() on SIGINT (ctrl-c) */
    signal(SIGINT,quit);
    /* check if root execute the program */
    if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);

    /* getopt stuff */
    while (EOF!=(option=getopt(argc,argv,optstring))) {
	switch (option) {
	case 'a': printf("Check if \"%s\" is in the text.\n",optarg); break;
	case 'p': printf("Print \"%s\" positions in the text.\n",optarg); break;
	case 'P': printf("Print santances of the text containing \"%s\".\n",optarg); break;
	case 'l': printf("Print words of text sorted list.\n"); break;
	case 'd': printf("Print words begining with \"%s*\" in the text.\n",optarg); break;
	case 'D': printf("Save sorted list in %s.DICO\n",optarg); break;
	case 'h': usage(stdout); return 0;;
	case ':': printf("arg missing for option %c\n",optopt); usage(stderr); return 1;
	case '?': printf("unknown option %c\n",optopt); usage(stderr); return 1;
	}
    }




    List *hash_table=init_hash_table();
    FILE* text=fopen("Colomba.txt","r");
    parse_text(text, hash_table);

    print_sentences_containing(text,hash_table,"femmdzdzdzes");
    fclose(text);
    free_hash(hash_table);
    return 0;
}

