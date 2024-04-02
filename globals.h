#ifndef KONGROO_GLOBALS_H
#define KONGROO_GLOBALS_H

#include "codesource.h"
#include "vector.h"

typedef enum ast_node_type {
  AST_NODE_EXPR,
  AST_NODE_BINOP,
  AST_NODE_LITERAL,
  AST_NODE_VAR_DECL,
} ast_node_type;

typedef enum ast_binop_type {
  AST_BINOP_ADD,
  AST_BINOP_SUB,
  AST_BINOP_MUL,
  AST_BINOP_DIV,
  AST_BINOP_MOD,

  AST_BINOP_GT,
  AST_BINOP_LT,
  AST_BINOP_GTE,
  AST_BINOP_LTE,
  AST_BINOP_EQ,
  AST_BINOP_NEQ,

  AST_BINOP_AND,
  AST_BINOP_OR
} ast_binop_type;

typedef enum ast_node_var_type {
  AST_VAR_I64,
  AST_VAR_I32,
  AST_VAR_I16,
  AST_VAR_I8,
  AST_VAR_DOUBLE
} ast_node_var_type;

typedef enum ast_node_value_type {
  AST_VALUE_INTEGER,
  AST_VALUE_DOUBLE,
  AST_VALUE_IDENTIFIER
} ast_node_value_type;

typedef enum token_type {
  TOKEN_SYMBOL,
  TOKEN_TYPE,
  TOKEN_OPERATOR,
  TOKEN_IDENTIFIER,
  TOKEN_KEYWORD,
  TOKEN_LITERAL_INTEGER,
  TOKEN_LITERAL_DECIMAL,
} token_type;

typedef struct token {
  int64_t line;
  int64_t col;
  token_type type;

  union {
    char *sval;
    int64_t inum;
    double dnum;
    ast_node_var_type vt;
  };
} token;

typedef struct ast_node {
  ast_node_type type;
} ast_node;

typedef struct ast_node_expr {
  ast_node_type type;
  ast_node *expr;
} ast_node_expr;

typedef struct ast_node_binop {
  ast_node_type type;
  ast_binop_type op;
  ast_node *left;
  ast_node *right;
} ast_node_binop;

typedef struct ast_node_literal {
  ast_node_type type;
  ast_node_value_type value_type;
  union {
    int64_t inum;
    double dnum;
    char *sval;
  };
} ast_node_literal;

typedef struct ast_node_var_decl {
  ast_node_type type;
  ast_node_var_type var_type;
  const char *identifier;
  ast_node_expr *assignment; // NULL if no assignment
} ast_node_var_decl;

typedef struct compile_process {
  code_source *source;
  vector *bytecode;

  token **tokens;
  size_t tokens_size;
  size_t tokens_pos;

  ast_node **nodes;
  size_t nodes_size;

  u_int64_t pos;
  u_int64_t line;
  u_int64_t col;
} compile_process;


#endif //KONGROO_GLOBALS_H
