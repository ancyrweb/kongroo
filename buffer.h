#ifndef KONGROO_BUFFER_H
#define KONGROO_BUFFER_H

#include <stddef.h>

typedef struct buffer {
  char *data;
  size_t size;
  size_t capacity;
} buffer;

buffer *buffer_create();

void buffer_append(buffer *b, char c);

void buffer_free(buffer *b);

char *buffer_ptr(buffer *b);

#endif //KONGROO_BUFFER_H
