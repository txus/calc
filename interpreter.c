#include "interpreter.h"
#include "parser.h"

int interpret(ASTNode *node)
{
  switch(node->type) {
    case AST_BINARY_OP: {
      ASTBinaryOp *binary_op = (ASTBinaryOp*)node;
      int a = interpret(binary_op->lhs);
      int b = interpret(binary_op->rhs);
      switch(binary_op->op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
      }
    }
    case AST_INT: {
      return ((ASTInt*)node)->value;
    }
  }
}
