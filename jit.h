#ifndef CALC_JIT_H
#define CALC_JIT_H

#include <llvm-c/Core.h>
#include "parser.h"

typedef struct JITCompiledProgram {
  LLVMModuleRef module;
  LLVMValueRef function;
} JITCompiledProgram;

JITCompiledProgram JIT_compile(ASTNode *node);
int JIT_execute(JITCompiledProgram program);
int JIT_evaluate(ASTNode *node);

#endif
