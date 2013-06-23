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

#define ADD_TOKEN(A, ...) tokens[token_count] = (Token){ .type = (A), ##__VA_ARGS__ }; token_count++;

Tokens tokenize(const char *code) {
  Token tokens[200];
  unsigned int token_count = 0;

  lexer_state state = LEX_NORMAL;

  char *ch = (char*)code;
  ch--;
  char current_number[20] = {'\0'};
  int current_number_idx = 0;
  while(ch && state != LEX_END) {
    char next = *(ch + 1);
    switch(state) {
      case LEX_NORMAL: {
        if(isdigit(next)) {
          memset(&current_number, 0, 20);
          current_number_idx= 0;
          state = LEX_NUMBER;
        } else if(isspace(next)) {
          // skip
        } else {
          switch(next) {
            case '+':
            case '-':
            case '*':
            case '/': ADD_TOKEN(T_OPERATOR, .value = next); break;
            case '(': ADD_TOKEN(T_LPAREN); break;
            case ')': ADD_TOKEN(T_RPAREN); break;
          }
        }
        ch++;
        break;
      }
      case LEX_NUMBER: {
        current_number[current_number_idx] = *ch;
        if(isdigit(next)) {
          current_number_idx++;
          ch++;
        } else {
          ADD_TOKEN(T_NUMBER, .value = atoi(current_number));
          state = LEX_NORMAL;
        }
        break;
      }
      case LEX_END: break;
    }
    if(!next) state = LEX_END;
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
