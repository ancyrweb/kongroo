//
// Created by Anthony Cyrille on 31/03/2024.
//

#include "virtual-machine.h"

#include <stdio.h>
#include "bytecode.h"
#include "stack.h"

#define CASE_BINOP(enuval, op) \
  case enuval: { \
    u_int64_t a = stack_pop(s); \
    u_int64_t b = stack_pop(s); \
    stack_push(s, a op b); \
    break; \
  }

void vm_run(compile_process *process) {
  int ip = 0;
  int halted = 0;
  stack *s = stack_create();

  while (halted == 0) {
    int64_t opcode = (int64_t) vector_at(process->bytecode, ip++);

    switch (opcode) {
      case OP_PUSH: {
        int64_t value = (int64_t) vector_at(process->bytecode, ip++);
        stack_push(s, value);
        break;
      }
      CASE_BINOP(OP_ADD, +)
      CASE_BINOP(OP_SUB, -)
      CASE_BINOP(OP_MUL, *)
      CASE_BINOP(OP_DIV, /)
      CASE_BINOP(OP_MOD, %)
      CASE_BINOP(OP_XOR, ^)
      CASE_BINOP(OP_AND, &)
      CASE_BINOP(OP_OR, |)
      case OP_PRINT: {
        printf("%llu\n", stack_peek(s));
        break;
      }
      case OP_HALT: {
        halted = 1;
        break;
      }
      default: {
        printf("Unknown opcode: %ld\n", opcode);
        break;
      }
    }
  }

  stack_free(s);
}
