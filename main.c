//
// Created by hekle on 2026-07-09.
//
// /what is an object? something that you can hold in your hand? Something that is induvidual? sometihng with multiople properties? Which

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



typedef enum
{
    OBJ_INT,
    OBJ_DOUBLE,
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
        double doubleValue;
        const char *stringValue;
        bool boolValue;
    };
} Value;


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


//MY REGISTERS
Value *registers[256];
//VERY IMPORTANT

//for determining which register - small int is better
typedef uint8_t RegIndex;



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

typedef enum {
    MATH_ADD,
    MATH_SUB,
    MATH_MUL,
    MATH_DIV,
} MathType;

void basicMathFunc(RegIndex dest, RegIndex firstNumIndex, RegIndex secondNumIndex, MathType mathType) {
    
    

    Value *firstNum = registers[firstNumIndex];
    Value *secondNum = registers[secondNumIndex];


    if ((firstNum->type != OBJ_INT &&
     firstNum->type != OBJ_DOUBLE) ||
    (secondNum->type != OBJ_INT &&
     secondNum->type != OBJ_DOUBLE))
    {
        printf("Type error\n");
        return;
    }
    
    Value *newValue = malloc(sizeof(Value));

    if (firstNum->type == OBJ_DOUBLE || secondNum->type == OBJ_DOUBLE)
    {
        double holderForFirstNum;
        double holderForSecondNum;

        if (firstNum->type == OBJ_DOUBLE)
            holderForFirstNum = firstNum->doubleValue;
        else
            holderForFirstNum = (double)firstNum->intValue;

        if (secondNum->type == OBJ_DOUBLE)
            holderForSecondNum = secondNum->doubleValue;
        else
            holderForSecondNum = (double)secondNum->intValue;

        double result;
        switch(mathType)
        {
            case MATH_ADD:
                result = holderForFirstNum + holderForSecondNum;
                break;

            case MATH_SUB:
                result = holderForFirstNum - holderForSecondNum;
                break;

            case MATH_MUL:
                result = holderForFirstNum * holderForSecondNum;
                break;

            case MATH_DIV:
                if (secondNum->intValue == 0)
                {
                    printf("Beep Boop. You failed. Do math better (you divided by zero)\n");
                    result = 0;
                } else {
                    result = holderForFirstNum / holderForSecondNum;
                }
                break;

            default:
                result = 0;

        }


        newValue->type = OBJ_DOUBLE;
        newValue->doubleValue = result;
    }
    else
    {
        int32_t result;
        switch(mathType)
        {
            case MATH_ADD:
                result = firstNum->intValue + secondNum->intValue;
                break;

            case MATH_SUB:
                result = firstNum->intValue - secondNum->intValue;
                break;

            case MATH_MUL:
                result = firstNum->intValue * secondNum->intValue;
                break;

            case MATH_DIV:
                if (secondNum->intValue == 0)
                {
                    printf("Beep Boop. You failed. Do math better (you divided by zero)\n");
                    result = 0;
                } else {
                    result = firstNum->intValue / secondNum->intValue;
                }
                break;

            default:
                result = 0;

        }

        newValue->type = OBJ_INT;
        newValue->intValue = result;
    }

    registers[dest] = newValue;
}

//remember to declare helper functions before they are used
void doVmStuff(OperationCode opcode, RegIndex a, RegIndex b, RegIndex c)
{
    switch(opcode)
    {
        case OP_LOAD:
            loadFunc(a,b,c);
            break;

        case OP_ADD:
            basicMathFunc(a, b, c, MATH_ADD);
            break;

        case OP_SUB:
            basicMathFunc(a, b, c, MATH_SUB);
            break;

        case OP_MUL:
            basicMathFunc(a, b, c, MATH_MUL);
            break;

        case OP_DIV:
            basicMathFunc(a, b, c, MATH_DIV);
            break;
    }
}



int main() {
    doVmStuff(OP_LOAD, 0, 0, 0);
}