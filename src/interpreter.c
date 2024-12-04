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
            return exec_decl(stack, &body->d, counter);
        }
        case T_ASGN: {
            return exec_asgn(stack, &body->d, counter);
        }
        case T_SEQ: {
            return exec_seq(stack, &body->d, counter);
        }
        case T_IF: {
            return exec_if(stack, &body->d, counter);
        }
        case T_WHILE: {
            return exec_while(stack, &body->d, counter);
        }
        case T_WI: { 
            return exec_if(stack, &body->d, counter);
        }
        case T_WC: {
            return exec_wc(stack, &body->d, counter);
        }
        case T_FDECL: {
            return exec_fdecl(stack, &body->d, counter);
        }
        case T_FCALLC: {
            return exec_asgn(stack, &body->d, counter);
        }
        case T_RET: {
            return exec_ret(stack, &body->d, counter);
        }
        case T_RETVAL: {
            return exec_retval(stack, &body->d, counter);
        }
    }
    return NULL;
}

Primitive *exec_decl(Stack *stack, union CmdContent *body, size_t *counter) {
    push(stack, new_empty_binding(body->DECL.name));
    *counter += 1;
    return NULL;
}

Primitive *exec_asgn(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement asgn evaluation
    return NULL;
}

Primitive *exec_seq(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement seq evaluation
    return NULL;
}

Primitive *exec_if(Stack *stack, union CmdContent *body, size_t *counter) {
    size_t new_counter = 0;
    if (eval(stack, body->IF.cond)) {
        exec(stack, body->IF.left, &new_counter);
    } else {
        exec(stack, body->IF.right, &new_counter);
    }
    popn(stack, new_counter);
    return NULL;
}

Primitive *exec_while(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement while evaluation
    size_t new_counter;
    while (eval(stack, body->WHILE.cond)) {
        new_counter = 0;
        exec(stack, body->WHILE.body, &new_counter);
        popn(stack, new_counter);
    }
    return NULL;
}

Primitive *exec_wi(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement wi evaluation
    return NULL;
}

Primitive *exec_wc(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement wc evaluation
    return NULL;
}

Primitive *exec_fdecl(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement fdecl evaluation
    return NULL;
}

Primitive *exec_fcallc(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement fcallc evaluation
    return NULL;
}

Primitive *exec_ret(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement ret evaluation
    return NULL;
}

Primitive *exec_retval(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement retval evaluation
    return NULL;
}

Primitive *eval(Stack *stack, struct expr* expr) {
    switch (expr->t) {
        case T_CONST: {
            return eval_const(stack, &expr->d);
        }
        case T_VAR: {
            return eval_var(stack, &expr->d);
        }
        case T_BINOP: {
            return eval_binop(stack, &expr->d);
        }
        case T_UNOP: {
            return eval_unop(stack, &expr->d);
        }
        case T_MALLOC: {
            return eval_malloc(stack, &expr->d);
        }
        case T_RI: {
            return eval_ri(stack, &expr->d);
        }
        case T_RC: {
            return eval_rc(stack, &expr->d);
        }
        case T_FCALLE: {
            return eval_fcalle(stack, &expr->d);
        }
    }
    return NULL;
}

Primitive *eval_const(Stack *stack, union ExprContent *expr) {
    // TODO: Implement const evaluation
    return NULL;
}

Primitive *eval_var(Stack *stack, union ExprContent *expr) {
    // TODO: Implement var evaluation
    return NULL;
}

Primitive *eval_binop(Stack *stack, union ExprContent *expr) {
    // TODO: Implement binop evaluation
    return NULL;
}

Primitive *eval_unop(Stack *stack, union ExprContent *expr) {
    // TODO: Implement unop evaluation
    return NULL;
}

Primitive *eval_deref(Stack *stack, union ExprContent *expr) {
    // TODO: Implement dereference evaluation
    return NULL;
}

Primitive *eval_malloc(Stack *stack, union ExprContent *expr) {
    // TODO: Implement malloc evaluation
    return NULL;
}

Primitive *eval_ri(Stack *stack, union ExprContent *expr) {
    // TODO: Implement ri evaluation
    return NULL;
}

Primitive *eval_rc(Stack *stack, union ExprContent *expr) {
    // TODO: Implement rc evaluation
    return NULL;
}

Primitive *eval_fcalle(Stack *stack, union ExprContent *expr) {
    // TODO: Implement fcalle evaluation
    return NULL;
}

void exec_prog(struct cmd *prog) {
    Stack *stack = init();
    size_t counter = 0;
    exec(stack, prog, &counter);
}