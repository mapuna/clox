#include "object.h"
#include "memory.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#include <stdio.h>
#include <string.h>

static Obj *allocateObject(size_t size, ObjType type);

#define ALLOCATE_OBJ(type, objectType)                                         \
    (type *)allocateObject(sizeof(type), objectType)

static Obj *allocateObject(size_t size, ObjType type) {
    Obj *object = (Obj *)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    vm.objects = object;
    return object;
}

static ObjString *allocateString(char *chars, int length, uint32_t hash) {
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    return string;
}

static uint32_t hashString(const char *key, int length) {
    /**
     * FNV-1a hash
     *
     *  hash = offset_basis
     *  for each octet_of_data to be hashed
     *      hash = hash xor octet_of_data
     *      hash = hash * FNV_prime
     *  return hash
     *
     * http://www.isthe.com/chongo/tech/comp/fnv/
     */
    uint32_t hash = 2166136261u; // FNV-1a offset_basis for 32-bits
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619; // FNV-1a prime for 32-bits
    }
    return hash;
}

ObjString *copyString(const char *chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString *interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL)
        return interned;
    char *heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length, hash);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING: printf("%s", AS_CSTRING(value)); break;

        default: break;
    }
}

ObjString *takeString(char *chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString *interened = tableFindString(&vm.strings, chars, length, hash);
    if (interened != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interened;
    }
    return allocateString(chars, length, hash);
}