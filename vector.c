#include "vector.h"

#include <stdlib.h>
#include <string.h>

#define VECTOR_BASE_CAPACITY 32

vector *vector_create() {
  vector *v = malloc(sizeof(vector));
  v->data = malloc(sizeof(u_int64_t) * VECTOR_BASE_CAPACITY);
  v->size = 0;
  v->capacity = VECTOR_BASE_CAPACITY;

  return v;
}

void vector_push(vector *v, u_int64_t item) {
  if (v->size >= v->capacity) {
    v->capacity *= 2;
    v->data = realloc(v->data, v->capacity * sizeof(u_int64_t));
  }

  u_int64_t index = v->size * sizeof(u_int64_t);

  memcpy(
      v->data + index,
      &item,
      sizeof(u_int64_t)
  );

  v->size++;
}

u_int64_t vector_at(vector *v, u_int64_t index) {
  return v->data[index];
}

void vector_free(vector *v) {
  free(v->data);
  free(v);
}