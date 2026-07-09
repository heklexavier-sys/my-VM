//
// Created by hekle on 2026-07-09.
//

//DUMMY CONSTANT TABLE
typedef enum
{
    OBJ_INT,
    OBJ_FLOAT,
    OBJ_STRING,
    OBJ_BOOL,
    OBJ_NULL
} ObjectType;

typedef struct
{
    ObjectType type;

    union
    {
        int intValue;
        float floatValue;
        const char *stringValue;
        bool boolValue;
    };
} Object;

Object constants[] =
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
Object *registers[8];
//VERY IMPORTANT


typedef enum {
    OP_LOAD
} OperationCode;

typedef struct {
    OperationCode opcode;
    int a;
    int b;
    int c;
}Intstructions;

void loadFunc(int dest, int startIndex, int uselesspeiceofJUNK) {
    //so the registers store the LOCATION of the variable, becasue it is more efficient
    registers[dest] = &constants[startIndex];
}

//remember to declare helper functions before they are used
void doVmStuff(OperationCode opcode, int a, int b, int c)
{
    switch(opcode)
    {
        case OP_LOAD:
            loadFunc(a,b,c);
            break;
    }
}



int main() {
    doVmStuff(OP_LOAD, 0, 0, 0);
}