#include <stdlib.h>
#include "ast.h"
#include "debug.h"
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define T_TYPE_EQ(a, b) (a->type == b)
#define IS_OP(a) (strcmp(peek(process)->sval, (a)) == 0)
#define S_EQ(a, b) (strcmp(a, b) == 0)

#define PARSE_AND_ASSIGN_BINOP(parse_func) do { \
  token *op = advance(process); \
  ast_node_binop *binop = malloc(sizeof(ast_node_binop)); \
  binop->type = AST_NODE_BINOP; \
  binop->op = ast_translate_binop(op->sval); \
  binop->left = node; \
  binop->right = parse_func(process); \
  node = binop; \
} while(0)

// Operator precedence (highest to lowest)
// Terms (+, -)
// Factors (*, /, %)
// Relation Operators (<, >, <=, >=)
// Equality Operators (==, !=)
// AND (&&)
// OR (||)

static token *peek(compile_process *process) {
  return process->tokens[process->tokens_pos];
}

static token *advance(compile_process *process) {
  return process->tokens[process->tokens_pos++];
}

static bool is_eof(compile_process *process) {
  return process->tokens_pos >= process->tokens_size;
}

static ast_binop_type ast_translate_binop(const char *op) {
  if (S_EQ(op, "+")) return AST_BINOP_ADD;
  if (S_EQ(op, "-")) return AST_BINOP_SUB;
  if (S_EQ(op, "*")) return AST_BINOP_MUL;
  if (S_EQ(op, "/")) return AST_BINOP_DIV;
  if (S_EQ(op, "%")) return AST_BINOP_MOD;
  if (S_EQ(op, ">")) return AST_BINOP_GT;
  if (S_EQ(op, "<")) return AST_BINOP_LT;
  if (S_EQ(op, ">=")) return AST_BINOP_GTE;
  if (S_EQ(op, "<=")) return AST_BINOP_LTE;
  if (S_EQ(op, "==")) return AST_BINOP_EQ;
  if (S_EQ(op, "!=")) return AST_BINOP_NEQ;
  if (S_EQ(op, "&&")) return AST_BINOP_AND;
  if (S_EQ(op, "||")) return AST_BINOP_OR;

  fprintf(stderr, "Unknown operator %s\n", op);
}

static void ast_match_semicolon(compile_process *process) {
  token *token = advance(process);

  if (!(T_TYPE_EQ(token, TOKEN_SYMBOL) && token->sval[0] == ';')) {
    fprintf(stderr, "Expected semicolon, got %s\n", token->sval);
    exit(1);
  }
}

static void ast_match_symbol(compile_process *process, const char *symbol) {
  token *token = advance(process);

  if (!(T_TYPE_EQ(token, TOKEN_SYMBOL) && S_EQ(token->sval, symbol))) {
    fprintf(stderr, "Expected symbol %s, got %s\n", symbol, token->sval);
    exit(1);
  }
}

static void ast_match_keyword(compile_process *process, const char *keyword) {
  token *token = advance(process);

  if (!(T_TYPE_EQ(token, TOKEN_KEYWORD) && S_EQ(token->sval, keyword))) {
    fprintf(stderr, "Expected keyword %s, got %s\n", keyword, token->sval);
    exit(1);
  }
}

static token *ast_match_type(compile_process *process) {
  token *token = advance(process);

  if (!(T_TYPE_EQ(token, TOKEN_TYPE))) {
    fprintf(stderr, "Expected type, got %s\n", token->sval);
    exit(1);
  }

  return token;
}

static token *ast_match_identifier(compile_process *process) {
  token *token = advance(process);

  if (!(T_TYPE_EQ(token, TOKEN_IDENTIFIER))) {
    fprintf(stderr, "Expected identifier, got %u\n", token->type);
    exit(1);
  }

  return token;
}

static ast_node *ast_parse_expr_literal(compile_process *process) {
  token *token = advance(process);
  if (T_TYPE_EQ(token, TOKEN_LITERAL_INTEGER)) {
    ast_node_literal *node = malloc(sizeof(ast_node_literal));
    node->type = AST_NODE_LITERAL;
    node->value_type = AST_VALUE_INTEGER;
    node->inum = token->inum;
    return (ast_node *) node;
  } else if (T_TYPE_EQ(token, TOKEN_LITERAL_DECIMAL)) {
    ast_node_literal *node = malloc(sizeof(ast_node_literal));
    node->type = AST_NODE_LITERAL;
    node->value_type = AST_VALUE_DOUBLE;
    node->dnum = token->dnum;
    return (ast_node *) node;
  } else if (T_TYPE_EQ(token, TOKEN_IDENTIFIER)) {
    ast_node_literal *node = malloc(sizeof(ast_node_literal));
    node->type = AST_NODE_LITERAL;
    node->value_type = AST_VALUE_IDENTIFIER;
    node->sval = token->sval;
    return (ast_node *) node;
  }

  fprintf(stderr, "Unexpected token");
  exit(1);
}

