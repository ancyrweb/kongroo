#ifndef KONGROO_BYTECODE_H
#define KONGROO_BYTECODE_H

enum bytecode_op {
  OP_PUSH,
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_MOD,
  OP_XOR,
  OP_AND,
  OP_OR,
  OP_PRINT,
  OP_HALT
};

#endif //KONGROO_BYTECODE_H
