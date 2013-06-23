#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "jit.h"
#include "dbg.h"

#define CALC_VERSION "0.0.1"

ASTNode* code_to_ast(char const *code)
{
  debug("Program: %s", code);

  Tokens tokens = tokenize(code);

  debug("Tokens: %s", Tokens_to_s(tokens));

  ASTNode *node = parse(tokens);
  debug("AST: %s", ASTNode_to_sexp(node));
  return node;
}

void print_usage_banner() {
  printf("Calc %s\n", CALC_VERSION);
  printf("\tUsage: ./calc [--jit] \"2 * (4 + 5)\"\n");
}

int main (int argc, char const *argv[])
{
  if(argc < 2) {
    fprintf(stderr, "Usage: ./calc \"3 + 3\"\n");
    return -1;
  }
  int jit = 0;
  const char *code;
  for(int i=0; i < argc; i++) {
    if(strcmp(argv[i], "--jit") == 0 ||
       strcmp(argv[i], "-j") == 0) {
      jit = 1;
    } else if(strcmp(argv[i], "--help") == 0 ||
              strcmp(argv[i], "-h") == 0) {
      print_usage_banner();
      return 0;
    } else {
      code = argv[i];
    }
  }

  ASTNode *root = code_to_ast(code);

  int result;
  if(jit) {
    debug("MODE: JIT");
    printf("Error: JIT mode not supported yet.\n");
    return 0;
    result = jit_compile(root);
  } else {
    debug("MODE: AST-Walking interpreter");
    result = interpret(root);
  }
  printf("RESULT: %i\n", result);

  return 0;
}
