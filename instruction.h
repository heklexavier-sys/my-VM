#ifndef VM_INSTRUCTION_H
#define VM_INSTRUCTION_H

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

inline void VMloadconst(VM* vm) {
	Value* dst = VMgetNextReg(vm);
	Value src = vm->constants[VMgetNextVal(vm)];
	dst->type = src.type;
	dst->data = src.data;
}

#endif
