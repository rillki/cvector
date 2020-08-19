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
    if(internal_vector_errorFound(v)) { 
	v->status = vectorStatus_error_operation;
	return;
    }
    
    internal_vector_resize(v, v->capacity + num);
}

void vector_setLength(vector* v, const size_t length) {
    if(internal_vector_errorFound(v)) { 
	v->status = vectorStatus_error_operation;
	return;
    }

    internal_vector_resize(v, length);
}

void vector_push(vector* v, const void* item) {
    if(internal_vector_errorFound(v)) { 
	v->status = vectorStatus_error_operation;
	return;
    }

    if(v->length >= v->capacity) {
	internal_vector_resize(v, v->capacity*VECTOR_GROWTH_RATE);
    }

    internal_vector_assign(v, (v->length)++, item);
}

void vector_insert(vector* v, const size_t index, const void* item) {
    if(internal_vector_errorFound(v)) { 
	v->status = vectorStatus_error_operation;
	return;
    }

    if(internal_vector_checkIndexBounds(v, index)) {
	internal_vector_assign(v, index, item);
    }
}

void vector_pop(vector* v) {
    if(internal_vector_errorFound(v)) {
	v->status = vectorStatus_error_operation;
	return;
    }

    if(v->length > 0) {
	v->length--;
    }
}

void vector_remove(vector* v, const size_t index) {
    if(internal_vector_errorFound(v)) { 
	v->status = vectorStatus_error_operation;
	return;
    }

    if(internal_vector_checkIndexBounds(v, index)) {
	internal_gswap(internal_vector_offset(v, index), internal_vector_offset(v, v->length-1), v->elementSize);
	vector_pop(v);
    }
}

void vector_shrink(vector* v) {
    if(internal_vector_errorFound(v)) { 
	v->status = vectorStatus_error_operation;
	return;
    }

    internal_vector_resize(v, v->length+1);
}

void vector_free(vector* v) {
    if(internal_vector_errorFound(v)) {
	v->status = vectorStatus_error_operation;
	return;
    }

    if(v->status == vectorStatus_freed) {
	return;
    }

    free(v->data);
    
    v->data = NULL;
    v->length = v->capacity = 0;
    v->status = vectorStatus_freed;
}

void vector_clear(vector* v) {
    if(internal_vector_errorFound(v)) {
	v->status = vectorStatus_error_operation;
	return;
    }

    vector_free(v);
}

void* vector_get(vector* v, const size_t index) {
    if(internal_vector_errorFound(v)) {
	v->status = vectorStatus_error_operation;
	return NULL;
    }

    if(internal_vector_checkIndexBounds(v, index)) {
	return internal_vector_offset(v, index);
    }
    
    v->status = vectorStatus_error_elementDoesntExist;
    return NULL;
}

const void* vector_constGet(vector* v, const size_t index) {
    if(internal_vector_errorFound(v)) { 
	v->status = vectorStatus_error_operation;
	return NULL;
    }

    if(internal_vector_checkIndexBounds(v, index)) {
	return internal_vector_offset(v, index);
    }

    v->status = vectorStatus_error_elementDoesntExist;
    return NULL;
}

void vector_copyArr(vector* v, const void* arr, const size_t length) {
    if(arr == NULL || length == 0 || internal_vector_errorFound(v)) {
	v->status = vectorStatus_error_null;
	return;
    }
    
    vector_setLength(v, length);
    v->length = length;
    memcpy(v->data, arr, v->length * v->elementSize);
    
    v->status = vectorStatus_success;
}

void vector_pushArr(vector* v, const void* arr, const size_t length) {
    if(arr == NULL || length == 0 || internal_vector_errorFound(v)) {
	v->status = vectorStatus_error_null;
	return;
    }
    
    if(vector_availableSpace(v) < length) {
	vector_reserve(v, length);
    }
    
    memmove((v->data + v->length * v->elementSize), arr, (length * v->elementSize));
    
    v->length += length;
    v->status = vectorStatus_success;
}

