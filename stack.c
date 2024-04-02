#include "stack.h"
#include <stdlib.h>

stack* stack_create() {
  stack* s = malloc(sizeof(stack));
  s->head = 0;
  s->capacity = 256;
  return s;
}

void stack_free(stack* s) {
  free(s);
}

int64_t stack_pop(stack* s) {
  if (s->head == 0) {
    return 0;
  }

  return s->values[--s->head];
}

void stack_push(stack* s, int64_t value) {
  s->values[s->head++] = value;
}

int64_t stack_peek(stack* s) {
  return s->values[s->head - 1];
}