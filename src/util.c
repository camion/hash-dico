#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /*winsize*/
#include <sys/ioctl.h>
#include <string.h>

#include  "../include/util.h"
#include  "../include/read.h"

/*
    Calculates a fingerprint for a string
    and returns it.
    We can with this find a key for a word
    to store it into a hash table.
*/
unsigned int hash_string(char *p){
/* maybe try with (i+1)*word[i] */
    unsigned int h=0,g;
    for(;*p;p++){
        h = (h<<4) + *p;
        if((g=h&0xf0000000)){
            h = h^(g>>24);
            h = h^g;
        }
    }
    return h;
}


/*
    Allocates memory for the hash
    and init all list with NULL (calloc)
*/
List* init_hash_table(){
    List* hash=(List*)calloc(HASH_SIZE,sizeof(Cell));
    if(hash==NULL){
        fprintf(stderr,"Error while allocating hash table\n");
        exit(1);
    }
    return hash;
}


/*
    Free entirely the hash and every sub-list contained in hash
*/
void free_hash(List *hash){
    int i;
    for(i=0;i<HASH_SIZE;i++){
        free_list_word(&hash[i]);
    }
    free(hash);
}


/*
    Save the index in a text file
    If there's no file_name, index is saved with a default_name
*/
void save_index(List index,char* basename){
    Listpos tmp;
    char file_name[strlen(basename)+6]; /* 6=strlen(".DICO")+'\0' */
    strcpy(file_name,basename);
    strcat(file_name,".DICO");
    /* if nothing to save, get out of here! */
    if(index==NULL)
        return;

    FILE* target;
    target=fopen(file_name,"w");
    if(target==NULL){
        fprintf(stderr,"Unable to save index in %s\n",file_name);
        return;
    }
    /* browses all words */
    for(;index!=NULL;index=index->next){
        /* print the word */
        if(fprintf(target,"%s ",index->value->word) == -1){
	        fprintf(stderr,"Unable to write in \"%s\n",file_name);
            fclose(target);
	        return;
    	}

        /* browses all position */
        for(tmp=index->value->positions;tmp!=NULL;tmp=tmp->next)
            fprintf(target,"%ld ",tmp->position);
        fprintf(target,"\n");
    }
    fclose(target);
}


/*
    Count number of columns in the current terminal
    Useful for nice print on stdout.
*/
int column_count(){
    struct winsize ws;
    ioctl(1, TIOCGWINSZ, &ws);
    return ws.ws_col;
}
int line_count(){
    struct winsize ws;
    ioctl(1, TIOCGWINSZ, &ws);
    return ws.ws_row;
}


FILE* get_input_filename(){
    int columns=column_count(),lines=line_count(), i;
    char answer[FILENAME_MAX];
    FILE* text;
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("\n\tMENU: get input file name\n"); lines-=2;
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("\n"); lines--;
    printf("Please enter input filename.\n"); lines--;
    for(i=2; i<lines; ++i)printf("\n");/* fill lines i=2 because 2 lines after */
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("Input file name >");
    scanf("%s",answer);
    printf("\n");
    if((text=fopen(answer,"r"))==NULL)
    	return get_input_filename();
    return text;
}

void get_string(char* word,char* message){
    int columns=column_count(),lines=line_count(), i;

    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("\n\tMENU: get input file name\n"); lines-=2;
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("\n"); lines--;
    printf("%s",message); lines--;
    for(i=2; i<lines; ++i)printf("\n");/* fill lines i=2 because 2 lines after */
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("Input file name >");
    scanf("%s",word);
    printf("\n");
}

/*
    Print usage of the program, how to use this program.
    Its printed in the stream given argument *stream.
*/
void usage(FILE *stream){
    int columns=column_count(), i;

    for(i=0; i<columns; ++i)fprintf(stream,"%c",(i%2)?'*':'-');
    fprintf(stream,"\nSYNOPSIS:\nIndex [option] file\n");
    fprintf(stream,"\tor\nIndex\n");
    for(i=0; i<columns; ++i)fprintf(stream,"%c",(i%2)?'*':'-');
    fprintf(stream,"\nExamples:\n");
    fprintf(stream,"Index -a word file\t| Check if word is in file.\n");
    fprintf(stream,"Index -p word file\t| Print word positions in file.\n");
    fprintf(stream,"Index -P word file\t| Print sentences containing word in file.\n");
    fprintf(stream,"Index -l text     \t| Print sorted list of text's words.\n");
    fprintf(stream,"Index -d word file\t| Print words begining with word in the text.\n");
    fprintf(stream,"Index -D out  file\t| Save sorted list of file's words in out.DICO\n");
    fprintf(stream,"Index -h out  file\t| Print this help\n");
    for(i=0; i<columns; ++i)fprintf(stream,"%c",(i%2)?'*':'-');
}

char main_menu(){
    int columns=column_count(),lines=line_count(), i;
    char answer;
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("\n\tMENU:\n"); lines-=2;
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("\n"); lines--;
    printf("0)\t| exit menu\n"); lines--;
    printf("1)\t| Check if word is in file.\n"); lines--;
    printf("2)\t| Print word positions in file.\n"); lines--;
    printf("3)\t| Print sentences containing word in file.\n"); lines--;
    printf("4)\t| Print sorted list of text's words.\n"); lines--;
    printf("5)\t| Print words begining with word in the text.\n"); lines--;
    printf("6)\t| Save sorted list of file's words in out.DICO\n"); lines--;
    for(i=2; i<lines; ++i)printf("\n");/* fill lines i=2 because 2 lines after */
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("Choice >");
    answer=getchar();
    printf("\n");
    return answer-'0';/* return the number */
}

