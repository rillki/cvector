#ifndef VECTOR_H
#define VECTOR_H

/****** VECTOR ******
 * public vector functions + macros(e.g. (m)vector_create, etc...):
 *  - vector_create
 *  - vector_reserve
 *  - vector_setLength
 *  - vector_push
 *  - vector_insert
 *  - vector_pop
 *  - vector_remove
 *  - vector_shrink
 *  - vector_clear
 *  - vector_free
 *  - vector_get
 *  - vector_constGet
 *  - vector_copyArr
 *  - vector_pushArr
 *  - vector_copy
 *  - vector_pushCopy
 *  - vector_move
 *  - vector_swap
 *  - vector_status_code
 *  - vector_status_msg
 *  - vector_status_msg_print
 *  - vector_status_msg_print_error
 *  - vector_getHead
 *  - vector_length
 *  - vector_capacity
 *  - vector_availableSpace
 *  - vector_memory
 *  - vector_isEmpty
 *
 * private vector functions:
 *  - internal_vector_resize
 *  - internal_vector_offset
 *  - internal_vector_assign
 *  - internal_vector_checkIndexBounds
 *  - internal_vector_errorFound
 *  - internal_gswap
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define VECTOR_INIT_CAPACITY 4
#define VECTOR_GROWTH_RATE 2

/* ****** MACROS ****** */

#define mvector_create(v, type) vector v; vector_create(&v, sizeof(type))
#define mvector_reserve(v, num) vector_reserve(&v, num)
#define mvector_setLength(v, length) vector_setLength(&v, length)
#define mvector_push(v, item) vector_push(&v, (void*)(&item))
#define mvector_insert(v, index, item) vector_insert(&v, index, (void*)(&item))
#define mvector_pushVal(v, val, type); { type x = val; vector_push(&v, (void*)(&x)); }
#define mvector_insertVal(v, index, val, type); { type x = val; vector_insert(&v, index, (void*)(&x)); }
#define mvector_pop(v) vector_pop(&v)
#define mvector_remove(v, index) vector_remove(&v, index)
#define mvector_shrink(v) vector_shrink(&v)
#define mvector_free(v) vector_free(&v)
#define mvector_clear(v) vector_clear(&v)
#define mvector_get(v, index, type) *((type*)vector_get(&v, index))
#define mvector_constGet(v, index, type) *((type*)vector_constGet(&v, index))
#define mvector_status_code(v) vector_status_code(&v)
#define mvector_status_msg(v) vector_status_msg(&v)
#define mvector_status_msg_print(v) vector_status_msg(&v)
#define mvector_status_msg_print_error(v) vector_status_msg_print_error(&v)
#define mvector_copyArr(v, arr, length) vector_copyArr(&v, (void*)arr, length)
#define mvector_pushArr(v, arr, length) vector_pushArr(&v, (void*)arr, length)
#define mvector_copy(v, s) vector_copy(&v, &s)
#define mvector_pushCopy(v, s) vector_pushCopy(&v, &s)
#define mvector_move(v, s) vector_move(&v, &s)
#define mvector_swap(v, s) vector_swap(&v, &s)
#define mvector_getHead(v) vector_getHead(&v)
#define mvector_length(v) vector_length(&v)
#define mvector_capacity(v) vector_capacity(&v)
#define mvector_availableSpace(v) vector_availableSpace(&v)
#define mvector_memory(v) vector_memory(&v)
#define mvector_isEmpty(v) vector_isEmpty(&v)

/* ****** VECTOR STRUCTURES ****** */

// vector status
typedef enum vectorStatus {
    // if fails to initialize
    vectorStatus_error_init = -6,
   
    // if fails to execute a vector function
    vectorStatus_error_operation = -5,
    
    // if fails to resize a vector
    vectorStatus_error_resize = -4,
    
    // if vector element does not exist
    vectorStatus_error_elementDoesntExist = -3,

    // if someone tries to push vector of one type onto vector of another type: v(int).push[ v(char*) ] => incompatible
    vectorStatus_error_incompatibleTypes = -2,

    // if vector is NULL
    vectorStatus_error_null = -1,

    // operation successfull
    vectorStatus_success = 1,

    // memory freed
    vectorStatus_freed = 2,
} vectorStatus;

// vector data structure
typedef struct vector {
    size_t capacity;      // vector total capacity
    size_t length;        // vector current length
    size_t elementSize;   // element size
    vectorStatus status;  // vector status
    
    void* data;           // vector data
} vector;

/* ****** PUBLIC VECTOR METHODS ****** */

/* initializes a vector 
 *  params:
 *	vector* v          => vector instance
 *	size_t elementSize => sizeof(type)
*/
void vector_create(vector* v, const size_t elementSize);

/* reserves additional memory for number of elements
 *  params:
 *	vector* v  => vector instance
 *	size_t num => number of elements
*/
void vector_reserve(vector* v, const size_t num);

/* sets vector length to specified size
 *  params:
 *	vector* v     => vector instance
 *	size_t length => custom vector length
*/
void vector_setLength(vector* v, const size_t length);

/* push an item at the end of vector
 *  params:
 *	vector* v  => vector instance
 *	void* item => value
*/
void vector_push(vector* v, const void* item);

