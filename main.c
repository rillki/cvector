#include <stdio.h>

#include "vector/vector.h"

int main() {
    mvector_create(v, char*);
    mvector_create(s, char*);

    char* h[] = {"hello world!", "apple", "orange", "rasbery", "moon", "sun", "earth", "circle", "radius"};

    mvector_pushVal(v, "americano", char*);
    mvector_push(v, h[1]);
    mvector_push(v, h[2]);
    mvector_push(v, h[3]);
    mvector_push(v, h[4]);

    mvector_insertVal(v, 2, "italiano", char*);
    
    mvector_push(s, h[5]);
    mvector_push(s, h[6]);
    mvector_push(s, h[7]);
    
    printf("vector v: length=%ld, cap=%ld\n", mvector_length(v), mvector_capacity(v));
    for(size_t i = 0; i < mvector_length(v); i++) {
	printf("%s \n", mvector_constGet(v, i, char*));
    }
    printf("\n");

    mvector_swap(v, s);

    printf("vector v: length=%ld, cap=%ld\n", mvector_length(v), mvector_capacity(v));
    for(size_t i = 0; i < mvector_length(v); i++) {
	printf("%s \n", mvector_constGet(v, i, char*));
    }
    printf("\n");
    
    printf("vector s: length=%ld, cap=%ld\n", mvector_length(v), mvector_capacity(v));
    for(size_t i = 0; i < mvector_length(s); i++) {
	printf("%s \n", mvector_constGet(s, i, char*));
    }
    printf("\n");

    mvector_free(v);
    mvector_free(s);
    return 0;
}
