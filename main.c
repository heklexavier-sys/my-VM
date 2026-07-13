//
// Created by hekle on 2026-07-09.
//
// /what is an object? something that you can hold in your hand? Something that is induvidual? sometihng with multiople properties? Which

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//when it will collect garbage. IDK how high/low to set this
#define INITIAL_GC_LIMIT 1024


typedef enum
{
    OBJ_INT,
    OBJ_DOUBLE,
    OBJ_STRING,
    OBJ_BOOL,
    OBJ_NULL
} ValueType;

typedef struct Value
{
    ValueType type;

    bool marked;

    struct Value *next;

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


//for determining which register - small int is better
typedef uint8_t RegIndex;

//MY REGISTERS
//makes the size of the registers equal to the size of RegIndex
//
typedef struct
{
    Value *registers[(size_t)(RegIndex)~(RegIndex)0 + 1];

    Value *objects;

    // Garbage collector information
    //important note: GC is not group chat, it is garbage collector. We use too many acronyms.
    size_t allocatedBytes;
    size_t nextGC;

} VMallMemory;
//VERY IMPORTANT


//GARBAGE COLLECTION GARBAGE COLLECTION GARBAGE COLLECTION GARBAGE COLLECTION GARBAGE COLLECTION GARBAGE COLLECTION
//MARKING OBJECTS
void markValue(Value *value)
{
    if(value == NULL)
        return;

    value->marked = true;
}
//MARKING THE REGISTERS
void markRegs(VMallMemory *vm)
{
    for(int i = 0; i <= (size_t)(RegIndex)~(RegIndex)0; i++)
    {
        markValue(vm->registers[i]);
    }
}

//taking out the trash
void sweep(VMallMemory *vm)
{
    Value **current = &vm->objects;


    while(*current)
    {
        Value *value = *current;


        if(value->marked == false)
        {
            *current = value->next;
            free(value);
            vm->allocatedBytes -= sizeof(Value);//gotta remember this.
        }
        else
        {
            value->marked = false;
            current = &value->next;
        }
    }
}
//all together now
void collectGarbage(VMallMemory *vm)
{
    markRegs(vm);

    sweep(vm);
}
//I think thats it for garbage collection - i need to add handling for strings later...



//for when I make a value
Value *allocateValue(VMallMemory *vm)
{

    if(vm->allocatedBytes > vm->nextGC)//call this first so there no temporary excess
    {
        collectGarbage(vm);

        vm->nextGC = vm->allocatedBytes * 2;
    }


    Value *value = malloc(sizeof(Value));

    if(value == NULL)
    {
        printf("Out of memory\n");
        exit(1);
    }

    vm->allocatedBytes += sizeof(Value);




    value->type = OBJ_NULL;
    value->marked = false;


    value->next = vm->objects;
    vm->objects = value;


    return value;
}



//Now THIS is where stuff happens. Mostly done initializing
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
    RegIndex a;
    RegIndex b;
    RegIndex c;
}Intstructions;
//I should get rid of the third RegIndex requirement, but its fun to have some goofyness
void loadFunc(VMallMemory *vm, RegIndex dest, RegIndex startIndex, RegIndex uselesspeiceofJUNK) {
    //so the registers store the LOCATION of the variable, because it is more efficient
    vm->registers[dest] = &constants[startIndex];
}

typedef enum {
    MATH_ADD,
    MATH_SUB,
    MATH_MUL,
    MATH_DIV,
} MathType;



void basicMathFuncDouble(VMallMemory *vm, RegIndex dest, RegIndex firstNumIndex, RegIndex secondNumIndex, MathType mathType) {



    Value *firstNum = vm->registers[firstNumIndex];
    Value *secondNum =  vm->registers[secondNumIndex];


    if ((firstNum->type != OBJ_INT &&
     firstNum->type != OBJ_DOUBLE) ||
    (secondNum->type != OBJ_INT &&
     secondNum->type != OBJ_DOUBLE))
    {
        printf("Type error\n");
        return;
    }

    Value *newValue = allocateValue(vm);

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
                if (holderForSecondNum == 0.0)
                {
                    printf("Beep Boop. You failed. Do math better (you divided by zero)\n");//heh. more goofyness. I was tired
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


     vm->registers[dest] = newValue;
}




void basicMathFuncInt(VMallMemory *vm, RegIndex dest, RegIndex firstNumIndex, RegIndex secondNumIndex, MathType mathType) {



    Value *firstNum = vm->registers[firstNumIndex];
    Value *secondNum = vm->registers[secondNumIndex];


    if ((firstNum->type != OBJ_INT) || (secondNum->type != OBJ_INT))
    {
        printf("Type error, expecting INT\n");
        return;
    }

    Value *newValue = allocateValue(vm);

    int32_t result;
    switch(mathType) {
        case MATH_ADD:
            result = firstNum->intValue + secondNum->intValue;
            break;

        case MATH_SUB:
            result = firstNum->intValue - secondNum->intValue;
            break;

        case MATH_MUL:
            result = firstNum->intValue * secondNum->intValue;
            break;


        default:
            result = 0;
    }

        newValue->type = OBJ_INT;
        newValue->intValue = result;


    vm->registers[dest] = newValue;
}





//remember to declare helper functions before they are used
void doVmStuff(VMallMemory *vm, OperationCode opcode, RegIndex a, RegIndex b, RegIndex c)
{



    switch(opcode)
    {
        case OP_LOAD:
            loadFunc(vm, a,b,c);
            break;

            //math functions
        case OP_ADD: {
            Value *left = vm->registers[b];
            Value *right = vm->registers[c];
            if (left->type == OBJ_DOUBLE ||
                right->type == OBJ_DOUBLE)//check everything...
            {
                basicMathFuncDouble(vm,a, b, c, MATH_ADD);
            }
            else
            {
                basicMathFuncInt(vm,a, b, c, MATH_ADD);
            }
            break;
        }

        case OP_SUB: {
            Value *left = vm->registers[b];
            Value *right = vm->registers[c];
            if (left->type == OBJ_DOUBLE ||
                right->type == OBJ_DOUBLE)
            {
                basicMathFuncDouble(vm,a, b, c, MATH_SUB);
            }
            else
            {
                basicMathFuncInt(vm,a, b, c, MATH_SUB);
            }
            break;
        }

        case OP_MUL: {
            Value *left = vm->registers[b];
            Value *right = vm->registers[c];
            if (left->type == OBJ_DOUBLE ||
                right->type == OBJ_DOUBLE)
            {
                basicMathFuncDouble(vm,a, b, c, MATH_MUL);
            }
            else
            {
                basicMathFuncInt(vm,a, b, c, MATH_MUL);
            }
            break;
        }

        case OP_DIV:
            basicMathFuncDouble(vm,a, b, c, MATH_DIV);
            break;
    }
}



int main() {
    VMallMemory vm = {0};

    vm.nextGC = INITIAL_GC_LIMIT;//otherwise there would be immediate garbage collection

    doVmStuff(&vm, OP_LOAD, 0, 0, 0);
}