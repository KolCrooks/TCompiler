// Copyright 2019 Justin Hu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file is part of the T Language Compiler.

// implementation of pretty printer for fragments and IR

#include "ir/printer.h"

#include "ir/ir.h"
#include "translate/translate.h"
#include "util/internalError.h"

#include <stdio.h>

static char const *allocHintToString(AllocHint ah) {
  switch (ah) {
    case AH_GP: {
      return "GP";
    }
    case AH_SSE: {
      return "SSE";
    }
    case AH_MEM: {
      return "MEM";
    }
    default: { error(__FILE__, __LINE__, "invalid AllocHint enum"); }
  }
}

static void irOperandPrint(IROperand *operand) {
  switch (operand->kind) {
    case OK_TEMP: {
      printf("TEMP(%zu, %zu, %zu, %s)", operand->data.temp.n,
             operand->data.temp.size, operand->data.temp.alignment,
             allocHintToString(operand->data.temp.kind));
      break;
    }
    case OK_REG: {
      printf("REG(%zu)", operand->data.reg.n);
      break;
    }
    case OK_CONSTANT: {
      printf("CONST(0x%lX)", operand->data.constant.bits);
      break;
    }
    case OK_NAME: {
      printf("NAME(%s)", operand->data.name.name);
      break;
    }
    case OK_ASM: {
      printf("ASM(%s)", operand->data.assembly.assembly);
      break;
    }
    case OK_STRING: {
      printf("STRING(%s)", (char *)operand->data.string.data);
      break;
    }
    case OK_WSTRING: {
      printf("WSTRING(%ls)", (wchar_t *)operand->data.wstring.data);
      break;
    }
    case OK_STACKOFFSET: {
      printf("OFFSET(%ld)", operand->data.stackOffset.stackOffset);
      break;
    }
  }
}

static void irEntryVectorPrint(IREntryVector const *ir) {
  for (size_t idx = 0; idx < ir->size; idx++) {
    IREntry *entry = ir->elements[idx];
    switch (entry->op) {
      case IO_CONST: {
        printf("CONST(%zu, ", entry->opSize);
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_ASM: {
        printf("ASM(");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_LABEL: {
        printf("LABEL(");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_MOVE: {
        printf("MOVE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_MEM_STORE: {
        printf("MEM_STORE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_MEM_LOAD: {
        printf("MEM_LOAD(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_STK_STORE: {
        printf("STK_STORE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_STK_LOAD: {
        printf("STK_LOAD(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_OFFSET_STORE: {
        printf("OFFSET_STORE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_OFFSET_LOAD: {
        printf("OFFSET_LOAD(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_ADD: {
        printf("ADD(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_ADD: {
        printf("FP_ADD(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_SUB: {
        printf("SUB(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_SUB: {
        printf("FP_SUB(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_SMUL: {
        printf("SMUL(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_UMUL: {
        printf("UMUL(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_MUL: {
        printf("FP_MUL(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_SDIV: {
        printf("SDIV(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_UDIV: {
        printf("UDIV(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_DIV: {
        printf("FP_DIV(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_SMOD: {
        printf("SMOD(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_UMOD: {
        printf("UMOD(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }

      case IO_SLL: {
        printf("SLL(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_SLR: {
        printf("SLR(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_SAR: {
        printf("SAR(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_AND: {
        printf("AND(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_XOR: {
        printf("XOR(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_OR: {
        printf("OR(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_L: {
        printf("L(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_LE: {
        printf("LE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_E: {
        printf("E(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_NE: {
        printf("NE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_GE: {
        printf("GE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_G: {
        printf("G(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_A: {
        printf("A(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_AE: {
        printf("AE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_B: {
        printf("B(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_BE: {
        printf("BE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_L: {
        printf("FP_L(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_LE: {
        printf("FP_LE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_E: {
        printf("FP_E(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_NE: {
        printf("FP_NE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_GE: {
        printf("FP_GE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_G: {
        printf("FP_G(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_NEG: {
        printf("NEG(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_FP_NEG: {
        printf("FP_NEG(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_LNOT: {
        printf("LNOT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_NOT: {
        printf("NOT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_SX_SHORT: {
        printf("SX_SHORT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_SX_INT: {
        printf("SX_INT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_SX_LONG: {
        printf("SX_LONG(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_ZX_SHORT: {
        printf("ZX_SHORT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_ZX_INT: {
        printf("ZX_INT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_ZX_LONG: {
        printf("ZX_LONG(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_TRUNC_BYTE: {
        printf("TRUNC_BYTE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_TRUNC_SHORT: {
        printf("TRUNC_SHORT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_TRUNC_INT: {
        printf("TRUNC_INT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_F_TO_BYTE: {
        printf("F_TO_BYTE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_F_TO_SHORT: {
        printf("F_TO_SHORT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_F_TO_INT: {
        printf("F_TO_INT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_F_TO_LONG: {
        printf("F_TO_LONG(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_U_TO_FLOAT: {
        printf("U_TO_FLOAT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_U_TO_DOUBLE: {
        printf("U_TO_DOUBLE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_S_TO_FLOAT: {
        printf("S_TO_FLOAT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_S_TO_DOUBLE: {
        printf("S_TO_DOUBLE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_F_TO_FLOAT: {
        printf("F_TO_FLOAT(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_F_TO_DOUBLE: {
        printf("F_TO_DOUBLE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_JUMP: {
        printf("JUMP(");
        irOperandPrint(entry->dest);
        printf(")\n");
        break;
      }
      case IO_JL: {
        printf("JL(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_JLE: {
        printf("JLE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_JE: {
        printf("JE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_JNE: {
        printf("JNE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_JGE: {
        printf("JGE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_JG: {
        printf("JG(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_JA: {
        printf("JA(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_JAE: {
        printf("JAE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_JB: {
        printf("JB(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_JBE: {
        printf("JBE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_JL: {
        printf("FP_JL(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_JLE: {
        printf("FP_JLE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_JE: {
        printf("FP_JE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_JNE: {
        printf("FP_JNE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_JGE: {
        printf("FP_JGE(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_FP_JG: {
        printf("FP_JG(%zu, ", entry->opSize);
        irOperandPrint(entry->dest);
        printf(", ");
        irOperandPrint(entry->arg1);
        printf(", ");
        irOperandPrint(entry->arg2);
        printf(")\n");
        break;
      }
      case IO_CALL: {
        printf("CALL(");
        irOperandPrint(entry->arg1);
        printf(")\n");
        break;
      }
      case IO_RETURN: {
        printf("RETURN()\n");
        break;
      }
    }
  }
}

void fragmentPrint(Fragment const *f) {
  printf("%s: ", f->label);
  switch (f->kind) {
    case FK_BSS: {
      printf("bss, aligned to %zu bytes, size %zu\n", f->data.bss.alignment,
             f->data.bss.size);
      break;
    }
    case FK_DATA: {
      printf("data, aligned to %zu bytes:\n", f->data.data.alignment);
      irEntryVectorPrint(f->data.data.ir);
      break;
    }
    case FK_RODATA: {
      printf("read-only data, aligned to %zu bytes:\n",
             f->data.rodata.alignment);
      irEntryVectorPrint(f->data.rodata.ir);
      break;
    }
    case FK_TEXT: {
      printf("function body:\n");
      irEntryVectorPrint(f->data.text.ir);
      break;
    }
  }
}