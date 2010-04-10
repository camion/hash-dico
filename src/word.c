#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/word.h"
#include "../include/read.h"
#include "../include/list.h"
#include "../include/util.h"



/*
  Allocates memory for a word
  The size of the string is not considered
  An empty word can be inserted so check before calling
  Returns the new allocated cell, or exit with 2 if not enouth memory
*/
List alloc_cell_word(char* word){
    List new_cell;
    int size;
    /* Allocates the box of the cell (Cellword + List) */
    if((new_cell = malloc(sizeof(Cell))) == NULL){
        fprintf(stderr,"Error while allocating a cell\n");
        exit(2);
    }
    /* Allocates the box of the word (string + Listpos)*/
    if((new_cell->value = malloc(sizeof(Cellword))) == NULL){
        fprintf(stderr,"Error while allocating a cell\n");
        exit(2);
    }
    size=strlen(word);
    /* Allocates the string */
    if((new_cell->value->word = malloc(size*sizeof(char)+1)) == NULL){
        fprintf(stderr,"Error while allocating a word\n");
        exit(2);
    }
    /* The list of its position is void */
    new_cell->value->positions = NULL;
    /* Copy the word into the new cell.
       We can use strcpy in place of strncpy because we have allocated exactly
       the place needed.. so */
    strcpy(new_cell->value->word, word);
    new_cell->next = NULL;
    return new_cell;
}

/* void lower_string(char *string){ */
/*     int i; */
/*     for(i=0; string[i]!='\0'; ++i) */
/*         string[i]=tolower(string[i]); */
/* } */


/*
  Inserts a word into the list *w respecting the lexicographical order
  and adds it position contained in offset.
  This function cannot duplicate word : if the word is already in, it
  just adds the word position.
*/
void insert_lexico_word(List *w, char* word, long offset){
    List new_cell, tmp1, tmp2;
    int result;

    /* if the list *w is empty */
    if(*w==NULL){
        new_cell=alloc_cell_word(word);
        *w=new_cell;
        insert_head_pos(&(new_cell->value->positions), offset);
    }
    /* if the word must be inserted before the head of the list */
    else if(strcmp(word, (*w)->value->word)<0){
        /* if the word is < head */
        new_cell=alloc_cell_word(word);
        new_cell->next = *w;
        *w=new_cell;
        insert_head_pos(&(new_cell->value->positions), offset);
    /* if the word must be inserted after the head of the list */
    }else{
        /* search word's position */
        tmp1=tmp2=*w;
        while(tmp2!=NULL && (result=strcmp(tmp2->value->word, word))<0){
            tmp1 = tmp2;
            tmp2 = tmp2->next;
        }
        /* if the word is not already in list */
        if(result!=0){
            new_cell=alloc_cell_word(word);
            new_cell->next = tmp2;
            tmp1->next = new_cell;
        }
        else
            insert_head_pos(&(tmp2->value->positions), offset);
    }
}



List search_word(List hash[], char* word){
    List w =  hash[hash_string(word)%HASH_SIZE];
    for(; w!=NULL; w=w->next)
        if(strcmp(w->value->word, word) == 0)
	        return w;
    return NULL;
}

void print_list_word(List w){
    if(w==NULL){
        printf("Empty List of words\n");
        return;
    }
    for(; w!=NULL; w=w->next)
        printf("%p [ %s -> %p ]\n",w,w->value->word,w->next);

}

/*
    Merges two list respecting lexical order
    The merged list is stored in w1
    w2 stay intact
*/
void merge_list(List *w1, const List w2){
    List new_cell;
    List tmp1, tmp2, tmp3;
    /* We cannot merge with a void list */
    if(w2==NULL)
        return;
    /* for every element of w2 */
    for(tmp3=w2;tmp3!=NULL;tmp3=tmp3->next){
        new_cell=(List)malloc(sizeof(List));
        new_cell->value=tmp3->value;
        new_cell->next=NULL;
        /* if the list *w is empty */
        if(*w1==NULL)
            *w1=new_cell;
        /* if the word must be inserted before the head of the list */
        else if(strcmp(tmp3->value->word, (*w1)->value->word)<0){
            /* if the word is < head */
            new_cell->next = *w1;
            *w1=new_cell;
        }
        /* if the word must be inserted after the head of the list */
        else{
            /* search word's position */
            tmp1=tmp2=*w1;
            while(tmp2!=NULL && strcmp(tmp2->value->word, tmp3->value->word)<0){
                tmp1 = tmp2;
                tmp2 = tmp2->next;
            }
            new_cell->next = tmp2;
            tmp1->next = new_cell;
        }
    }
}


/*
    Creates a sorted list from lists contained in hash[]
    It merges all list in hash, in the first box of hash[]
    Returns the list created
*/
List create_sorted_list(List hash[]){
    int i;
    /* We merge all lists with the first */
    List w = NULL;
    for(i=0;i<HASH_SIZE;i++)
        merge_list(&w,hash[i]);
    return w;
}

void free_list_word(List *w){
    List tmp=*w;
    while(*w!=NULL){
        tmp=(*w)->next;
        free_list_pos(&(*w)->value->positions);
        free((*w)->value->word);
        free((*w)->value);
        free(*w);
        *w=tmp;
    }
    *w=NULL;
}

void print_sentences_containing(FILE* stream, List hash[], char *word){
    List w =  search_word(hash,word);
    if(w==NULL)
        return;
    Listpos tmp=w->value->positions;
    int i=count_list_pos(tmp);
    char c;

	printf("\t%s apparaît dans %d phrases du texte :\n\n",word,i);
	/* print every sentences */
    for(i=1;tmp!=NULL;tmp=tmp->next,i++){
        /* we set stream position at the begining of the sentence*/
        fseek(stream,tmp->position,SEEK_SET);
        printf("\t> #%d ",i);
        while((c=fgetc(stream)) && c!=EOF){
            printf("%c",c);
            if(END_OF_PHRASE(c))
                break;
        }
        printf("\n");
    }
}

