#ifndef VM_INSTRUCTION_H
#define VM_INSTRUCTION_H

#include <math.h>
#include "vm.h"

inline BytecodeSize VMgetNextVal(VM* vm) {
    return vm->bytecode[vm->currentFrame->func->offset + vm->currentFrame->pc++];
}

inline Value* VMgetReg(VM* vm, BytecodeSize index) {
    return &vm->currentFrame->registers[index];
}

inline Value* VMgetNextReg(VM* vm) {
    return VMgetReg(vm, VMgetNextVal(vm));
}

inline long double makeDouble(Value src) {
    double dub = (src.type == VAL_DOUBLE) ? src.data.doubleVal : (double)src.data.intVal;
    return dub;
};


inline void VMloadconst(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src = vm->constants[VMgetNextVal(vm)];
    dst->type = src.type;
    dst->data = src.data;
}

inline void VMadd(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        double dub1 = makeDouble(src1);
        double dub2 = makeDouble(src2);
        dst->type = VAL_DOUBLE;
        dst->data.doubleVal = dub1 + dub2;
    } else {
        dst->type = VAL_INT;
        dst->data.intVal = src1.data.intVal + src2.data.intVal;
    }
}

inline void VMsub(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        double dub1 = makeDouble(src1);
        double dub2 = makeDouble(src2);
        dst->type = VAL_DOUBLE;
        dst->data.doubleVal = dub1 - dub2;
    } else {
        dst->type = VAL_INT;
        dst->data.intVal = src1.data.intVal - src2.data.intVal;
    }
}

inline void VMmul(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        double dub1 = makeDouble(src1);
        double dub2 = makeDouble(src2);
        dst->type = VAL_DOUBLE;
        dst->data.doubleVal = dub1 * dub2;
    } else {
        dst->type = VAL_INT;
        dst->data.intVal = src1.data.intVal * src2.data.intVal;
    }
}

inline void VMdiv(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    double dub1 = makeDouble(src1);
    double dub2 = makeDouble(src2);
    dst->type = VAL_DOUBLE;
    dst->data.doubleVal = dub1 / dub2;
}

inline void VMmod(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        double dub1 = makeDouble(src1);
        double dub2 = makeDouble(src2);
        dst->type = VAL_DOUBLE;
        dst->data.doubleVal = fmod(dub1, dub2);
    } else {
        dst->type = VAL_INT;
        dst->data.intVal = src1.data.intVal % src2.data.intVal;
    }
}

inline void VMneg(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src = *VMgetNextReg(vm);

    dst->type = src.type;

    if (src.type == VAL_DOUBLE) {
        dst->data.doubleVal = -src.data.doubleVal;
    } else {
        dst->data.intVal = -src.data.intVal;
    }
}


#endif