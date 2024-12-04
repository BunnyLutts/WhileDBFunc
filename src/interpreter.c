#include "interpreter.h"
#include "lang.h"
#include <stdio.h>

// U CAN ONLY USE MACROS TO ALLOC MEMORY!!!
// these three macros solves memory management on the backend
// NEW(Type) -> Type *
// MALLOC(size: Primitive) -> space: void *
// DEREF(p: Primitive) -> *p: Primitive *
// This deref is actually a type conversion...
// As u need to tackle with null case

Stack * init() {
    Stack *p = NEW(Stack);
    p->top = NULL;
    return p;
}

Primitive *exec(Stack *stack, struct cmd *body, size_t *counter) {
    switch (body->t) {
        case T_DECL: {
        }
        case T_ASGN: {
        }
        case T_SEQ: {
        }
        case T_IF: {
        }
        case T_WHILE: {
        }
        case T_WI: {
        }
        case T_WC: {
        }
        case T_FDECL: {
        }
        case T_FCALLC: {
        }
        case T_RET: {
        }
        case T_RETVAL: {
        }
    }
}