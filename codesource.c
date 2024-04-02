
#include "codesource.h"
#include <stdlib.h>
#include <stdio.h>

code_source *code_source_from_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc(sizeof(char) * (size + 1));
  fread(buffer, 1, size, file);
  buffer[size] = 0x00;

  fclose(file);

  code_source *source = malloc(sizeof(code_source));
  source->filename = filename;
  source->buffer = buffer;
  source->size = size;

  return source;
}