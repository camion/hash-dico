#include  "../include/util.h"

unsigned int hash(char *p){
	unsigned int h=0,g;
	for(;*p;p++){
		h = (h<<4) + *p;
		if(g=h&0xf0000000){
			h = h^(g>>24);
			h = h^g;
		}
	}
	return h;
}

 