void sub_main_command(int argc, char *argv[]){
    int size;
    int menu1,menu2,menu3,menu4,menu5;
    menu1=menu2=menu3=menu4=menu5=0;
    List *hash,tmp;
    List sorted_list=NULL;
    FILE* text;
    size=strlen(argv[1]);
    /*if(argc==3 && !(size==2 && strcmp(argv[1],"-l")==0)){
        fprintf(stderr,"Arguments missing, see usage\n");
        return;
    }*/
    if(size!=2 && size!=3){
        fprintf(stderr,"Argument %s unknown\n",argv[1]);
        return;
    }
    if((size==2 && argv[1][0]=='v') || (size==3 && (argv[1][1]=='v' || argv[1][2]=='v'))){
        verbose=1;
    }
    if(strcmp(argv[1],"-a")==0 || strcmp(argv[1],"-va")==0 || strcmp(argv[1],"-av")==0){
        if(argc==3){
            fprintf(stderr,"Arguments missing, see usage\n");
            return;
        }
        text=fopen(argv[3],"r");
        if(text==NULL){
            fprintf(stderr,"Error while opening %s\n",argv[3]);
            return;
        }
        if(search_word_text(text,argv[2]))
            printf("\"%s\" is FOUND in %s\n",argv[2],argv[3]);
        else
            printf("\"%s\" is NOT in %s\n",argv[2],argv[3]);
        return;
    }
    if(strcmp(argv[1],"-p")==0 || strcmp(argv[1],"-vp")==0 || strcmp(argv[1],"-pv")==0){
        if(argc==3){
            fprintf(stderr,"Arguments missing, see usage\n");
            return;
        }
        menu1=1;
    }
    if(strcmp(argv[1],"-P")==0 || strcmp(argv[1],"-vP")==0 || strcmp(argv[1],"-Pv")==0){
        if(argc==3){
            fprintf(stderr,"Arguments missing, see usage\n");
            return;
        }
        menu2=1;
    }
    if(strcmp(argv[1],"-l")==0 || strcmp(argv[1],"-vl")==0 || strcmp(argv[1],"-lv")==0)
        menu3=1;
    if(strcmp(argv[1],"-d")==0 || strcmp(argv[1],"-vd")==0 || strcmp(argv[1],"-dv")==0){
        if(argc==3){
            fprintf(stderr,"Arguments missing, see usage\n");
            return;
        }
        menu4=1;
    }
    if(strcmp(argv[1],"-D")==0 || strcmp(argv[1],"-vD")==0 || strcmp(argv[1],"-Dv")==0){
        if(argc==3){
            fprintf(stderr,"Arguments missing, see usage\n");
            return;
        }
        menu5=1;
    }
    if(!menu1 && !menu2 && !menu3 && !menu4 && !menu5){
        fprintf(stderr,"Argument %s unknown\n",argv[1]);
        return;
    }
    else{
        hash=init_hash_table();
        if(menu3)
            text=fopen(argv[2],"r");
        else
            text=fopen(argv[3],"r");
        if(text==NULL){
            fprintf(stderr,"Error while opening %s\n",(menu3)?argv[2]:argv[3]);
            return;
        }
        parse_text(text,hash);
        sorted_list=create_sorted_list(hash);
        if(menu1){
            if((tmp=search_word(hash,argv[2]))==NULL)
                fprintf(stderr,"\"%s\" not found in %s\n",argv[2],argv[3]);
            else{
                printf("\t> \"%s\" is found at these positions :\n",argv[2]);
                print_list_pos(tmp->value->positions);
            }
        }
        else if(menu2)
            print_sentences_containing(text,stdout,hash,argv[2]);
        else if(menu3)
            print_list_word(sorted_list);
        else if(menu4)
            print_words_beginning_with(text,stdout,sorted_list,argv[2]);
        else if(menu5)
            save_index(sorted_list,argv[2]);
        free_hash(hash);
        free_sorted_list(&sorted_list);
    }

}


void sub_main_interractive(FILE* text){
    int option;
    int need_hash=0;
    int need_list=0;
    List  l=NULL;
    List* hash_table=NULL;
    char word[WORD_BUFFER];
    char *output=NULL;

    for(option=main_menu(); option!=EOF; option=main_menu()){
        if(option==0)return;
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
}



/*
    Print the evolution of the hashing.
    Printed only if '-v' verbose option is called.
*/
int hash_progress_bar(long position, long size){
    char* begin = "Hashing [";
    char* end = "] ";
    int columns = column_count();
    int used = strlen(begin)+strlen(end)+4; /* column used with begin + end + percent */
    int progress = (position * (columns - used))/ size ;
    int percent =  (position * 100)/ size;
    int i;

    /* fseek(stdout,0,0); */
    while(fgetc(stdout)!=EOF)
	fseek(stdout, -1, SEEK_CUR);

    if(columns < used+progress)return 0;/*not enouth place*/
    printf("%s",begin);
    for(i=0; i<progress; ++i, ++used)printf("#");
    for(; used<columns; ++used)printf("-");
    printf("%s%3d%%\r", end, percent);

    fflush(stdout);
    return percent;
}


/*
    Calculates and returns size of a file
*/
long filesize(FILE* f) {
   long old_pos=ftell(f);
   long size;

   fseek(f,0,SEEK_END);
   size=1+ftell(f);
   fseek(f,old_pos,SEEK_SET);
   return size;
}
