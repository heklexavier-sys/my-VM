//
// Created by hekle on 2026-07-09.
//
// /what is an object? something that you can hold in your hand? Something that is induvidual? sometihng with multiople properties? Which

#include <stdint.h>
#include <stdlib.h>



typedef enum
{
    OBJ_INT,
    OBJ_FLOAT,
    OBJ_STRING,
    OBJ_BOOL,
    OBJ_NULL
} ValueType;

typedef struct
{
    ValueType type;

    union
    {
        int32_t intValue;
        float floatValue;
        const char *stringValue;
        bool boolValue;
    };
} Value;
//DUMMY CONSTANT TABLE
Value constants[] =
{
    { .type = OBJ_INT,    .intValue = 5 },
    { .type = OBJ_INT,    .intValue = 10 },
    { .type = OBJ_FLOAT,  .floatValue = 3.14159f },
    { .type = OBJ_STRING, .stringValue = "Hello, World!" },
    { .type = OBJ_BOOL,   .boolValue = true },
    { .type = OBJ_BOOL,   .boolValue = false },
    { .type = OBJ_NULL }
};
//DUMMY CONSTANT TABLE END


//MY REGISTERS
Value *registers[256];
//VERY IMPORTANT


typedef enum {
    OP_LOAD,
    OP_MOVE,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_JUMP
} OperationCode;

typedef struct {
    OperationCode opcode;
    int a;
    int b;
    int c;
}Intstructions;

void loadFunc(int dest, int startIndex, int uselesspeiceofJUNK) {
    //so the registers store the LOCATION of the variable, because it is more efficient
    registers[dest] = &constants[startIndex];
}

void addFunc(int dest, int firstNumIndex, int secondNumIndex) {
    Value *firstNum = registers[firstNumIndex];
    Value *secondNum = registers[secondNumIndex];
    

    Value *newValue = malloc(sizeof(Value));

    if (firstNum->type == OBJ_FLOAT || secondNum->type == OBJ_FLOAT) {
        float result = firstNum->intValue + secondNum->intValue;
        newValue->type = OBJ_FLOAT;
        newValue->floatValue = result;
    } else {
        int32_t result = firstNum->intValue + secondNum->intValue;
        newValue->type = OBJ_INT;
        newValue->intValue = result;
    }
    
    registers[dest] = newValue;

}

//remember to declare helper functions before they are used
void doVmStuff(OperationCode opcode, int a, int b, int c)
{
    switch(opcode)
    {
        case OP_LOAD:
            loadFunc(a,b,c);
            break;

        case OP_ADD:
            addFunc(a,b,c);
            break;
    }
}



int main() {
    doVmStuff(OP_LOAD, 0, 0, 0);
}