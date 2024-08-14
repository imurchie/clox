#include <stdlib.h>

#include "memory.h"


/*
  Reallocate a pointer with a specific size. If the new size
  is 0, then release the pointer.
*/
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  /*
    Some notes of the C standard library's "realloc" function:
      - if the pointer has 0 size, this is equivalent to "malloc"
      - if the new size is smaller than it was, the allocated memory is simply decreased
      - if the rnew size is larger, it will
        - add more storage after the original, if it is free
        - create a new memory block, copy the data over, and release the old block
  */
  void* result = realloc(pointer, newSize);
  if (result == NULL) exit(1);  // Oops! We've run out of allocatable memory
  return result;
}