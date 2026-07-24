#include "vm.h"
#include "array.h"
#include "heapmem.h"

#include <stdio.h>

inline Value* VMAllocRegs(uint64_t regcount) {
	Value* regs = heap_malloc(sizeof(Value) * regcount);
	// idk if i want to initialize them just in case (prob not because all values are initialized when constants are loaded or registers are used in instructions)
	return regs;
}

inline FunctionFrame* VMAllocFunctionFrame(Function* func) {
	FunctionFrame* ff = heap_malloc(sizeof(FunctionFrame));
	ff->func = func;
	ff->registers = VMAllocRegs(func->regCount);
	ff->pc = 0;
	return ff;
}

inline void VMinitFunctionFrame(FunctionFrame* currentfunc, FunctionFrame* newfunc) {
	for (uint64_t i = 0; i < newfunc->func->argCount; i++) {
		newfunc->registers[i] = (Value){.type = VAL_POINTER, .data.pointerVal = &currentfunc->registers[i + currentfunc->func->argCount]};
	}
}

inline Value* VMgetValue(Value* reg) {
	Value* currentVal = reg;
decode_start:
	if (currentVal->type == VAL_POINTER)
		currentVal = currentVal->data.pointerVal;
	else if (currentVal->type == VAL_REGREF)
		printf("why are you using this bs bruh");
	else
		return currentVal;
goto decode_start;
}


