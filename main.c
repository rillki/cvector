#include <stdio.h>

#include "vector/vector.h"

int main() {
    vector v;
    vector_create(&v, sizeof(double));

    printf("vector_create ...\n\n");
    printf("length: %ld, capacity: %ld, available space: %ld\n\n", vector_length(&v), vector_capacity(&v), vector_availableSpace(&v));
    
    double arr[] = {1.12, 2.32, 3.99, 4.001, 5.00973, 6.2938928};
    double x = 99.002;
    double y = 2.0023;
    vector_push(&v, (void*)(&x));
    vector_push(&v, (void*)(&y));

    printf("vector_push ...\n\n");
    printf("length: %ld, capacity: %ld, available space: %ld\n\n", vector_length(&v), vector_capacity(&v), vector_availableSpace(&v));
    
    vector_pushArr(&v, arr, 6);
    
    printf("vector_pushArr ...\n\n");
    printf("length: %ld, capacity: %ld, available space: %ld\n\n", vector_length(&v), vector_capacity(&v), vector_availableSpace(&v));
    
    for(size_t i = 0; i < vector_length(&v); i++) {
	printf("%f ", *(double*)vector_get(&v, i));
    }
    printf("\n\n");
    
    vector_free(&v);
    
    printf("vector freed...\n\n");
    printf("length: %ld, capacity: %ld, available space: %ld\n\n", vector_length(&v), vector_capacity(&v), vector_availableSpace(&v));
    return 0;
}
