#include <lightning.h>
#include <sys/mman.h>
#include "jit.h"
#include "dbg.h"

static jit_insn *codeBuffer;
typedef int (*pifi)();    /* Pointer to Int Function of Nothing */

#define MAX_REG 5

static inline int alloc_reg(int regs[]) {
  for(int i = 0; i < MAX_REG; i++) {
    if (!regs[i]) {
      regs[i] = 1;
      switch(i) {
      case 0: return JIT_R0;
      case 1: return JIT_R1;
      case 2: return JIT_R2;
      case 3: return JIT_V0;
      case 4: return JIT_V1;
      case 5: return JIT_V2;
      }
    }
  }
  return -1;
}

static inline void free_reg(int regs[], int reg) {
  switch(reg) {
    case JIT_R0: regs[0] = 0; break;
    case JIT_R1: regs[1] = 0; break;
    case JIT_R2: regs[2] = 0; break;
    case JIT_V0: regs[3] = 0; break;
    case JIT_V1: regs[4] = 0; break;
    case JIT_V2: regs[5] = 0; break;
  }
}

static inline void JIT_visit(ASTNode *node, int regs[], int reg) {
  switch(node->type) {
    case AST_BINARY_OP: {
      ASTBinaryOp *binary_op = (ASTBinaryOp*)node;

      int a_reg = alloc_reg(regs);
      JIT_visit(binary_op->lhs, regs, a_reg);
      int b_reg = alloc_reg(regs);
      JIT_visit(binary_op->rhs, regs, b_reg);

      switch(binary_op->op) {
        case '+': jit_addr_i(reg, a_reg, b_reg); break;
        case '-': jit_subr_i(reg, a_reg, b_reg); break;
        case '*': jit_mulr_i(reg, a_reg, b_reg); break;
        case '/': jit_divr_i(reg, a_reg, b_reg); break;
      }

      free_reg(regs, a_reg);
      free_reg(regs, b_reg);
      break;
    }
    case AST_INT: {
      jit_movi_i(reg, ((ASTInt*)node)->value);
      break;
    }
  }
}

int JIT_evaluate(ASTNode *root)
{
#ifdef __APPLE__
  // Mac OS X hack
  codeBuffer = mmap (NULL, 4096, PROT_EXEC | PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANON, -1, 0);
#endif

  pifi function = (pifi) (jit_set_ip(codeBuffer).iptr);

  jit_leaf(1);
  int regs[MAX_REG] = {0};
  int ret = alloc_reg(regs);
  JIT_visit(root, regs, ret);
  jit_ret();

  jit_flush_code(codeBuffer, jit_get_ip().ptr);

  return function();
}
