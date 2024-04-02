
#ifndef KONGROO_STACK_H
#define KONGROO_STACK_H

#include <stdint.h>

typedef struct {
  int64_t values[256];
  int head;
  int capacity;
} stack;

stack* stack_create();
void stack_free(stack* s);
int64_t stack_pop(stack* s);
void stack_push(stack* s, int64_t value);
int64_t stack_peek(stack* s);

#endif //KONGROO_STACK_H
