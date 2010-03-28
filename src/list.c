#include "../include/list.h"

Listpos alloc_cell_pos(int value){
	Listpos new_cell;
	if((new_cell=malloc(sizeof(Cellpos)))!=NULL){
		new_cell->position=value;
		new_cell->next=NULL;
	}
	return new_cell;
}

int insert_head_pos(Listpos *l, int value){
	Listpos new_cell;
	if((new_cell=alloc_cell_pos(value))==NULL){
		fprintf(stderr,"Error while allocating a position\n");
		return 0;
	}
	new_cell->next=*l;
	*l=new_cell;
	return 1;
}

int insert_tail_pos(Listpos *l, int value){
	Listpos new_cell;
	Listpos last;
	if((new_cell=alloc_cell_pos(value))==NULL){
		fprintf(stderr,"Error while inserting a position\n");
		return 0;
	}
	if(*l==NULL)
		*l=new_cell;
	else{
		last=last_cell_pos(*l);
		last->next=new_cell;
	}
	return 1;
}

Listpos last_cell_pos(Listpos l){
	while(l!=NULL && l->next!=NULL)
		l=l->next;
	return l;
}

Listpos search_pos(Listpos l, int value){
	if(l==NULL)
		return NULL;
	for(;l!=NULL;l=l->next)
		if(l->position==value)
			return l;
	return NULL;
}

void print_list_pos(Listpos l){
	if(l==NULL)
		printf("Empty list of positions\n");
	for(;l!=NULL;l=l->next)
		printf("%p [ %4d -> %p ]\n",l,l->position,l->next);
}

