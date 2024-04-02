#include "compiler.h"
#include "bytecode.h"
#include "buffer.h"
#include "codesource.h"
#include "ast.h"
#include "debug.h"
#include <stdlib.h>
#include <printf.h>
#include <string.h>
#include <stdbool.h>

#define S_EQ(str, str2) \
  (str && str2 && (strcmp(str, str2) == 0))

#define SYMBOL_CASE \
  case '{':         \
  case '}':         \
  case ':':         \
  case ';':         \
  case '#':          \
  case '(':         \
  case ')':         \
  case '[':         \
  case ']'

#define OPERATOR_CASE \
    case '+':                            \
    case '-':                            \
    case '*':                            \
    case '/':                             \
    case '%':                            \
    case '<':                            \
    case '>':                            \
    case '^':                            \
    case '!':                            \
    case '=':                            \
    case '~':                            \
    case '|':                            \
    case '&':                            \
    case ',':                            \
    case '.':                            \
    case '?'

#define NUMERIC_CASE \
    case '0':        \
    case '1':        \
    case '2':        \
    case '3':        \
    case '4':        \
    case '5':        \
    case '6':        \
    case '7':        \
    case '8':        \
    case '9'


static char peekc(compile_process *process) {
  return process->source->buffer[process->pos];
}

static char advance(compile_process *process) {
  return process->source->buffer[process->pos++];
}

static bool is_eof(compile_process *process) {
  return process->pos >= process->source->size;
}

static bool is_keyword(const char *str) {
  return S_EQ(str, "var");
}

static bool is_type(const char *str) {
  return S_EQ(str, "i64") || S_EQ(str, "i32") || S_EQ(str, "i16") || S_EQ(str, "i8");
}

static ast_node_var_type str_to_vt(const char *str) {
  if (S_EQ(str, "i64")) return AST_VAR_I64;
  if (S_EQ(str, "i32")) return AST_VAR_I32;
  if (S_EQ(str, "i16")) return AST_VAR_I16;
  if (S_EQ(str, "i8")) return AST_VAR_I8;

  fprintf(stderr, "Unknown type: %s\n", str);
  exit(1);
}

static token *token_create(token *source) {
  token *t = calloc(1, sizeof(token));
  memcpy(t, source, sizeof(token));

  t->line = source->line;
  t->col = source->col;

  return t;
}

static token *read_symbol_token(compile_process *process) {
  char c = advance(process);
  return token_create(&(token) {
      .type = TOKEN_SYMBOL,
      .sval = strdup((char[]) {c, 0x00})
  });
}

static token *read_operator_token(compile_process *process) {
  buffer *buf = buffer_create();
  char c = advance(process);

  buffer_append(buf, c);

  if (
      c == '+' || c == '-' || c == '*' || c == '/' ||
      c == '%' || c == '!' || c == '=' || c == '<' ||
      c == '>') {
    if (peekc(process) == '=') {
      buffer_append(buf, advance(process));
    }
  }

  if (c == '&' || c == '|') {
    if (peekc(process) == c) {
      buffer_append(buf, advance(process));
    }
  }

  buffer_append(buf, 0x00);

  return token_create(&(token) {
      .type = TOKEN_OPERATOR,
      .sval = strdup(buffer_ptr(buf))
  });
}


static token *read_special_token(compile_process *process) {
  buffer *buf = buffer_create();
  char c = peekc(process);
  while (isalpha(c) || isdigit(c) || c == '_') {
    buffer_append(buf, advance(process));
    c = peekc(process);
  }

  buffer_append(buf, 0x00);

  const char *str = buffer_ptr(buf);

  if (is_keyword(str)) {
    return token_create(&(token) {
        .type = TOKEN_KEYWORD,
        .sval = strdup(str)
    });
  } else if (is_type(str)) {
    return token_create(&(token) {
        .type = TOKEN_TYPE,
        .vt = str_to_vt(str),
    });
  } else {
    return token_create(&(token) {
        .type = TOKEN_IDENTIFIER,
        .sval = strdup(str)
    });
  }
}

static token *read_numeric_token(compile_process *process) {
  buffer *buf = buffer_create();
  char c = peekc(process);

  while (isdigit(c)) {
    buffer_append(buf, advance(process));
    c = peekc(process);
  }

  buffer_append(buf, 0x00);

  return token_create(&(token) {
      .type = TOKEN_LITERAL_INTEGER,
      .inum = atoi(buffer_ptr(buf))
  });
}

static void skip_whitespace(compile_process *process) {
  char c = peekc(process);

  while ((c == ' ' || c == '\n' || c == '\t')) {
    if (c == '\n') {
      process->line++;
      process->col = 1;
    } else {
      process->col++;
    }

    advance(process);

    if (is_eof(process)) {
      return;
    }

    c = peekc(process);
  }
}

static token *compiler_next_token(compile_process *process) {
  skip_whitespace(process);

  if (is_eof(process)) {
    return NULL;
  }

  token *t = NULL;
  char c = peekc(process);

  switch (c) {
    SYMBOL_CASE: {
      t = read_symbol_token(process);
      break;
    }
    OPERATOR_CASE: {
      t = read_operator_token(process);
      break;
    }
    NUMERIC_CASE: {
      t = read_numeric_token(process);
      break;
    }
    default: {
      t = read_special_token(process);
      break;
    }
  }

  if (t == NULL) {
    fprintf(stderr, "Unknown character: %c\n", c);
    exit(1);
  }

  return t;
}

token *compiler_lex(compile_process *process) {
  token **tokens = malloc(sizeof(token) * 1024);

  int i = 0;
  while (!is_eof(process)) {
    tokens[i++] = compiler_next_token(process);
  }


  process->tokens = tokens;
  process->tokens_size = i;
}

void compiler_run(compile_process *process) {
  compiler_lex(process);
  ast_parse(process);
  vector_push(process->bytecode, OP_HALT);
}

compile_process *compiler_process_create(code_source *source) {
  compile_process *process = calloc(1, sizeof(compile_process));
  process->source = source;
  process->tokens = NULL;
  process->tokens_size = 0;
  process->tokens_pos = 0;
  process->nodes = NULL;
  process->nodes_size = 0;
  process->bytecode = vector_create();
  process->pos = 0;
  process->line = 1;
  process->col = 1;

  return process;
}
