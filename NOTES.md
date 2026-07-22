so like I've basically finished the vm.h because i dont think we really need any changes yet, you might notice how basically everything you've done is erased (not actually erased because its still in main.c but just not used in the new code) and thats because when i started actually writting the header i realized how poinless half of the ideas were and we could simplify it

so now the system is way simpler, ive just added 3 types that were needed for some language features that couldnt be done without the VM supporting it.

> a `VAL_POINTER` is a memory address (and because the constants table is also in memory it could technically be pointing to there but idk thats a maybe)

a `VAL_REGREF` is an index of a register which might be useful idk but it can be used to modify a string in the current frame in another register (because string are immutable you cant do that with pointers, ill explain a bit later in the notes)

> a `VAL_NATIVEPTR` is a void* (a pointer which can hold a pointer to any type basically just a way to reference any data you want which is why its used in things such as malloc) its ONLY used in native functions (hence the name "native pointer") and the reason for it is because if we need to hold some random data that isn't a primitive (something the VM already has as a type) like a FILE* returned from fopen when we need files and stuff

# bytecode operations
ive also implemented most of the needed bytecode instructions but because you might not know what they are im gonna explain them here and also show their arguments
a single element in the table is a single *BytecodeSize* block of data
all registers in the bytecode are just indexes into the array inside the current function frame

an opcode is the instruction
dst is a register used for return values
srcX is a register a value of which shouldn't change but can be read (X is a placeholder for a number for when we have multiple src registers)
literal ocupies the same data block as a register index but instead of using it as an index we use it as a integer literal

| instruction | arg1 | arg2 | arg3 | description |
| ----------- | ---- | ---- | ---- | ----------- |
| OP_LOADCONST | dst | literal | | i hope you already know what it does |
| OP_LOAD | dst | src | | it will look into what pointer src holds and try to get the value from the |
| OP_STORE | dst | src | | it will try to look into the pointer dst holds and put the value from src into it
| OP_MOVE | dst | src | | this literally just creates a copy of a src Value struct and puts it in dst |
| OP_PTR | dst | src | | using the new pointer value type it points to the register (its physical address not index) of src and puts that pointer in dst |
| OP_REGREF | dst | literal | | the literal is really just a register index as if it was src but its used as a literal and stored in dst with type VAL_REGREF so we have a register index as a value |
| OP_ADD | dst | src1 | src2 | adds src1 and src2 and mutates the dst value to the new value |
| OP_SUB | dst | src1 | src2 | dst = src1 - src2 |
| OP_MUL | dst | src1 | src2 | dst = src1 * src2 |
| OP_DIV | dst | src1 | src2 | dst = src1 / src2 |
| OP_MOD | dst | src1 | src2 | dst = src1 % src2 |
| OP_NEG | dst | src | | dst = -(src) |
| OP_BITAND | dst | src1 |src2 | dst = src1 & src2 |
| OP_BITOR | dst | src1 |src2 | dst = src1 \| src2 |
| OP_BITXOR | dst | src1 |src2 |dst = src1 ^ src1 |
| OP_BITNOW | dst | src | | dst = ~(src) |
| OP_SHL | dst | src1 | src2 | dst = src1 << src2 (both int?)|
| OP_SHR | dst | src1 | src2 | dst = src1 >> src2 (both should be int 90% sure) |
| OP_EQ | dst |src1 | src2 | dst = src1 == src2 |
| OP_NE | dst | src1 | src2 | dst = src1 != src2 |
| OP_LT | dst | src1 | src2 | dst = src1 < src2 |
| OP_LE | dst | src1 | src2 | dst = src1 <= src2 |
| OP_GT | dst | src1 | src2 | dst = src1 > src2 |
| OP_GE | dst | src1 | src2 | dst = src1 >= src2 |
| OP_JUMP | src | | | change pc (program counter) by src amount (int) |
| OP_JIFTRUE | src1 | src2 | | change pc by src1 if src2 is true |
| OP_JIFFALSE | src1 | src2 | | change pc by src1 if src2 is false |
| OP_CALL | src1 | | | creates a new function frame using the Function struct at index src1, creates pointer to the argCount amount of registers from the current function frame to the new one starting at 0 |
| OP_TAILCALL | src1 | | | (ill implement this later i think) this is like CALL but means that the current function doesnt do anything other than return this function (ex: "return someFunc()") so we can just reuse the current function frame for this function keeping everything unchanged too probably |
| OP_RET | | | | we pop the latest function frame from the callstack and set it as our function frame |
 OP_NATIVECALL | src1 | | | probably implement this later but there would be an array of function pointer that take the current function frame pointer and we call the function at index src1 |

# what you need to do
 ill implement the function frame stuff probably, you could try and make the instructions starting with the basic math and continuing to bitwise math and comparisons, maybe the constant loading.
 im using a flat array for bytecode instead, when youre in an instruction it would take in the VM struct as the argument and you would need to make the helpers to read the next BytecodeSize block of data from the bytecode array and increase the curentFrame pc value (just increase the pc no need for bounds checking with the function length and such)
 the instructions themselves ask for bytes so instead of always having an opcode and 3 arguments we can have `[OP_RET][OP_ADD][arg][arg][arg]` right next to each other and then in the OP_ADD code it would so something like this: 
 ```C
  void add(VM* vm) {
    Value* dst = getNextValue(vm);
    Value* src1 = getNextValue(vm);
    Value* src2 = getNextValue(vm);
    ...
  }

 ```
 and thanks to the instructions getting the next value themselves we can squish them as close as possible and not care about accidentaly skipping an instruction or reading it as an argument
