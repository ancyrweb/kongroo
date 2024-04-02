#ifndef KONGROO_COMPILER_H
#define KONGROO_COMPILER_H

#include "globals.h"

compile_process *compiler_process_create(code_source *source);

void compiler_run(compile_process *process);

#endif //KONGROO_COMPILER_H
