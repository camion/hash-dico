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


int interractive(){
    int option;
    int need_hash=0;
    int need_list=0;
    List  l=NULL;
    List* hash_table=NULL;
    FILE* text=NULL;
    char* word;
    char* output=NULL;

    for(option=main_menu(); option!=EOF; option=main_menu()){
	if(option==0)return 0;
	if(option!=1)need_hash=1;
	if(option==4 || option==5 || option==6)need_list=1;

	/*********************/
	/* do things needed  */
	/*********************/
	if(need_list && l==NULL)need_hash=1;/* list need this */
	if(need_hash && hash_table==NULL){
	    hash_table=init_hash_table();
	    parse_text(text, hash_table);
	    if(need_list && l==NULL)l=create_sorted_list(hash_table);
	}
	/****************************/
	/* do explicit user request */
	/****************************/
	switch(option){
	case 1:
	    get_string(word,"Enter the word to search.");
	    if(hash_table==NULL){/* go faster if don't need hash to look directly in the text */
		if(search_word_text(text, word))printf("TRUE\n");
		else printf("FALSE\n");
	    }else{/* if we got hash look in this */
		if(search_word(hash_table, word))printf("TRUE\n");
		else printf("FALSE\n");
	    }
	    break;
	case 3:
	    get_string(word,"Enter the word.");
	    print_sentences_containing(text, stdout, hash_table, word);
	    break;
	case 6:
	    if(verbose)
		printf("Saving sorted list in \"%s.DICO\"...\n",output); break;
	    save_index(l, output);
	    break;
	}
    }

int main(int argc, char* argv[]){
	/* execute quit() on SIGINT (ctrl-c) */
	signal(SIGINT,quit);
	/* check if root execute the program */
	if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);
    FILE *text=NULL;
	if(argc==1 || text==NULL){
	    text=get_input_filename();
	    interractive();
	}
    else if(argc==2)
	    text=fopen(argv[1],"r");
	else if(argc>2)
	    sub_main_command(argc,argv)
    return 0;
}

