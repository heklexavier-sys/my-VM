//
// Created by hekle on 2026-07-09.
//
// /what is an object? something that you can hold in your hand? Something that is individual? something with multiple properties? Which

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_MAX 256

// --- TYPES & ENUMS ---

//for determining which register - small int is better
typedef uint8_t RegIndex;

typedef enum
{
    OBJ_INT,
    OBJ_DOUBLE,
    OBJ_STRING,
    OBJ_BOOL,
    OBJ_NULL
} ValueType;


typedef enum {
    OP_LOAD,
    OP_PUSH,
    OP_MOVE,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_JUMP
} OperationCode;

typedef enum {
    MATH_ADD,
    MATH_SUB,
    MATH_MUL,
    MATH_DIV,
} MathType;

// --- STRUCTS ---

//this is the arena, it askes for a large amount of memory instead of malloc, which does smaller amounts
typedef struct {
    uint8_t *buffer;   // The block of memory
    size_t capacity;   // Total size of the block
    size_t offset;     // How much used so far
} Arena;

typedef struct Value {
    ValueType type;

    union {
        int32_t intValue;
        double doubleValue;
        const char *stringValue;
        bool boolValue;
    };
} Value;

typedef struct {
    OperationCode opcode;
    RegIndex a;
    RegIndex b;
    RegIndex c;
}Intstructions;

//MY REGISTERS
//makes the size of the registers equal to the size of RegIndex
//
typedef struct {
    Value *registers[(size_t)(RegIndex)~(RegIndex)0 + 1];
    Arena objectArena;

    Value *stack[STACK_MAX]; // The physical pile of plates
    int sp;                  // How many plates are currently in the pile
} VMallMemory;
//VERY IMPORTANT



//DUMMY CONSTANT TABLE
Value constants[] =
{
    { .type = OBJ_INT,    .intValue = 5 },
    { .type = OBJ_INT,    .intValue = 10 },
    { .type = OBJ_DOUBLE,  .doubleValue = 3.14159f },
    { .type = OBJ_STRING, .stringValue = "Hello, World!" },
    { .type = OBJ_BOOL,   .boolValue = true },
    { .type = OBJ_BOOL,   .boolValue = false },
    { .type = OBJ_NULL }
};
//DUMMY CONSTANT TABLE END


// --- MEMORY / ARENA FUNCTIONS ---

void initArena(Arena *arena, size_t size) {
    arena->buffer = malloc(size);
    if (arena->buffer == NULL) {
        printf("Failed to allocate arena!\n");
        exit(1);
    }
    arena->capacity = size;
    arena->offset = 0;
}

void* arenaAlloc(Arena *arena, size_t size) {
    // Memory alignment (keeps things perfectly packed in 8-byte increments)
    size_t aligned_size = (size + 7) & ~7;

    if (arena->offset + aligned_size > arena->capacity) {
        printf("Arena out of memory! (Need to implement auto-growth)\n");
        exit(1);
    }

    // Grab the current pointer, then "bump" the offset forward
    void *ptr = &arena->buffer[arena->offset];
    arena->offset += aligned_size;

    return ptr;
}

void freeArena(Arena *arena) {
    free(arena->buffer);
    arena->buffer = NULL;
    arena->capacity = 0;
    arena->offset = 0;
}


// --- VM HELPER FUNCTIONS ---

//for when I make a value
Value *allocateValue(VMallMemory *vm)
{
    // Ask the arena for memory instead of the OS
    Value *value = (Value *)arenaAlloc(&vm->objectArena, sizeof(Value));

    // Default initialization
    value->type = OBJ_NULL;

    return value;
}

//functions for stack
void push(VMallMemory *vm, Value *value) {
    if (vm->sp >= STACK_MAX) {
        printf("Stack overflow (Too many plates!)\n");
        exit(1);
    }
    // Put the value at the current index, then move the pointer up one
    vm->stack[vm->sp] = value;
    vm->sp++;
}

Value *pop(VMallMemory *vm) {
    if (vm->sp == 0) {
        printf("Stack underflow\n");//OOOOOOOHH I GET THE REFERENCE I GET IT I GET IT I GET IT absolute legend
        exit(1);
    }
    // Move the pointer down one, then grab that value
    vm->sp--;
    return vm->stack[vm->sp];
}

Value *peek(VMallMemory *vm) {
    if (vm->sp == 0) return NULL;
    return vm->stack[vm->sp - 1]; // Just look, don't move the pointer
}


// --- CORE OPERATIONS ---

//I should get rid of the third RegIndex requirement, but its fun to have some goofyness
void loadFunc(VMallMemory *vm, RegIndex dest, RegIndex startIndex, RegIndex uselesspeiceofJUNK) {
    //so the registers store the LOCATION of the variable, because it is more efficient
    vm->registers[dest] = &constants[startIndex];
}

void pushConstant(VMallMemory *vm, RegIndex constantIndex) {
    // Grab the value from the constants array and toss it on the pile
    push(vm, &constants[constantIndex]);
}

