#ifndef KONGROO_CODESOURCE_H
#define KONGROO_CODESOURCE_H

#include <stdint.h>

typedef struct {
  const char *filename;
  const char *buffer;
  u_int64_t size;
} code_source;

code_source *code_source_from_file(const char *filename);

#endif //KONGROO_CODESOURCE_H
