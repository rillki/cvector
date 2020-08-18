#include <stdio.h>

#include "vector/vector.h"

int main() {
    mvector_create(v, char*);

    char* h = "hello world!";
    mvector_push(v, h);
    
    for(size_t i = 0; i < mvector_length(v); i++) {
<<<<<<< HEAD
	printf("%s\n", mvector_get(v, i, char*));
=======
	printf("%f ", mvector_constGet(v, i, double));
>>>>>>> f430732bac7792571f1be57197cdbe4dab024d1f
    }

    mvector_free(v);

    return 0;
}
