#ifndef _LIST_
#define _LIST_

/* List of positions */
typedef struct cellpos{
	int position;
	struct cellpos* next;
}Celpos,*Listpos;

/* A word (string) go with a list of all its positions */
typedef struct celword{
	char *word;
	Listpos positions;
}Celword;

/* List of words  */
typedef struct cell{
	Celword *value;
	struct cell* next;
}Cell,*List;

#endif