void vector_copy(vector* vdest, const vector* vsrc) {
    if(vdest == NULL || vsrc == NULL || internal_vector_errorFound(vdest) || internal_vector_errorFound(vsrc)) {
	vdest->status = vectorStatus_error_operation;
	return;
    }
    
    vdest->elementSize = vsrc->elementSize;
    vector_setLength(vdest, vsrc->length);
    vdest->length = vsrc->length;

    memmove(vdest->data, vsrc->data, vsrc->length * vsrc->elementSize);
    
    vdest->status = vectorStatus_success;
}

void vector_pushCopy(vector* vdest, const vector* vsrc) {
    if(vdest == NULL || vsrc == NULL || internal_vector_errorFound(vdest) || internal_vector_errorFound(vsrc)) {
	vdest->status = vectorStatus_error_operation;
	return;
    }
    
    if(vdest->elementSize != vsrc->elementSize) {
	vdest->status = vectorStatus_error_incompatibleTypes;
	return;
    }

    if(vector_availableSpace(vdest) < vsrc->length) {
	vector_reserve(vdest, vsrc->length);
    }
    
    memmove((vdest->data + vdest->length * vdest->elementSize), vsrc->data, vsrc->length * vsrc->elementSize);
    vdest->length += vsrc->length;
}

void vector_move(vector* vdest, vector* vsrc) {
    if(vdest == NULL || vsrc == NULL || internal_vector_errorFound(vdest) || internal_vector_errorFound(vsrc)) {
	vdest->status = vsrc->status = vectorStatus_error_operation;
	return;
    }

    if(vdest->status != vectorStatus_freed) {
	vector_free(vdest);
    }

    vdest->data = vsrc->data;
    vsrc->data = NULL;
    vsrc->status = vectorStatus_freed;

    vdest->length = vsrc->length;
    vdest->capacity = vsrc->capacity;
    vdest->elementSize = vsrc->elementSize;
    vdest->status = vectorStatus_success;

    vector_free(vsrc);
}

void vector_swap(vector* v1, vector* v2) {
    if(v1 == NULL || v2 == NULL) {
	v1->status = v2->status = vectorStatus_error_null;
	return;
    }

    internal_gswap(&v1->length, &v2->length, sizeof(size_t));
    internal_gswap(&v1->capacity, &v2->capacity, sizeof(size_t));
    internal_gswap(&v1->elementSize, &v2->elementSize, sizeof(size_t));

    void* temp = v1->data;
    v1->data = v2->data;
    v2->data = temp;
    
    v1->status = v2->status = vectorStatus_success;
}

vectorStatus vector_status_code(const vector* v) {
    return v->status;
}

char* vector_status_msg(const vector* v) {
    switch(v->status) {
	case vectorStatus_error_init:
	    return "VECTOR STATUS MSG(-6): Failed to initialize the vector!\n";
	case vectorStatus_error_operation:
	    return "VECTOR STATUS MSG(-5): Failed to perform a vector function operation.\n";
	case vectorStatus_error_resize:
	    return "VECTOR STATUS MSG(-4): Failed to resize the vector!\n";
	case vectorStatus_error_elementDoesntExist:
	    return "VECTOR STATUS MSG(-3): Vector element does not exist!\n";
	case vectorStatus_error_incompatibleTypes:
	    return "VECTOR STATUS MSG(-2): Incompatible vector types: cannot push one onto another!\n";
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

void* vector_getHead(const vector* v) {
    return v->data;
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
    memmove(internal_vector_offset(v, index), item, v->elementSize);
}

static bool internal_vector_checkIndexBounds(vector* v, const size_t index) {
    if(index < v->length) {
	return true;
    }
    
    v->status = vectorStatus_error_operation;
    return false;
}

static bool internal_vector_errorFound(const vector* v) {
    if(v == NULL || v->data == NULL || v->elementSize == 0) {
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








