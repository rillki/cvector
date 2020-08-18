#include <stdio.h>

#include "vector/vector.h"

int main() {
    mvector_create(v, double);

    printf("vector_create ...\n\n");
    printf("length: %ld, capacity: %ld, available space: %ld\n\n", mvector_length(v), mvector_capacity(v), mvector_availableSpace(v));
    
    double arr[] = {1.12, 2.32, 3.99, 4.001, 5.00973, 6.2938928};
    double x = 99.002;
    double y = 2.0023;
    mvector_push(v, x);
    mvector_push(v, y);

    printf("vector_push ...\n\n");
    printf("length: %ld, capacity: %ld, available space: %ld\n\n", mvector_length(v), mvector_capacity(v), mvector_availableSpace(v));
    
    mvector_pushArr(v, arr, 6);
    
    printf("vector_pushArr ...\n\n");
    printf("length: %ld, capacity: %ld, available space: %ld\n\n", mvector_length(v), mvector_capacity(v), mvector_availableSpace(v));
    
    double z = 111.111;
    mvector_insert(v, 4, z);

    for(size_t i = 0; i < mvector_length(v); i++) {
	printf("%f ", mvector_get(v, i, double));
    }
    printf("\n\n");
    
    mvector_free(v);
    
    printf("vector freed...\n\n");
    printf("length: %ld, capacity: %ld, available space: %ld\n\n", mvector_length(v), mvector_capacity(v), mvector_availableSpace(v));
    return 0;
}
