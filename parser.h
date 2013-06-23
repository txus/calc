#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include "lexer.h"

typedef enum {
  AST_BINARY_OP,
  AST_INT
} ast_node_type;

typedef struct ASTNode {
  ast_node_type type;
} ASTNode;

typedef struct ASTBinaryOp {
  ASTNode base;
  ASTNode *lhs;
  ASTNode *rhs;
  char op;
} ASTBinaryOp;

typedef struct ASTInt {
  ASTNode base;
  int value;
} ASTInt;

ASTBinaryOp* ASTBinaryOp_new(ASTNode *lhs, ASTNode *rhs, char op);
ASTInt* ASTInt_new(int value);

const char* ASTNode_to_sexp(ASTNode *node);
ASTNode* parse(Tokens tokens);

#endif
