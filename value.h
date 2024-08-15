#ifndef clox_value_h
#define clox_value_h

#include "common.h"

// abstract how values are handled, so code using them
// do not need to change when the representation changes
typedef double Value;

// constant pool type
typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif