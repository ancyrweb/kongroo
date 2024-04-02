#ifndef KONGROO_VECTOR_H
#define KONGROO_VECTOR_H

#include <stdint.h>
#include <stddef.h>

typedef struct vector {
  u_int64_t *data;
  u_int64_t size;
  u_int64_t capacity;
} vector;

vector *vector_create();

void vector_push(vector *v, u_int64_t item);

u_int64_t vector_at(vector *v, u_int64_t index);

void vector_free(vector *v);

#endif //KONGROO_VECTOR_H
