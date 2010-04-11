#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <getopt.h>

#include "../include/read.h"
#include "../include/util.h"
#include "../include/word.h"

int verbose=0;

void quit(){
    printf("\nctrl-c caught, exiting...\n");
    exit(1);
}

int main(int argc, char* argv[]){
    const char* optstring=":a:p:P:ld:D:hv";
    int option;
    int menu=0;
    int must_hash=0;
    int sorted_list=0;
    extern int optind;
    FILE* text;
    List* hash_table;
    char* word=NULL;

    /* execute quit() on SIGINT (ctrl-c) */
    signal(SIGINT,quit);
    /* check if root execute the program */
    if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);

    if(argc>2){
    /* getopt stuff */
	while (EOF!=(option=getopt(argc,argv,optstring))) {
	    switch (option) {
	    case 'a': printf("Check if \"%s\" is in the text.\n",optarg); break;
	    case 'p':
		must_hash=1;
		menu=2;
		word = malloc((strlen(optarg)+1)*sizeof(char));
		strcpy(word, optarg);
		break;
	    case 'P':
		must_hash=1;
		menu=3;
		word = malloc((strlen(optarg)+1)*sizeof(char));
		strcpy(word, optarg);
		break;
	    case 'l': sorted_list=1; printf("Print words of text sorted list.\n"); break;
	    case 'd': sorted_list=1; printf("Print words begining with \"%s*\" in the text.\n",optarg); break;
	    case 'D': sorted_list=1; printf("Save sorted list in %s.DICO\n",optarg); break;
	    case 'v': verbose=1; break;
	    case 'h': usage(stdout); return 0;
	    case ':': printf("Option -%c requires an operand\n",optopt); usage(stderr); return 1;
	    case '?': printf("Unknown option %c\n",optopt); usage(stderr); return 1;
	    }
	}
	if(optind+1 != argc){
	    fprintf(stderr,"You must specifiate filename\n");
	    usage(stderr);
	    return 1;
	}
    }else{/* interactive */


    }

    if((text=fopen(argv[optind],"r")) == NULL){
	fprintf(stderr,"Unable to open %s\n",argv[optind]);
	usage(stderr);
	return 1;
    }

    if(must_hash){
	hash_table=init_hash_table();
	parse_text(text, hash_table);
    }

    if(menu==3)print_sentences_containing(text, stdout, hash_table, word);


    fclose(text);
    if(must_hash)free_hash(hash_table);

    return 0;
}

