// Headers required by LLVM
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <stdio.h>
#include <stdlib.h>
#include "llvm.h"

static inline LLVMValueRef LLVM_visit(ASTNode *node, LLVMBuilderRef builder) {
  switch(node->type) {
    case AST_BINARY_OP: {
      ASTBinaryOp *binary_op = (ASTBinaryOp*)node;
      LLVMValueRef a = LLVM_visit(binary_op->lhs, builder);
      LLVMValueRef b = LLVM_visit(binary_op->rhs, builder);
      switch(binary_op->op) {
        case '+': return LLVMBuildAdd(builder, a, b, "a + b");
        case '-': return LLVMBuildSub(builder, a, b, "a - b");
        case '*': return LLVMBuildMul(builder, a, b, "a * b");
        case '/': return LLVMBuildSDiv(builder, a, b, "a / b");
      }
    }
    case AST_INT: {
      return LLVMConstInt(LLVMInt32Type(), ((ASTInt*)node)->value, 0);
    }
  }
}

LLVMCompiledProgram LLVM_compile(ASTNode *node)
{
  char *error = NULL; // Used to retrieve messages from functions
  LLVMLinkInJIT();
  LLVMInitializeNativeTarget();
  LLVMModuleRef mod = LLVMModuleCreateWithName("calc_module");
  LLVMTypeRef program_args[] = { };
  LLVMValueRef program = LLVMAddFunction(mod, "program", LLVMFunctionType(LLVMInt32Type(), program_args, 0, 0));
  LLVMSetFunctionCallConv(program, LLVMCCallConv);

  LLVMBuilderRef builder = LLVMCreateBuilder();
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(program, "entry");

  LLVMPositionBuilderAtEnd(builder, entry);

  LLVMValueRef res = LLVM_visit(node, builder);

  LLVMBuildRet(builder, res);
  LLVMVerifyModule(mod, LLVMAbortProcessAction, &error);
  LLVMDisposeMessage(error); // Handler == LLVMAbortProcessAction -> No need to check errors

  LLVMDisposeBuilder(builder);

  return (LLVMCompiledProgram) { .module = mod, .function = program };
}

int LLVM_execute(LLVMCompiledProgram program)
{
  LLVMModuleRef module = program.module;
  LLVMValueRef function = program.function;

  char *error = NULL; // Used to retrieve messages from functions

  LLVMExecutionEngineRef engine;
  LLVMModuleProviderRef provider = LLVMCreateModuleProviderForExistingModule(module);
  error = NULL;
  if(LLVMCreateJITCompiler(&engine, provider, 2, &error) != 0) {
    fprintf(stderr, "%s\n", error);
    LLVMDisposeMessage(error);
    abort();
  }

  LLVMPassManagerRef pass = LLVMCreatePassManager();
  LLVMAddTargetData(LLVMGetExecutionEngineTargetData(engine), pass);
  LLVMAddConstantPropagationPass(pass);
  LLVMAddInstructionCombiningPass(pass);
  LLVMAddPromoteMemoryToRegisterPass(pass);
  LLVMAddGVNPass(pass);
  LLVMAddCFGSimplificationPass(pass);
  LLVMRunPassManager(pass, module);
#ifdef NDEBUG
#else
  LLVMDumpModule(module);
#endif

  LLVMGenericValueRef exec_args[] = {};
  LLVMGenericValueRef exec_res = LLVMRunFunction(engine, function, 0, exec_args);

  int result = LLVMGenericValueToInt(exec_res, 0);

  LLVMDisposePassManager(pass);
  LLVMDisposeExecutionEngine(engine);

  return result;
}

int LLVM_evaluate(ASTNode *node)
{
  LLVMCompiledProgram program = LLVM_compile(node);
  return LLVM_execute(program);
}
