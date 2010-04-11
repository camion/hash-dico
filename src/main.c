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
    extern int optind;
    FILE* text=NULL;
    List* hash_table=NULL;
    List  l=NULL;
    char* word=NULL;
    char* output=NULL;
    int menu=0;
    int menu_mode=0;
    int need_hash=0;
    int need_list=0;


    /* execute quit() on SIGINT (ctrl-c) */
    signal(SIGINT,quit);
    /* check if root execute the program */
    if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);

    if(argc==1){
	usage(stderr);
	return 1;
    }
    if(argc>2){
/**********************/
/* command line stuff */
/**********************/
	while (EOF!=(option=getopt(argc,argv,optstring))) {
	    switch (option) {
	    case 'a':
		menu=1;
		word = malloc((strlen(optarg)+1)*sizeof(char));
		strcpy(word, optarg);
		break;
	    case 'p':
		need_hash=1;
		menu=2;
		word = malloc((strlen(optarg)+1)*sizeof(char));
		strcpy(word, optarg);
		break;
	    case 'P':
		need_hash=1;
		menu=3;
		word = malloc((strlen(optarg)+1)*sizeof(char));
		strcpy(word, optarg);
		break;
	    case 'l':
		menu=4;
		need_list=1; printf("Print words of text sorted list.\n"); break;
	    case 'd':
		menu=5;
		need_list=1;
		printf("Print words begining with \"%s*\" in the text.\n",optarg); break;
	    case 'D':
		menu=6;
		need_list=1;
		output = malloc((strlen(optarg)+1)*sizeof(char));
		strcpy(output, optarg);
	    case 'v': verbose=1; break;
	    case 'h': usage(stdout); return 0;
	    case ':': fprintf(stderr,"Option -%c requires an operand\n",optopt); usage(stderr); return 1;
	    case '?': fprintf(stderr,"Unknown option %c\n",optopt); usage(stderr); return 1;
	    }
	}
	/* if(optind+1 != argc){ */
	/*     fprintf(stderr,"You must specifiate filename\n"); */
	/*     usage(stderr); */
	/*     return 1; */
	/* } */

/**************************/
/* interactive menu stuff */
/**************************/
    }else{/* interactively get user request */
	menu_mode=1;
	verbose=1; /*in this mode ... speak a lot! */
	for(option=main_menu(); option!=EOF && option!=0; option=main_menu()){
	    switch (option) {
	    case 1: printf("camion");

	    }
	}
    }


/*********************/
/* do things needed  */
/*********************/
    if((text=fopen(argv[optind],"r")) == NULL){
	fprintf(stderr,"Unable to open %s\n",argv[optind]);
	usage(stderr);
	exit(1);
    }

    if(need_list)need_hash=1;/* list need this */

    if(need_hash){
	hash_table=init_hash_table();
	parse_text(text, hash_table);
	if(need_list)l=create_sorted_list(hash_table);
    }

/****************************/
/* do explicit user request */
/****************************/
    switch(menu){
    case 1:
	if(hash_table==NULL){/* go faster if don't need hash to look directly in the text */
	    if(search_word_text(text, word))printf("TRUE\n");
	    else printf("FALSE\n");
	}else{/* if we got hash look in this */
	    if(search_word(hash_table, word))printf("TRUE\n");
	    else printf("FALSE\n");
	}
	break;
    case 3:
	print_sentences_containing(text, stdout, hash_table, word);
	break;
    case 6:
	if(verbose)
	    printf("Saving sorted list in \"%s.DICO\"...\n",output); break;
	save_index(l, output);
	break;
    }


/******************/
/* cleaning stuff */
/******************/
    fclose(text);
    if(hash_table != NULL){
	free_hash(hash_table);
	if(l != NULL)free_sorted_list(&l);
    }
    if(word != NULL)free(word);
    if(output != NULL)free(output);

    return 0;
}

