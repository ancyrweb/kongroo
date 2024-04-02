#ifndef KONGROO_DEBUG_H
#define KONGROO_DEBUG_H

#include <stdio.h>
#include "compiler.h"

void debug_token(token *t);

const char *debug_var_type(ast_node_var_type type);

const char *debug_value_type(ast_node_value_type type);

void debug_ast_node(ast_node *node);

#endif //KONGROO_DEBUG_H
