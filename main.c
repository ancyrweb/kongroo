#include <stdio.h>
#include "virtual-machine.h"
#include "codesource.h"
#include "compiler.h"

int main() {
  code_source *source = code_source_from_file("../app.ekg");
  if (!source) {
    printf("Failed to read file\n");
    return 1;
  }

  compile_process *process = compiler_process_create(source);
  if (!process) {
    printf("Failed to create compile process\n");
    return 1;
  }

  compiler_run(process);
  vm_run(process);

  return 0;
}
