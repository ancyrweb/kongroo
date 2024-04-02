#include "buffer.h"

#include <stdlib.h>

buffer *buffer_create() {
  buffer *b = malloc(sizeof(buffer));
  b->data = malloc(sizeof(char) * 8);
  b->size = 0;
  b->capacity = 8;

  return b;
}

void buffer_append(buffer *b, char c) {
  if (b->size >= b->capacity) {
    b->capacity *= 2;
    b->data = realloc(b->data, b->capacity * sizeof(char));
  }

  b->data[b->size] = c;
  b->size++;
}

void buffer_free(buffer *b) {
  free(b->data);
  free(b);
}

char *buffer_ptr(buffer *b) {
  return b->data;
}