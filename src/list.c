#include "../include/list.h"

Listpos alloc_cell(int value){
	Listpos new_cell;
	if((new_cell=malloc(sizeof(Celpos)))!=NULL){
		new_cell->position=value;
		new_cell->next=NULL;
	}
	return new_cell;
}

int insert_head(Listpos *l, int value){
	Listpos new_cell;
	if((new_cell=alloc_cell(value))==NULL){
		fprintf(stderr,"Error while allocating a position\n");
		return 0;
	}
	new_cell->next=*l;
	*l=new_cell;
	return 1;
}

int insert_tail(Listpos *l, int value){
	Listpos new_cell;
	Listpos last;
	if((new_cell=alloc_cell(value))==NULL){
		fprintf(stderr,"Error while allocating a position\n");
		return 0;
	}
	if(*l==NULL)
		*l=new_cell;
	else{
		last=last_cell(*l);
		last->next=new_cell;
	}
	return 1;
}

Listpos last_cell(Listpos l){
	while(l!=NULL && l->next!=NULL)
		l=l->next;
	return l;
}

void print_list(Listpos l){
	if(l==NULL)
		printf("Empty list\n");
	for(;l!=NULL;l=l->next)
		printf("%p [ %4d -> %p ]\n",l,l->position,l->next);
}