/* insert an item at certain index
 *  params:
 *	vector* v    => vector instance
 *	size_t index => index
 *	void* item   => value
*/
void vector_insert(vector* v, const size_t index, const void* item);

/* pop the last element (does not realloc the vector)
 *  params:
 *	vector* v => vector instance
*/
void vector_pop(vector* v);

/* remove an element at an index (does not realloc the vector)
 *  params:
 *	vector* v    => vector instance
 *	size_t index => index
*/
void vector_remove(vector* v, const size_t index);

/* shrinks the size of vector to current length+1 (reallocs the vector)
 *  params:
 *	vector* v => vector instance
*/
void vector_shrink(vector* v);

/* frees vector memory
 *  params:
 *	vector* v => vector instance
*/
void vector_free(vector* v);

/* resizes the vector to length of 0
 *  params:
 *	vector* v => vector instance
*/
void vector_clear(vector* v);

/* returns vector element at an index
 *  params:
 *	vector* v    => vector instance
 *	size_t index => index
*/
void* vector_get(vector* v, const size_t index);

/* returns const vector element at an index
 *  params: 
 *	vector* v => vector instance
 *	size_t index => index
*/
const void* vector_constGet(vector* v, const size_t index);

/* copies array data to vector (vector is resized to array length), returns vectorStatus value
 *  params:
 *	vector* v     => vector instance
 *	void* arr     => array data
 *	size_t length => array length
*/
void vector_copyArr(vector* v, const void* arr, const size_t length);

/* adds array data to vector (additional space is reserved, total: v->length + arr length), returns vectorStatus value
 *  params: 
 *	vector* v     => vector instance
 *	void* arr     => array
 *	size_t length => array length
*/
void vector_pushArr(vector* v, const void* arr, const size_t length);

/* copies source vector contents to destination: resizes destination to source vector size (does not free source vector)
 *  params:
 *	vector* vdest => destination vector where to copy data
 *	vector* vsrc  => source vector to copy data from
*/
void vector_copy(vector* vdest, const vector* vsrc);

/* pushes source vector contents to destination vector (does not free source vector)
 *  params:
 *	vector* vdest => destination vector where to copy data
 *	vector* vsrc  => source vector to copy data from
*/
void vector_pushCopy(vector* vdest, const vector* vsrc);

/* moves source vector contents to destination vector (frees source vector)
 *  params:
 *	vector* vdest => destination vector where to move data
 *	vector* vsrc  => source vector
*/
void vector_move(vector* vdest, vector* vsrc);

/* swaps two vectors 
 *  params:
 *	vector* v1
 *	vector* v2
*/
void vector_swap(vector* v1, vector* v2);

/* returns a vector status code (check out vectorStatus enum)
 *  params:
 *	vector* v => vector instance
*/
vectorStatus vector_status_code(const vector* v);

/* returns a vector status message of vector status code
 *  params:
 *	vector* v => vector instance
*/
char* vector_status_msg(const vector* v);

/* prints vector status message to stderr
 *  params:
 *	vector* v => vector instance
*/
void vector_status_msg_print(const vector* v);

/* prints vector error message only to stderr
 *  params:
 *	vector* v => vector instance
*/
void vector_status_msg_print_error(const vector* v);

/* returns a void pointer to the begining of vector data
 *  params:
 *	vector* v => vector instance
*/
void* vector_getHead(const vector* v);

/* returns vector length
 *  params:
 *	vector* v => vector instance
*/
size_t vector_length(const vector* v);

/* returns vector capacity
 *  params:
 *	vector* v => vector instance
*/
size_t vector_capacity(const vector* v);

/* returns vector available space (capacity - length)
 *  params:
 *	vector* v => vector instance
*/
size_t vector_availableSpace(const vector* v);

/* returns allocated memory in bytes
 *  params:
 *	vector* v => vector instance
*/
size_t vector_memory(const vector* v);

/* returns true if vector is empty
 *  params:
 *	vector* v => vector instance
*/
bool vector_isEmpty(const vector* v);

/* ****** PRIVATE VECTOR METHODS FOR INTERNAL USE ONLY ****** */

/* resizes the vector
 *  params:
 *	vector* v   => vector instance
 *	size_t size => new vector length
*/
static void internal_vector_resize(vector* v, const size_t size);

/* return vector offset
 *  params:
 *	vector* v => vector instance
 *	size_t index => index
*/
static void* internal_vector_offset(const vector* v, const size_t index);

/* assigns data at index 
 *  params:
 *	vector* v => vector instance
 *	size_t index => index
 *	void* item => element
*/
static void internal_vector_assign(vector* v, const size_t index, const void* item);

/* checks if index is within vector bounds 0..length-1
 *  params:
 *	vector* v    => vector instance
 *	size_t index => index
*/
static bool internal_vector_checkIndexBounds(vector *v, const size_t index);

/* checks vector for errors conditions 
 *  params:
 *	const vector* v => vector instance
*/
static bool internal_vector_errorFound(const vector* v);

/* generic swap
 *  params:
 *	void* a, void* b  => elements to swap
 *	const size_t size => number of bytes to swap
*/
static void internal_gswap(void* a, void* b, const size_t size);

#endif // VECTOR_H