void exAdd(VMallMemory *vm) {
    // Grab the top two values
    Value *right = pop(vm);
    Value *left = pop(vm);

    // Type check
    if ((left->type != OBJ_INT && left->type != OBJ_DOUBLE) ||
        (right->type != OBJ_INT && right->type != OBJ_DOUBLE)) {
        printf("Type error\n");
        return;
        }

    // the math
    Value *newValue = allocateValue(vm);
    if (left->type == OBJ_DOUBLE || right->type == OBJ_DOUBLE) {
        double lVal = (left->type == OBJ_DOUBLE) ? left->doubleValue : (double)left->intValue;
        double rVal = (right->type == OBJ_DOUBLE) ? right->doubleValue : (double)right->intValue;
        newValue->type = OBJ_DOUBLE;
        newValue->doubleValue = lVal + rVal;
    } else {
        newValue->type = OBJ_INT;
        newValue->intValue = left->intValue + right->intValue;
    }

    // Put the answer back on the pile
    push(vm, newValue);
}

void exSub(VMallMemory *vm) {
    Value *left = pop(vm);//i basically have to rethink how im doing all the math now
    Value *right = pop(vm);

    if ((left->type != OBJ_INT && left->type != OBJ_DOUBLE) ||
        (right->type != OBJ_INT && right->type != OBJ_DOUBLE)) {
        printf("Type error\n");
        return;
    }

    Value *newValue = allocateValue(vm);

    if (left->type == OBJ_DOUBLE || right->type == OBJ_DOUBLE) {
        double lVal = (left->type == OBJ_DOUBLE) ? left->doubleValue : (double)left->intValue;
        double rVal = (right->type == OBJ_DOUBLE) ? right->doubleValue : (double)right->intValue;

        newValue->type = OBJ_DOUBLE;
        newValue->doubleValue = lVal - rVal;
    } else {
        newValue->type = OBJ_INT;
        newValue->intValue = left->intValue - right->intValue;
    }

    push(vm, newValue);
}

void exMul(VMallMemory *vm) {
    Value *left = pop(vm);
    Value *right = pop(vm);

    if ((left->type != OBJ_INT && left->type != OBJ_DOUBLE) ||
        (right->type != OBJ_INT && right->type != OBJ_DOUBLE)) {
        printf("Type error\n");
        return;
    }

    Value *newValue = allocateValue(vm);

    if (left->type == OBJ_DOUBLE || right->type == OBJ_DOUBLE) {
        double lVal = (left->type == OBJ_DOUBLE) ? left->doubleValue : (double)left->intValue;
        double rVal = (right->type == OBJ_DOUBLE) ? right->doubleValue : (double)right->intValue;

        newValue->type = OBJ_DOUBLE;
        newValue->doubleValue = lVal * rVal;
    } else {
        newValue->type = OBJ_INT;
        newValue->intValue = left->intValue * right->intValue;
    }

    push(vm, newValue);
}

void exDiv(VMallMemory *vm) {
    Value *left = pop(vm);
    Value *right = pop(vm);

    if ((left->type != OBJ_INT && left->type != OBJ_DOUBLE) ||
        (right->type != OBJ_INT && right->type != OBJ_DOUBLE)) {
        printf("Type error\n");
        return;
    }

    Value *newValue = allocateValue(vm);

    // In your original code, division ALWAYS forced a double result,
    // so we handle it by casting both to doubles immediately.
    double lVal = (left->type == OBJ_DOUBLE) ? left->doubleValue : (double)left->intValue;
    double rVal = (right->type == OBJ_DOUBLE) ? right->doubleValue : (double)right->intValue;

    if (rVal == 0.0) {
        printf("Beep Boop. You failed. Do math better (you divided by zero)\n"); //heh. more goofyness. I was tired
        newValue->type = OBJ_DOUBLE;
        newValue->doubleValue = 0.0;
    } else {
        newValue->type = OBJ_DOUBLE;
        newValue->doubleValue = lVal / rVal;
    }

    push(vm, newValue);
}


// --- EXECUTION ---

//remember to declare helper functions before they are used
void doVmStuff(VMallMemory *vm, OperationCode opcode, RegIndex a, RegIndex b, RegIndex c)
{
    switch(opcode)
    {
        case OP_LOAD:
            loadFunc(vm, a, b, c);
            break;

        case OP_PUSH:
            pushConstant(vm, a);
            break;

        case OP_ADD:
            exAdd(vm);//haha less parameters!
            break;

        case OP_SUB:
            exSub(vm);
            break;

        case OP_MUL:
            exMul(vm);
            break;

        case OP_DIV:
            exDiv(vm);
            break;

        default:
            printf("Unknown instruction\n");
            break;
    }
}


// --- MAIN ---

int main() {
    VMallMemory vm = {0};
    vm.sp = 0;//make stack start at 0

    //Initialize a 1MB Arena (1024 bytes * 1024 bytes)
    initArena(&vm.objectArena, 1024 * 1024);

    // Run VM logic
    doVmStuff(&vm, OP_LOAD, 0, 0, 0);

    // Free the entire arena chunk all at once when done
    freeArena(&vm.objectArena);

    return 0;
}