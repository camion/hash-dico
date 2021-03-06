#include <stdio.h>
#include <unistd.h>
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
    printf("\n\tMENU: get input file\n"); lines-=2;
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("\n"); lines--;
    printf("Enter path to text.\n"); lines--;
    for(i=2; i<lines; ++i)printf("\n");/* fill lines i=2 because 2 lines after */
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("Input file name >");
    (void)scanf("%s",answer);
    printf("\n");
    if((text=fopen(answer,"r"))==NULL)
    	return get_input_filename();
    return text;
}

void get_string(char* word,char* message){
/*you should not ahve any \n in message and strlen(message) < columns*/
    int columns=column_count(),lines=line_count(), i;

    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("\n\tMENU:\n"); lines-=2;
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf("\n"); lines--;
    printf("%s",message); lines--;
    for(i=2; i<lines; ++i)printf("\n");/* fill lines i=2 because 2 lines after */
    for(i=0; i<columns; ++i)printf("%c",(i%2)?'*':'-');
    printf(">");
    scanf("%s",word);
    printf("\n");
}

/*
  Print usage of the program, how to use this program.
  Its printed in the stream given argument *stream.
*/
void usage(FILE *stream){
    fprintf(stream,"\nSYNOPSIS:\nIndex [option] file\n");
    fprintf(stream,"\tor\nIndex [file]\n");
    fprintf(stream,"\nExamples:\n");
    fprintf(stream,"Index -a word file\t| Check if word is in file.\n");
    fprintf(stream,"Index -p word file\t| Print word positions in file.\n");
    fprintf(stream,"Index -P word file\t| Print sentences containing word in file.\n");
    fprintf(stream,"Index -l text     \t| Print sorted list of text's words.\n");
    fprintf(stream,"Index -d word file\t| Print words begining with word in the text.\n");
    fprintf(stream,"Index -D out  file\t| Save sorted list of file's words in out.DICO\n");
    fprintf(stream,"Index -h          \t| Print this help\n");
}

char main_menu(){
    int columns=column_count(),lines=line_count(), i;
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
    i=-1;
    scanf("%d",&i);/* get the user choice */
    printf("\n");
    return i;/* return the number */
}

void sub_main_command(int argc, char *argv[]){
    int size;
    int menu=0;
    List *hash,tmp;
    List sorted_list=NULL;
    FILE* text;
    size=strlen(argv[1]);
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
    else if(strcmp(argv[1],"-p")==0 || strcmp(argv[1],"-vp")==0 || strcmp(argv[1],"-pv")==0){
        if(argc==3){
            fprintf(stderr,"Arguments missing, see usage\n");
            return;
        }
        menu=1;
    }
    else if(strcmp(argv[1],"-P")==0 || strcmp(argv[1],"-vP")==0 || strcmp(argv[1],"-Pv")==0){
        if(argc==3){
            fprintf(stderr,"Arguments missing, see usage\n");
            return;
        }
        menu=2;
    }
    else if(strcmp(argv[1],"-l")==0 || strcmp(argv[1],"-vl")==0 || strcmp(argv[1],"-lv")==0)
        menu=3;
    else if(strcmp(argv[1],"-d")==0 || strcmp(argv[1],"-vd")==0 || strcmp(argv[1],"-dv")==0){
        if(argc==3){
            fprintf(stderr,"Arguments missing, see usage\n");
            return;
        }
        menu=4;
    }
    else if(strcmp(argv[1],"-D")==0 || strcmp(argv[1],"-vD")==0 || strcmp(argv[1],"-Dv")==0){
        if(argc==3){
            fprintf(stderr,"Arguments missing, see usage\n");
            return;
        }
        menu=5;
    }
    if(!menu){
        fprintf(stderr,"Argument %s unknown\n",argv[1]);
        return;
    }
    else{
        hash=init_hash_table();
        if(menu==3)
            text=fopen(argv[2],"r");
        else
            text=fopen(argv[3],"r");
        if(text==NULL){
            fprintf(stderr,"Error while opening %s\n",(menu==3)?argv[2]:argv[3]);
            return;
        }
        parse_text(text,hash);
        sorted_list=create_sorted_list(hash);
        switch(menu){
            case 1 :
                if((tmp=search_word(hash,argv[2]))==NULL)
                    fprintf(stderr,"\"%s\" not found in %s\n",argv[2],argv[3]);
                else{
                    printf("\t> \"%s\" is found at these positions :\n",argv[2]);
                    print_list_pos(tmp->value->positions);
                }
            break;
            case 2 :
                print_sentences_containing(text,stdout,hash,argv[2]);
            break;
            case 3 :
                print_list_word(sorted_list);
            break;
            case 4 :
                print_words_beginning_with(text,stdout,sorted_list,argv[2]);
             break;
            case 5 :
	        if(verbose)
    		    printf("Saving sorted list in \"%s.DICO\"...\n",argv[2]);
                    save_index(sorted_list,argv[2]);
            break;
    	}
        free_hash(hash);
        free_sorted_list(&sorted_list);
        fclose(text);
    }

}


void sub_main_interractive(FILE* text){
    int option;
    List  l=NULL,tmp;
    List* hash_table=NULL;
    char word[WORD_BUFFER];
    char output[FILENAME_MAX];

    verbose=1;
    for(option=main_menu(); option!=EOF; option=main_menu()){
        if(option==0){
	    /* cleaning */
	    if(hash_table != NULL)free_hash(hash_table);
	    if(l != NULL)free_sorted_list(&l);
	    return;
	}
        /* do things needed  */
        if(hash_table==NULL){
            hash_table=init_hash_table();
            parse_text(text, hash_table);
            if(l==NULL)l=create_sorted_list(hash_table);
        }

        /* do explicit user request */
        switch(option){
	case 1:/* Check if word is in file. */
	    get_string(word,"Enter the word to search.");
	    if(hash_table==NULL){/* go faster if don't need hash to look directly in the text */
		if(search_word_text(text, word))printf("TRUE\n");
		else printf("FALSE\n");
	    }else{/* if we got hash look in this */
		if(search_word(hash_table, word))printf("TRUE\n");
		else printf("FALSE\n");
	    }
            break;
	case 2:/* Print word positions in file. */
	    get_string(word,"Enter the word to search.");
            if((tmp=search_word(hash_table,word))==NULL)
                printf("\"%s\" not found.\n",word);
            else{
                printf("\t> \"%s\" is found at these positions :\n",word);
                print_list_pos(tmp->value->positions);
            }
	    break;
	case 3:/* Print sentences containing word in file. */
	    get_string(word,"Enter the word.");
	    print_sentences_containing(text, stdout, hash_table, word);
            break;
	case 4:/* Print sorted list of text's words. */
	    print_list_word(l);
	    break;
	case 5:/* Print words begining with word in the text. */
	    get_string(word,"Enter the prefix to search.");
	    print_words_beginning_with(text,stdout,l,word);
	    break;
	case 6:/* Save sorted list of file's words in out.DICO */
	    get_string(output,"Enter output \"name\" to generate output.DICO");
	    printf("Saving sorted list in \"%s.DICO\"...\n",output); break;
	    save_index(l, output);
            break;
        }
	sleep(2);
    }
/* cleaning */
    if(hash_table != NULL)free_hash(hash_table);
    if(l != NULL)free_sorted_list(&l);
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
