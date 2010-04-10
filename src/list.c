#include <stdio.h>
#include <stdlib.h>
#include "../include/list.h"

/*
    Allocates memory for a position
    Returns the new cell allocated
    or NULL if memory error
 */
Listpos alloc_cell_pos(int value){
	Listpos new_cell;
	if((new_cell=malloc(sizeof(Cellpos)))!=NULL){
		new_cell->position=value;
		new_cell->next=NULL;
	}
	return new_cell;
}

/*
    Inserts a position(int) at the head of the list 
    Exit the program if a memory error occures
*/
void insert_head_pos(Listpos *l, int value){
	Listpos new_cell;
	if((new_cell=alloc_cell_pos(value))==NULL){
		fprintf(stderr,"Error while allocating a position\n");
		exit(2);
	}
	new_cell->next=*l;
	*l=new_cell;
}

/*
    Count and returns number of elements 
    of a list of positions
*/
int count_list_pos(Listpos l){
    int i=0;
    for(; l!=NULL; l=l->next)
	        i++;
    return i;
}

/*
    Print all elements of list
    For debug only
*/
void print_list_pos(Listpos l){
	if(l==NULL)
		printf("Empty list of positions\n");
	for(;l!=NULL;l=l->next)
		printf("%p [ %4ld -> %p ]\n",l,l->position,l->next);
}

/*
    Free all elements of the list
    *l=NULL after free
*/
void free_list_pos(Listpos *l){
	Listpos tmp=*l;
	while(*l!=NULL){
		tmp=(*l)->next;
		free(*l);
		*l=tmp;
	}
	*l=NULL;
}

