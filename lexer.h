#ifndef CALC_LEXER_H
#define CALC_LEXER_H

typedef enum token_type {
  T_NUMBER = 0,
  T_OPERATOR,
  T_LPAREN,
  T_RPAREN
} token_type;

struct token_s {
  token_type type;
  int value;
};

typedef struct token_s Token;

struct tokens_s {
  Token* start;
  unsigned int count;
};

typedef struct tokens_s Tokens;

Tokens tokenize(char const *code);
const char* Token_to_s(Token token);
const char* Tokens_to_s(Tokens tokens);

#endif
