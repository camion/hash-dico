#include <stdio.h>
#include "../include/word.h"

int main(int argc, char* argv[]){
	List w=NULL;
	insert_head_word(&w,"et");
	insert_head_word(&w,"merde");
	print_list_word(w);
	return 0;
}

