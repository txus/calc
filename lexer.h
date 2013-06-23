#ifndef CALC_LEXER_H
#define CALC_LEXER_H

enum TOKEN_TYPE {
  T_NUMBER = 0,
  T_OPERATOR,
  T_LPAREN,
  T_RPAREN
};

struct token_s {
  enum TOKEN_TYPE type;
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
