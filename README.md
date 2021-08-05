# CVector
A simple to use generic Vector library for C programming language.

---
### Naming
* Every vector operation starts with the word `vector`:
```C
vector_doSomething(...)
``` 
* Vector does contain macros as well to facilitate clean code. Every macro has the same name as the analogous vector function, but with a prefix `m`:
```C
mvector_doSometing(...)
```

### Usage 
This section can be subdivided into three parts:
1. Vector creation and allocation
2. Vector data manipulation
3. Vector error-checking

#### Vector creation and allocation
Using macros, we get:
```C
mvector_create(v, int);

mvector_setLength(v, 12); 	// sets vector length to 12
mvector_reserve(v, 100);	// preallocates 100 elements for future use (length is 12)

mvector_shrink(v);	// shrinks vector's capacity to length + 1 = 13

mvector_free(v);	// frees the vector

```
And this will expand to the following code:
```C
vector v; 
vector_create(&v, sizeof(int));

vector_setLength(&v, 12); 	// sets vector length to 12
vector_reserve(&v, 100); 	// preallocates 100 elements for future use (length is 12)

vector_shrink(&v);	// shrinks vector's capacity to length + 1 = 13

vector_free(&v); 	// frees the vector 

```

#### Vector data manipulation
Using macros, we get:
```C
int num = 25;
mvector_push(v, num);				// push 25 at the end
mvector_pushVal(v, 13, int); 		// push 13 at the end

mvector_insert(v, 1, num);			// insert 25 at index 1
mvector_insertVal(v, 0, 13, int);	// insert 13 at index 0

mvector_pop(v);			// pop the last element
mvector_remove(v, 1);	// remove element at index 1

mvector_clear(v); 		// resize the vector to length of 0

```
And this will expand to the following code:
```C
int num = 25;
vector_push(&v, (void*)(&num))			// push 25 at the end
{
	int x = 13; 
	vector_push(&v, (void*)(&x)); 		// push 13 at the end
}

vector_insert(&v, 1, (void*)(&num));	// insert 25 at index 1
{ 
	int x = 13; 
	vector_insert(&v, 0, (void*)(&x)); 	// insert 13 at index 0
}

vector_pop(&v);			// pop the last element
vector_remove(&v, 1);	// remove element at index 1

vector_clear(&v); 		// resize the vector to length of 0

```

#### Vector error-checking
Vector provides a way to retrieve the status for every vector operation. The status can be queried through special `vector_status_code` that returns a `vectorStatus` value. Alternatively, `vector_msg_*` functions can be used.

##### Vector status codes
`vectorStatus` is an enum that contains the following list of status codes:
```C
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
vectorStatus_freed = 2

```

##### Onto error handling
Using macros, we get:
```C
vectorStatus vstatus = mvector_status_code(v); // get the latest code
if(vstatus != vectorStatus_success) {
	printf("%s\n", mvector_status_msg(v));

	// error handling
	// ...
}

// you can also print the status msg right a way
mvector_status_msg_print(v);

// alternatively, you can print only the error messages, ignoring anything else
mvector_status_msg_print_error(v);

```
And this further expands to:
```C
vectorStatus vstatus = vector_status_code(&v); // get the latest code
if(vstatus != vectorStatus_success) {
	printf("%s\n", vector_status_msg(&v));

	// error handling
	// ...
}

// you can also print the status msg right a way
vector_status_msg_print(&v);

// alternatively, you can print only the error messages, ignoring anything else
vector_status_msg_print_error(&v);

```

To check out more functionality or to read the documentation on what each function does, see the [vector.h](https://github.com/rillki/cvector/blob/master/vector/vector.h) file.

### LICENSE
This project is released under the MIT License. For more information, see the LICENSE file.





