#include "../include/word.h"
#include "../include/list.h"



/*
	Allocate memory for word
	The size of the string is not considered
	An empty word will be inserted
	Returns the new allocated cell, or NULL if a memmory error occured
*/
List alloc_cell_word(char* word){
	List new_cell;
	/* Allocates the box of the cell (Cellword + List) */
	if((new_cell=malloc(sizeof(Cell)))==NULL){
		fprintf(stderr,"Error while allocating a cell\n");
		return NULL;
	}
	/* Allocates the box of the word (string + Listpos)*/
	if((new_cell->value=malloc(sizeof(Cellword)))==NULL){
		fprintf(stderr,"Error while allocating a cell\n");
		return NULL;
	}
	/* Allocates the string */
	if((new_cell->value->word=malloc(strlen(word)*sizeof(char)))==NULL){
		fprintf(stderr,"Error while allocating a word\n");
		return NULL;
	}
	/* The list of its position is void */
	new_cell->value->positions=NULL;
	/* Copy the word into the new cell. We can use strcpy in place of strncpy because we have allocated exactly the place needed*/
	strcpy(new_cell->value->word,word);
	new_cell->next=NULL;
	return new_cell;
}

/*
	Insert a word word at the top of the list
	The size of the string is not considered
	An empty word can be inserted
	Returns 1 if *word is inserted succesfully, 0 either
*/
int insert_head_word(List *w, char* word){
	List new_cell;
	if((new_cell=alloc_cell_word(word))==NULL){
		fprintf(stderr,"Error while inserting a word\n");		
		return 0;
	}
	new_cell->next=*w;
	*w=new_cell;
	return 1;
}

int insert_tail_word(List *w, char* word){
	List new_cell;
	List last;
	if((new_cell=alloc_cell_word(word))==NULL){
		fprintf(stderr,"Error while inserting a word\n");
		return 0;
	}
	if(*w==NULL)
		*w=new_cell;
	else{
		last=last_cell_word(*w);
		last->next=new_cell;
	}
	return 1;
}

List last_cell_word(List w){
	while(w!=NULL && w->next!=NULL)
		w=w->next;
	return w;
}

List search_word(List w,char* word){
	if(w==NULL)
		return NULL;
	for(;w!=NULL;w=w->next)
		if(strcmp(w->value->word,word)==0)
			return w;
	return NULL;
}

void print_list_word(List w){
	if(w==NULL){
		printf("Empty List of words\n");
		return;
	}
	for(;w!=NULL;w=w->next)
		printf("%p [ %s -> %p ]\n",w,w->value->word,w->next);
	
}

