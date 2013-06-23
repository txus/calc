CC=clang
CFLAGS=-g -std=c11 -Wall -Werror `llvm-config --cflags`
LD=g++
LDFLAGS=`llvm-config --libs --cflags --ldflags core analysis executionengine jit interpreter native`

calc: calc.o lexer.o parser.o list.o interpreter.o jit.o
	$(LD) calc.o lexer.o parser.o list.o interpreter.o jit.o $(LDFLAGS) -o calc

calc.o: calc.c
	$(CC) $(CFLAGS) -c calc.c

lexer.o: lexer.c
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

list.o: list.c
	$(CC) $(CFLAGS) -c list.c

interpreter.o: interpreter.c
	$(CC) $(CFLAGS) -c interpreter.c

jit.o: jit.c
	$(CC) $(CFLAGS) -c jit.c

clean:
	-rm -rf calc.o calc
.PHONY: clean
