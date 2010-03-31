#include <stdio.h>
#include "../include/read.h"
#include "../include/util.h"
#include "../include/word.h"

int main(int argc, char* argv[]){
    List w=NULL;
    insert_lexico_word(&w,"amio",3434);
    insert_lexico_word(&w,"truc",0);
    insert_lexico_word(&w,"bobo",234);
    insert_lexico_word(&w,"bobo",234);

    print_list_word(w);
    return 0;
}

