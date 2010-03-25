#include <stdio.h>
#include "../include/word.h"

int main(int argc, char* argv[]){
	FILE* text=fopen(argv[1],"r");
	while(get_word(text))
		;
	return 0;
}

