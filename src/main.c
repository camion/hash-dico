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
   FILE* text=NULL;

    /* execute quit() on SIGINT (ctrl-c) */
    signal(SIGINT,quit);
    /* check if root execute the program */
    if(getgid() == 0)fprintf(stderr,"You shouldn't run %s as root.\n",argv[0]);

    if(argc==1){
	text=get_input_filename();
	sub_main_interractive(text);
        fclose(text);
    }
    else if(argc==2){
        if(strcmp(argv[1],"-h")==0){
            usage(stdout);
            return 0;
        }
	text=fopen(argv[1],"r");
	if(text==NULL)text=get_input_filename();
        sub_main_interractive(text);
        fclose(text);
    }
    else
	sub_main_command(argc,argv);

    return 0;
}

