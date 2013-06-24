CC=clang
CFLAGS=-g -std=c11 -Wall -Werror `llvm-config --cflags`
LD=g++
LDFLAGS=`llvm-config --libs --cflags --ldflags core analysis executionengine jit interpreter native`

calc: calc.o lexer.o parser.o list.o interpreter.o llvm.o
	$(LD) calc.o lexer.o parser.o list.o interpreter.o llvm.o $(LDFLAGS) -o calc

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

llvm.o: llvm.c
	$(CC) $(CFLAGS) -c llvm.c

clean:
	-rm -rf calc *.o
.PHONY: clean
