#include <stdio.h>
#include "../include/read.h"
#include "../include/util.h"
#include "../include/word.h"

int main(int argc, char* argv[]){
	List w=NULL;
	insert_lexico_word(&w,"bateau");
	insert_lexico_word(&w,"avion");
	insert_lexico_word(&w,"aviateur");	
	print_list_word(w);
	free_list_word(&w);
	return 0;
}