static ast_node *ast_parse_expr_factor(compile_process *process) {
  ast_node *node = ast_parse_expr_literal(process);
  while (peek(process)->type == TOKEN_OPERATOR && (IS_OP("*") || IS_OP("/") || IS_OP("%"))) {
    PARSE_AND_ASSIGN_BINOP(ast_parse_expr_literal);
  }

  return node;
}

static ast_node *ast_parse_expr_terms(compile_process *process) {
  ast_node *node = ast_parse_expr_factor(process);
  while (peek(process)->type == TOKEN_OPERATOR && (IS_OP("+") || IS_OP("-"))) {
    PARSE_AND_ASSIGN_BINOP(ast_parse_expr_factor);
  }

  return node;
}

static ast_node *ast_parse_expr_relations(compile_process *process) {
  ast_node *node = ast_parse_expr_terms(process);
  while (peek(process)->type == TOKEN_OPERATOR && (IS_OP("<") || IS_OP("<=") || IS_OP(">") || IS_OP(">="))) {
    PARSE_AND_ASSIGN_BINOP(ast_parse_expr_terms);
  }

  return node;
}

static ast_node *ast_parse_expr_equality(compile_process *process) {
  ast_node *node = ast_parse_expr_relations(process);
  while (peek(process)->type == TOKEN_OPERATOR && (IS_OP("==") || IS_OP("!="))) {
    PARSE_AND_ASSIGN_BINOP(ast_parse_expr_relations);
  }

  return node;
}

static ast_node *ast_parse_expr_and(compile_process *process) {
  ast_node *node = ast_parse_expr_equality(process);
  while (peek(process)->type == TOKEN_OPERATOR && (IS_OP("&&"))) {
    PARSE_AND_ASSIGN_BINOP(ast_parse_expr_equality);
  }

  return node;
}

static ast_node *ast_parse_expr_or(compile_process *process) {
  ast_node *node = ast_parse_expr_and(process);
  while (peek(process)->type == TOKEN_OPERATOR && (IS_OP("||"))) {
    PARSE_AND_ASSIGN_BINOP(ast_parse_expr_and);
  }

  return node;
}

static ast_node *ast_parse_expr(compile_process *process) {
  ast_node *e = ast_parse_expr_or(process);

  ast_node_expr *node = malloc(sizeof(ast_node_expr));
  node->type = AST_NODE_EXPR;
  node->expr = e;

  return (ast_node *) node;
}

static ast_node *ast_parse_expr_statement(compile_process *process) {
  ast_node *node = ast_parse_expr(process);
  ast_match_semicolon(process);

  return node;
}

static ast_node *ast_parse_var_declaration(compile_process *process) {
  ast_match_keyword(process, "var");
  token *identifier = ast_match_identifier(process);
  ast_match_symbol(process, ":");
  token *type = ast_match_type(process);
  ast_node *expr = NULL;

  if (peek(process)->sval[0] == '=') {
    advance(process);
    expr = ast_parse_expr(process);
  }

  ast_match_semicolon(process);

  ast_node_var_decl *node = malloc(sizeof(ast_node_var_decl));
  node->type = AST_NODE_VAR_DECL;
  node->var_type = type->vt;
  node->identifier = identifier->sval;
  node->assignment = (ast_node_expr *) expr;


  return (ast_node *) node;
}

void ast_parse(compile_process *process) {
  process->nodes = malloc(sizeof(ast_node *) * 1024);
  process->nodes_size = 0;
  process->tokens_pos = 0;

  while (!is_eof(process)) {
    token *token = peek(process);
    ast_node *node = NULL;

    if (T_TYPE_EQ(token, TOKEN_LITERAL_INTEGER)) {
      node = ast_parse_expr_statement(process);
    } else if (T_TYPE_EQ(token, TOKEN_KEYWORD)) {
      if (S_EQ(token->sval, "var")) {
        node = ast_parse_var_declaration(process);
      }
    }

    if (node == NULL) {
      fprintf(stderr, "Unexpected token");
      exit(1);
    }

    debug_ast_node(node);
    printf("\n");
  }
}