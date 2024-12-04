#include "interpreter.h"
#include "lang.h"
#include "stack.h"
#include "utility.h"
#include <stdio.h>

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