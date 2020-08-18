#include <stdio.h>

#include "vector/vector.h"

int main() {
    mvector_create(v, char*);

    char* h = "hello world!";
    mvector_push(v, h);
    
    for(size_t i = 0; i < mvector_length(v); i++) {
	printf("%s ", mvector_constGet(v, i, char*));
    }
    printf("\n");

    mvector_free(v);

    return 0;
}
