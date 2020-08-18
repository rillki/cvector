#include "vector.h"

/* Documentation: READ vector.h */

/* ****** PUBLIC VECTOR METHODS ****** */

void vector_create(vector* v, const size_t elementSize) {
    v->capacity = VECTOR_INIT_CAPACITY;
    v->length = 0;
    v->elementSize = elementSize;
    
    v->data = NULL;
    v->data = malloc(v->elementSize * v->capacity);
    v->status = (v->data == NULL) ? (vectorStatus_error_init) : (vectorStatus_success);
}

void vector_reserve(vector* v, const size_t num) {
    if(internal_vector_isNULL(v)) { return; }
    
    internal_vector_resize(v, v->capacity + num);
}

void vector_setLength(vector* v, const size_t length) {
    if(internal_vector_isNULL(v)) { return; }

    internal_vector_resize(v, length);
}

void vector_push(vector* v, const void* item) {
    if(internal_vector_isNULL(v)) { return; }

    if(v->length >= v->capacity) {
	internal_vector_resize(v, v->capacity*VECTOR_GROWTH_RATE);
    }

    internal_vector_assign(v, (v->length)++, item);
}

void vector_insert(vector* v, const size_t index, const void* item) {
    if(internal_vector_isNULL(v)) { return; }

    if(internal_vector_checkIndexBounds(v, index)) {
	internal_vector_assign(v, index, item);
    }
}

void vector_pop(vector* v) {
    if(internal_vector_isNULL(v)) { return; }

    if(v->length > 0) {
	v->length--;
    }
}

void vector_remove(vector* v, const size_t index) {
    if(internal_vector_isNULL(v)) { return; }

    if(internal_vector_checkIndexBounds(v, index)) {
	internal_gswap(internal_vector_offset(v, index), internal_vector_offset(v, v->length-1), v->elementSize);
	vector_pop(v);
    }
}

void vector_shrink(vector* v) {
    if(internal_vector_isNULL(v)) { return; }

    internal_vector_resize(v, v->length+1);
}

void vector_free(vector* v) {
    if(internal_vector_isNULL(v)) {
	v->status = vectorStatus_freed;
	return;
    }
    
    free(v->data);
    
    v->data = NULL;
    v->length = v->capacity = 0;
    v->status = vectorStatus_freed;
}

void vector_clear(vector* v) {
    if(internal_vector_isNULL(v)) {
	return;
    }

    vector_free(v);
}

void* vector_get(vector* v, const size_t index) {
    if(internal_vector_isNULL(v)) { return NULL; }

    if(internal_vector_checkIndexBounds(v, index)) {
	return internal_vector_offset(v, index);
    }
    
    v->status = vectorStatus_error_elementDoesntExist;
    return NULL;
}

const void* vector_constGet(vector* v, const size_t index) {
    if(internal_vector_isNULL(v)) { return NULL; }

    if(internal_vector_checkIndexBounds(v, index)) {
	return internal_vector_offset(v, index);
    }

    v->status = vectorStatus_error_elementDoesntExist;
    return NULL;
}

void vector_assignArr(vector* v, const void* arr, const size_t length) {
    if(arr == NULL || length == 0 || internal_vector_isNULL(v)) {
	v->status = vectorStatus_error_null;
    }
    
    vector_setLength(v, length);
    v->length = length;
    memcpy(v->data, arr, v->length * v->elementSize);
    
    v->status = vectorStatus_success;
}

void vector_pushArr(vector* v, const void* arr, const size_t length) {
    if(arr == NULL || length == 0 || internal_vector_isNULL(v)) {
	v->status = vectorStatus_error_null;
    }
    
    if(vector_availableSpace(v) < length) {
	vector_reserve(v, length);
    }
    
    for(size_t i = 0; i < length; i++) {
	vector_push(v, (arr + i * v->elementSize));
    }

    v->status = vectorStatus_success;
} 

vectorStatus vector_status_code(const vector* v) {
    return v->status;
}

char* vector_status_msg(const vector* v) {
    switch(v->status) {
	case vectorStatus_error_init:
	    return "VECTOR STATUS MSG(-5): Failed to initialize the vector!\n";
	case vectorStatus_error_operation:
	    return "VECTOR STATUS MSG(-4): Failed to perform a vector function operation.\n";
	case vectorStatus_error_resize:
	    return "VECTOR STATUS MSG(-3): Failed to resize the vector!\n";
	case vectorStatus_error_elementDoesntExist:
	    return "VECTOR STATUS MSG(-2): Vector element does not exist!\n";
	case vectorStatus_error_null:
	    return "VECTOR STATUS MSG(-1): Vector is NULL!\n";
	case vectorStatus_success:
	    return "VECTOR STATUS MSG(1): Operation sucessful. No errors.\n";
	case vectorStatus_freed:
	    return "VECTOR STATUS MSG(2): Vector freed!\n";
    }

    return "VECTOR STATUS MSG(_): Uninitialzed, use vector_create(vecName)! or unknown error.\n";
}

void vector_status_msg_print(const vector* v) {
    fprintf(stderr, "\n%s\n", vector_status_msg(v));
}

void vector_status_msg_print_error(const vector* v) {
    if(v->status == vectorStatus_success) {
	return;
    }

    fprintf(stderr, "\n%s\n", vector_status_msg(v));
}

size_t vector_length(const vector* v) {
    return v->length;
}

size_t vector_capacity(const vector* v) {
    return v->capacity;
}

size_t vector_availableSpace(const vector* v) {
    return (v->capacity - v->length);
}

size_t vector_memory(const vector* v) {
    return (v->capacity * v->elementSize);
}

bool vector_isEmpty(const vector* v) {
    return (v->length == 0);
}

/* ****** PRIVATE VECTOR METHODS FOR INTERNAL USE ONLY ****** */

static void internal_vector_resize(vector* v, const size_t size) {
    void* data = NULL;
    data = realloc(v->data, v->elementSize * size);
    if(data == NULL) {
	v->status = vectorStatus_error_resize;
	return;
    }

    v->data = data;
    v->capacity = size;
    if(v->length > size) {
	v->length = size;
    }
}

static void* internal_vector_offset(const vector* v, const size_t index) {
    return (v->data + (index) * v->elementSize);
}

static void internal_vector_assign(vector* v, const size_t index, const void* item) {
    memcpy(internal_vector_offset(v, index), item, v->elementSize);
}

static bool internal_vector_checkIndexBounds(vector* v, const size_t index) {
    if(index >= 0 && index < v->length) {
	return true;
    }
    
    v->status = vectorStatus_error_operation;
    return false;
}

static bool internal_vector_isNULL(vector* v) {
    if(v->data == NULL) {
	v->status = vectorStatus_error_null;
	return true;
    }

    return false;
}

static void internal_gswap(void* a, void* b, const size_t size) {
    uint8_t temp[size];
    memmove(temp, a, size);
    memmove(a, b, size);
    memmove(b, temp, size);
}








