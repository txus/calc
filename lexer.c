#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "lexer.h"
#include "dbg.h"

static inline Token* allocate_tokens(Token tokens[], unsigned int count)
{
  Token* allocated = calloc(count, sizeof(Token));
  memcpy(allocated, tokens, count * sizeof(Token));

  return allocated;
}

#define EMIT(A, ...) tokens[token_count] = (Token){ .type = (A), ##__VA_ARGS__ }; token_count++;

Tokens tokenize(const char *code) {
  Token tokens[200];
  unsigned int token_count = 0;

  char *ptr = (char*)code;

  while(*ptr) {
    while(*ptr == ' ') ptr++;
    int number;
    int consumed;
    if (sscanf(ptr, "%d%n", &number, &consumed) == 1) {
      EMIT(T_NUMBER, .value = number);
      ptr += consumed;
    } else {
      switch(*ptr) {
        case '+':
        case '-':
        case '*':
        case '/': EMIT(T_OPERATOR, .value = *ptr); break;
        case '(': EMIT(T_LPAREN); break;
        case ')': EMIT(T_RPAREN); break;
      }
      ptr++;
    }
  }

  return (Tokens) {
    .start = allocate_tokens(tokens, token_count),
    .count = token_count
  };
}

const char* Token_to_s(Token token) {
  switch(token.type) {
    case T_NUMBER: {
      char* str = calloc(50, sizeof(char));
      sprintf(str, "T_NUMBER(%i)", token.value);
      return (const char*)str;
      break;
    }
    case T_OPERATOR: {
      char* str = calloc(50, sizeof(char));
      sprintf(str, "T_OPERATOR(%c)", token.value);
      return (const char*)str;
      break;
    }
    case T_LPAREN: return "T_LPAREN";
    case T_RPAREN: return "T_RPAREN";
  }
}

const char* Tokens_to_s(Tokens tokens) {
  char *str = calloc(55 * tokens.count, sizeof(char));
  strcat(str, "[");
  for(int i = 0; i < tokens.count; i++) {
    Token token = tokens.start[i];
    strcat(str, Token_to_s(token));
    if(i != tokens.count) strcat(str, ", ");
  }
  strcat(str, "]");
  return (const char*)str;
}
