#include <stdio.h>
#include "debug.h"

void do_debug_ast_node(ast_node *node, int depth);

void debug_token(token *t) {
  switch (t->type) {
    case TOKEN_SYMBOL:
      printf("Token: SYMBOL\t\tvalue: \"%s\"\n", t->sval);
      break;
    case TOKEN_TYPE:
      printf("Token: TYPE\t\tvalue: \"%s\"\n", debug_var_type(t->vt));
      break;
    case TOKEN_IDENTIFIER:
      printf("Token: IDENTIFIER\t\tvalue: \"%s\"\n", t->sval);
      break;
    case TOKEN_KEYWORD:
      printf("Token: KEYWORD\t\tvalue: \"%s\"\n", t->sval);
      break;
    case TOKEN_OPERATOR:
      printf("Token: OPERATOR\t\tvalue: \"%s\"\n", t->sval);
      break;
    case TOKEN_LITERAL_INTEGER:
      printf("Token: INTEGER\t\tvalue: %llu\n", t->inum);
      break;
  }
}

const char *debug_var_type(ast_node_var_type type) {
  switch (type) {
    case AST_VAR_I64:
      return "i64";
    case AST_VAR_I32:
      return "i32";
    case AST_VAR_I16:
      return "i16";
    case AST_VAR_I8:
      return "i8";
    case AST_VAR_DOUBLE:
      return "double";
    default:
      return "Unknown";
  }
}

const char *debug_value_type(ast_node_value_type type) {
  switch (type) {
    case AST_VALUE_INTEGER:
      return "INTEGER";
    case AST_VALUE_DOUBLE:
      return "DOUBLE";
    case AST_VALUE_IDENTIFIER:
      return "IDENTIFIER";
    default:
      return "Unknown";
  }
}

void debug_ast_node(ast_node *node) {
  do_debug_ast_node(node, 0);
}

void do_debug_ast_node(ast_node *node, int depth) {
  char buff[64] = {0};
  int i = 0;

  while (i < depth) {
    buff[i] = ' ';
    buff[i + 1] = ' ';
    i += 2;
  }

  switch (node->type) {
    case AST_NODE_VAR_DECL: {
      ast_node_var_decl *n = (ast_node_var_decl *) node;
      printf("%sVAR_DECL\tname: \"%s\"\ttype: \"%s\"\n", buff, n->identifier, debug_var_type(n->var_type));

      if (n->assignment != NULL) {
        do_debug_ast_node(n->assignment, depth + 1);
      }
      break;
    }
    case AST_NODE_EXPR: {
      ast_node_expr *n = (ast_node_expr *) node;
      printf("%sEXPR\n", buff);
      do_debug_ast_node(n->expr, depth + 1);
      break;
    }
    case AST_NODE_LITERAL: {
      ast_node_literal *n = (ast_node_literal *) node;
      const char *type = debug_value_type(n->value_type);

      if (n->value_type == AST_VALUE_INTEGER) {
        printf("%sLITERAL\ttype: %s\tvalue: %llu\n", buff, type, n->inum);
        break;
      } else if (n->value_type == AST_VALUE_DOUBLE) {
        printf("%sLITERAL\ttype: %s\tvalue: %f\n", buff, type, n->dnum);
        break;
      } else if (n->value_type == AST_VALUE_IDENTIFIER) {
        printf("%sLITERAL\ttype: %s\tvalue: \"%s\"\n", buff, type, n->sval);
        break;
      }
      break;
    }
    case AST_NODE_BINOP: {
      ast_node_binop *n = (ast_node_binop *) node;
      switch (n->op) {
        case AST_BINOP_ADD:
          printf("%sBINOP(ADD)\n", buff);
          break;
        case AST_BINOP_SUB:
          printf("%sBINOP(SUB)\n", buff);
          break;
        case AST_BINOP_MUL:
          printf("%sBINOP(MUL)\n", buff);
          break;
        case AST_BINOP_DIV:
          printf("%sBINOP(DIV)\n", buff);
          break;
        case AST_BINOP_MOD:
          printf("%sBINOP(MOD)\n", buff);
          break;
        case AST_BINOP_GT:
          printf("%sBINOP(GT)\n", buff);
          break;
        case AST_BINOP_LT:
          printf("%sBINOP(LT)\n", buff);
          break;
        case AST_BINOP_GTE:
          printf("%sBINOP(GTE)\n", buff);
          break;
        case AST_BINOP_LTE:
          printf("%sBINOP(LTE)\n", buff);
          break;
        case AST_BINOP_EQ:
          printf("%sBINOP(EQ)\n", buff);
          break;
        case AST_BINOP_NEQ:
          printf("%sBINOP(NEQ)\n", buff);
          break;
        case AST_BINOP_AND:
          printf("%sBINOP(AND)\n", buff);
          break;
        case AST_BINOP_OR:
          printf("%sBINOP(OR)\n", buff);
          break;
        default:
          printf("%sBINOP(Unknown %d)\n", buff, n->op);
          break;
      }

      do_debug_ast_node(n->left, depth + 1);
      do_debug_ast_node(n->right, depth + 1);
      break;
    }
    default: {
      printf("Unknown node type %d\n", node->type);
      break;
    }
  }
}