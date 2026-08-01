#ifndef VM_INSTRUCTION_H
#define VM_INSTRUCTION_H

#include <math.h>
#include <string.h>
#include <stdbool.h>
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

inline void VMload(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);

    Value src2 = *src1.data.pointerVal;

    dst->type = src2.type;
    dst->data = src2.data;
}

//shoulda started with these cuz they're so easy
inline void VMmove(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src = *VMgetNextReg(vm);

    dst->type = src.type;
    dst->data = src.data;
}

inline void VMptr(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value* src = VMgetNextReg(vm);

    dst->type = VAL_POINTER;
    dst->data.pointerVal = src;
}

inline void VMstore(VM* vm) {
    Value* dst = VMgetNextReg(vm);   // 1. Get the register holding the pointer
    Value src = *VMgetNextReg(vm);   // 2. Get the register holding the data we want to store

    // 3 & 4. Overwrite the Value struct located at the pointer destination
    // with the contents of src.
    *dst->data.pointerVal = src;
}

inline void VMregRef(VM* vm) {
    Value* dst = VMgetNextReg(vm);

    BytecodeSize regIndex = VMgetNextVal(vm);

    dst->type = VAL_REGREF;
    dst->data.regrefVal = regIndex;
}




inline void VMadd(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        long double dub1 = makeDouble(src1);
        long double dub2 = makeDouble(src2);
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
        long double dub1 = makeDouble(src1);
        long double dub2 = makeDouble(src2);
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
        long double dub1 = makeDouble(src1);
        long double dub2 = makeDouble(src2);
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

    long double dub1 = makeDouble(src1);
    long double dub2 = makeDouble(src2);
    dst->type = VAL_DOUBLE;
    dst->data.doubleVal = dub1 / dub2;
}

inline void VMmod(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        long double dub1 = makeDouble(src1);
        long double dub2 = makeDouble(src2);
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

inline void VMbitand(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_INT;
    dst->data.intVal = src1.data.intVal & src2.data.intVal;
}

inline void VMbitor(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_INT;
    dst->data.intVal = src1.data.intVal | src2.data.intVal;
}

inline void VMbitxor(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_INT;
    dst->data.intVal = src1.data.intVal ^ src2.data.intVal;
}

inline void VMbitnot(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src = *VMgetNextReg(vm);

    dst->type = VAL_INT;
    dst->data.intVal = ~(src.data.intVal);
}

inline void VMbitshl(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_INT;
    dst->data.intVal = src1.data.intVal << src2.data.intVal;
}

inline void VMbitshr(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_INT;
    dst->data.intVal = src1.data.intVal >> src2.data.intVal;
}

inline void VMeq(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_BOOL;

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        dst->data.boolVal = makeDouble(src1) == makeDouble(src2);
        return;
    }

    if (src1.type != src2.type) {
        dst->data.boolVal = false;
        return;
    }

    switch (src1.type) {
        case VAL_INT:
            dst->data.boolVal =
                src1.data.intVal == src2.data.intVal;
            break;

        case VAL_BOOL:
            dst->data.boolVal =
                src1.data.boolVal == src2.data.boolVal;
            break;

        case VAL_STRING:
            dst->data.boolVal =
                strcmp(src1.data.stringVal, src2.data.stringVal) == 0;
            break;

        case VAL_OBJECT:
            dst->data.boolVal =
                src1.data.objectVal == src2.data.objectVal;
            break;

        default:
            dst->data.boolVal = false;
            break;
    }
}

inline void VMne(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_BOOL;

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        dst->data.boolVal = makeDouble(src1) != makeDouble(src2);
        return;
    }

    if (src1.type != src2.type) {
        dst->data.boolVal = true;
        return;
    }

    switch (src1.type) {
        case VAL_INT:
            dst->data.boolVal =
                src1.data.intVal != src2.data.intVal;
            break;

        case VAL_BOOL:
            dst->data.boolVal =
                src1.data.boolVal != src2.data.boolVal;
            break;

        case VAL_STRING:
            dst->data.boolVal =
                strcmp(src1.data.stringVal, src2.data.stringVal) != 0;
            break;

        case VAL_OBJECT:
            dst->data.boolVal =
                src1.data.objectVal != src2.data.objectVal;
            break;

        default:
            dst->data.boolVal = true;
            break;
    }
}

inline void VMlt(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_BOOL;

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        long double dub1 = makeDouble(src1);
        long double dub2 = makeDouble(src2);
        dst->data.boolVal = (dub1 < dub2);
    } else {

        dst->data.boolVal = (src1.data.intVal < src2.data.intVal);
    }
}

inline void VMle(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_BOOL;

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        long double dub1 = makeDouble(src1);
        long double dub2 = makeDouble(src2);
        dst->data.boolVal = (dub1 <= dub2);
    } else {

        dst->data.boolVal = (src1.data.intVal <= src2.data.intVal);
    }
}

inline void VMgt(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_BOOL;

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        long double dub1 = makeDouble(src1);
        long double dub2 = makeDouble(src2);
        dst->data.boolVal = (dub1 > dub2);
    } else {

        dst->data.boolVal = (src1.data.intVal > src2.data.intVal);
    }
}

inline void VMge(VM* vm) {
    Value* dst = VMgetNextReg(vm);
    Value src1 = *VMgetNextReg(vm);
    Value src2 = *VMgetNextReg(vm);

    dst->type = VAL_BOOL;

    if (src1.type == VAL_DOUBLE || src2.type == VAL_DOUBLE) {
        long double dub1 = makeDouble(src1);
        long double dub2 = makeDouble(src2);
        dst->data.boolVal = (dub1 >= dub2);
    } else {

        dst->data.boolVal = (src1.data.intVal >= src2.data.intVal);
    }
}

//changing pc allows the code so skip forward, like if an if statement fails
inline void VMjump(VM* vm) {
    Value src = *VMgetNextReg(vm);
    vm->currentFrame->pc += src.data.intVal;
}

inline void VMjumpIfTrue(VM* vm) {
    Value src = *VMgetNextReg(vm);
    Value decider = *VMgetNextReg(vm);

    if (decider.data.boolVal) {
        vm->currentFrame->pc += src.data.intVal;
    }
}

inline void VMjumpIfFalse(VM* vm) {
    Value src = *VMgetNextReg(vm);
    Value decider = *VMgetNextReg(vm);

    if (!decider.data.boolVal) {
        vm->currentFrame->pc += src.data.intVal;
    }
}

#endif
