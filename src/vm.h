#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"

#define STACK_MAX 256

typedef struct {
    Chunk *chunk;
    uint8_t *ip; /* Instruction pointer */
    Value stack[STACK_MAX];
    Value *stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM();
void freeVM();

/* The VM runs the chunk and then responds with a value from `InterpretResult`
 * enum: */
// InterpretResult interpret(Chunk *chunk);
InterpretResult interpret(const char *source);
void push(Value value);
Value pop();

#endif // CLOX_VM_H