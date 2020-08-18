#include <stdio.h>

#include "vector/vector.h"

int main() {
    mvector_create(v, char*);

    char* h = "hello world!";
    mvector_push(v, h);
    
    for(size_t i = 0; i < mvector_length(v); i++) {
	printf("%s\n", mvector_get(v, i, char*));
    }

    mvector_free(v);

    return 0;
}
