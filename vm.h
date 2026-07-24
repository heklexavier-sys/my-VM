#ifndef CONSTRUCT_VM_H
#define CONSTRUCT_VM_H
// for xavier: (if youre too lazy to read this just search up "header guard in c")
// the reason for these 2 lines (look above the comment) and the one at the end is because we only want the file to be included once so that any definitions are copied and we dont get errors because of "redefinition" of them
// the first line is "if not defined" and the next defines it. the last line ends the first line's if (so the code in thise file only "runs" once no matter how much we use the file) 
// the reason this works in the editor when you include it in multiple files is because in the scope of the file its only used once and in the scope of the project its also used once thanks to the header guard

#include <stdint.h>

#include "_string.h"
#include "array.h"

// the size of a single element in the bytecode
typedef uint8_t BytecodeSize;
// it could technically be a different size if we have way more instructions than registers that we could need (also this makes it more clear what functions return and such)
typedef BytecodeSize RegIndex;

typedef enum {
	VAL_INT,
	VAL_DOUBLE,
	VAL_STRING,
	VAL_BOOL,
	VAL_OBJECT,
	VAL_NULL,

	VAL_POINTER,   // can point to any memory address that holds a Value
	VAL_REGREF,    // holds a register index which is used to refer to a register at that index inside the current function frame
	VAL_NATIVEPTR, // holds a void* only used by native code when a type isn't easily put into a normal Value struct such as a FILE* from the standard library
} ValueType;

typedef enum : BytecodeSize {
	OP_LOADCONST,

	// used with REGREF and POINTER types to load/store values in their pointers instead of the pointer itself (basically works like this: "int ptr = &x; *ptr = 6" changes the x instead of making ptr = 6)
	OP_LOAD,
	OP_STORE,

	// assignments
	OP_MOVE,   // makes the first register have the same Value* as the second one
	OP_PTR,    // uses the next value as a location that holds a Value*
	OP_REGREF, // creates a register reference by placing the second literal value (register index) in the first register

	// math
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_NEG,

	// bitwise math (BIT + operation done)
	OP_BITAND,
	OP_BITOR,
	OP_BITXOR,
	OP_BITNOT,
	OP_SHL, // shift left  (<<)
	OP_SHR, // shift right (>>)

	// comparisons
	OP_EQ, // ==
	OP_NE, // !=
	OP_LT, // <
	OP_LE, // <=
	OP_GT, // >
	OP_GE, // >=

	// control flow
	OP_JUMP,     // jumps a specific amount of bytes calculated at compile time
	OP_JIFTRUE,  // jump if true
	OP_JIFFALSE, // jump if false
	
	// function handling
	OP_CALL,      // initializes a new function frame and runs the function bytecode
	OP_TAILCALL,  // call which doesnt require a new function frame (high level representation: "return someFunc()" we just need to run someFunc and return its value no need for the original function anymore so we can reuse the function frame)
	OP_RET,       // pops a function frame from the call stack into the current function frame
	OP_NATIVECALL // calls a native function from an index provided
} OpCode;

// forward declaration because we need it inside of Object but Object is needed inside it
typedef struct Value Value;

typedef struct {
	// assigned when the compiler is initialized
	uint64_t typeID;
	Value fields[]; // FAM (flexible array member) its literally just an array but instead of it being a pointer to another block of memory which actually holds the data we allocate the struct with its size + how much elements we want so its one continous block of memory which is faster
} Object;

struct Value {
	ValueType type;

	// union needs to be named so we can easily move it without needing the type
	union Data {
		int64_t intVal;
		long double doubleVal;
		const str stringVal;
		bool boolVal;
		Object *objectVal;

		Value* pointerVal;
		RegIndex regrefVal;
		void *native_ptr; // not a part of Val technically because its just an edge case really
	} data;
};

typedef struct {
	uint64_t offset; // amount of bytes from the start of the bytecode array to the function
	uint64_t len;
	uint64_t regCount;
	uint64_t argCount;
} Function;

typedef struct {
	Function* func;

	Value* registers;
	uint64_t pc;
} FunctionFrame;

typedef struct {
	BytecodeSize *bytecode;
	arr functions;
	arr callstack;
	FunctionFrame* currentFrame;
	Value* constants;
} VM;

VM* VM_create(BytecodeSize bytecode, arr functions, Value* constants);
void VM_destroy(VM* vm);

#endif
