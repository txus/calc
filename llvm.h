#ifndef CALC_LLVM_H
#define CALC_LLVM_H

#include <llvm-c/Core.h>
#include "parser.h"

typedef struct LLVMCompiledProgram {
  LLVMModuleRef module;
  LLVMValueRef function;
} LLVMCompiledProgram;

LLVMCompiledProgram LLVM_compile(ASTNode *node);
int LLVM_execute(LLVMCompiledProgram program);
int LLVM_evaluate(ASTNode *node);

#endif
