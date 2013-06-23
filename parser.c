#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "parser.h"

// operators
// precedence   operators       associativity
// 3            * / %           left to right
// 2            + -             left to right
static inline int precedence(Token *token)
{
  switch(token->value) {
    case '*':  case '/': case '%':
      return 3;
    case '+': case '-':
      return 2;
  }
  return 0;
}

#define PARSE_ERROR(A, ...) printf(A "\n", ##__VA_ARGS__); abort()

static inline ASTNode* token_to_ast(Token *token, Stack *output) {
  switch(token->type) {
    case T_NUMBER: {
      return (ASTNode*) ASTInt_new(token->value);
    }
    case T_OPERATOR: {
      Token* lhs_tok = (Token*)Stack_pop(output);
      ASTNode *lhs = token_to_ast(lhs_tok, output);
      Token* rhs_tok = (Token*)Stack_pop(output);
      ASTNode *rhs = token_to_ast(rhs_tok, output);
      return (ASTNode*) ASTBinaryOp_new(lhs, rhs, token->value);
    }
    default: {
      PARSE_ERROR("No AST conversion for token type: %i", token->type);
    }
  }
}

ASTBinaryOp* ASTBinaryOp_new(ASTNode *lhs, ASTNode *rhs, char op) {
  ASTBinaryOp *node = calloc(1, sizeof(ASTBinaryOp));
  node->base = (ASTNode){ .type = AST_BINARY_OP };
  node->lhs = lhs;
  node->rhs = rhs;
  node->op = op;
  return node;
}

ASTInt* ASTInt_new(int value) {
  ASTInt *node = calloc(1, sizeof(ASTInt));
  node->base = (ASTNode){ .type = AST_INT };
  node->value = value;
  return node;
}

const char* ASTNode_to_sexp(ASTNode *node) {
  switch(node->type) {
    case AST_INT: {
      char *str = malloc(50);
      sprintf(str, "%i", ((ASTInt*)node)->value);
      return (const char*)str;
    }
    case AST_BINARY_OP: {
      ASTBinaryOp *binary_op = (ASTBinaryOp*)node;
      const char *lhs = ASTNode_to_sexp(binary_op->lhs);
      const char *rhs = ASTNode_to_sexp(binary_op->rhs);
      char *buf = malloc(5 + sizeof(lhs) + sizeof(rhs));
      sprintf(buf, "(%c ", binary_op->op);

      strcat(buf, lhs);
      strcat(buf, " ");
      strcat(buf, rhs);
      strcat(buf, ")");

      return buf;
    }
  }
}

ASTNode* parse(Tokens tokens) {
  Stack* output = Stack_create();
  Stack* operators = Stack_create();

  for(int i = 0; i < tokens.count; i++) {
    Token *token = malloc(sizeof(Token));
    token->type = tokens.start[i].type;
    token->value = tokens.start[i].value;

    switch(token->type) {
      case T_NUMBER: Stack_push(output, token); break;
      case T_OPERATOR: {
        while(Stack_peek(operators) &&
              precedence(token) <= precedence((Token*)Stack_peek(operators))) {
          Stack_push(output, Stack_pop(operators));
        }
        Stack_push(operators, token);
        break;
      }
      case T_LPAREN: Stack_push(operators, token); break;
      case T_RPAREN: {
        while(Stack_peek(operators) &&
              ((Token*)Stack_peek(operators))->type != T_LPAREN) {
          Stack_push(output, Stack_pop(operators));
        }
        if(Stack_peek(operators)) {
          Stack_pop(operators);
        } else {
          PARSE_ERROR("Mismatched parentheses.");
        }
        break;
      }
    }
  }

  Token *tos;
  while((tos = Stack_peek(operators))) {
    if(tos->type == T_LPAREN || tos->type == T_RPAREN) {
      PARSE_ERROR("Mismatched parentheses.");
    }
    Stack_push(output, Stack_pop(operators));
  }

  return token_to_ast((Token*)Stack_pop(output), output);
}
