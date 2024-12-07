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

void fault(const char *msg) {
    fprintf(stderr, "Program fault: %s\n", msg);
    purge(_mem_);
    exit(-1);
}

void checkp(void *p) {
    if (IsNull(p)) {
        fault("Illegal expression!");
    }
}

void push_args(Stack *stack, Closure *closure, struct list *args, size_t *counter) {
    struct list *params = closure->params;
    Stack *prev_stack = stack;
    for (; params && params->t != T_NIL && args && args->t != T_NIL; params = params->d.PARAMS.tails, args = args->d.PARAMS.tails) {
        push(stack, new_primitive_binding(params->d.PARAMS.head->d.VAR.name, *eval(prev_stack, args->d.PARAMS.head)));
        *counter++;
    }
    if ((params && params->t!= T_NIL) ^ (args && args->t!= T_NIL)) {
        fault("Wrong number of arguments.");
    }
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
    push(stack, new_primitive_binding(body->DECL.name, 0));
    // push(stack, new_empty_binding(body->DECL.name));
    *counter += 1;
    return NULL;
}

Primitive *exec_asgn(Stack *stack, union CmdContent *body, size_t *counter) {
    Primitive *left = eval(stack, body->ASGN.left);
    Primitive *right = eval(stack, body->ASGN.right);
    if (left) {
        if (right) {
            *left = *right;
        } else {
            fault("Illegal right expression!");
        }
    } else {
        fault("Undefined left-expr!");
    }
    return NULL;
}

Primitive *exec_seq(Stack *stack, union CmdContent *body, size_t *counter) {
    Primitive *ret = NULL;
    if (ret = exec(stack, body->SEQ.left, counter)) {
        return ret;
    } else {
        ret = exec(stack, body->SEQ.right, counter);
    }
    return ret;
}

Primitive *exec_if(Stack *stack, union CmdContent *body, size_t *counter) {
    Primitive *ret = NULL;
    size_t new_counter = 0;
    if (eval(stack, body->IF.cond)) {
        ret = exec(stack, body->IF.left, &new_counter);
    } else {
        ret = exec(stack, body->IF.right, &new_counter);
    }
    popn(stack, new_counter);
    return ret;
}

Primitive *exec_while(Stack *stack, union CmdContent *body, size_t *counter) {
    Primitive *ret = NULL;
    size_t new_counter;
    while (eval(stack, body->WHILE.cond)) {
        new_counter = 0;
        ret = exec(stack, body->WHILE.body, &new_counter);
        popn(stack, new_counter);
        if (ret) {
            return ret;
        }
    }
    return ret;
}

Primitive *exec_wi(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement write_int evaluation
    
    struct expr *arg = body->WI.arg;
    Primitive *result = eval(stack, arg);
    printf("%lld\n", *result);
    

    return NULL;
}

Primitive *exec_wc(Stack *stack, union CmdContent *body, size_t *counter) {
    // TODO: Implement write_char evaluation

    struct expr *arg = body->WC.arg;
    Primitive *result = eval(stack, arg);
    char res=(char)(*result);
    printf("%c\n", res);

    return NULL;
}

Primitive *exec_fdecl(Stack *stack, union CmdContent *body, size_t *counter) {
    push(stack, new_closure_binding(body->FDECL.fname, body->FDECL.params, body->FDECL.body));
    return NULL;
}

Primitive *exec_fcallc(Stack *stack, union CmdContent *body, size_t *counter) {
    size_t new_counter = 0;
    Binding *func_b = search(stack, D_CLOSURE, body->FCALLC.fname);
    if (!func_b) {
        fault("Undefined function!");
    }
    Closure *func = func_b->data->data->closure;
    push_args(stack, func, body->FCALLC.params, &new_counter);
    exec(stack, func->body, &new_counter);
    popn(stack, new_counter);
    return NULL;
}

Primitive *exec_ret(Stack *stack, union CmdContent *body, size_t *counter) {
    Primitive *p = NEW(Primitive);
    *p = 0;
    return p;
}

Primitive *exec_retval(Stack *stack, union CmdContent *body, size_t *counter) {
    return eval(stack, body->RETVAL.val);
}

Primitive *eval(Stack *stack, struct expr* expr) {
    Primitive *ret = NULL;
    switch (expr->t) {
        case T_CONST: {
            ret = eval_const(stack, &expr->d);
            break;
        }
        case T_VAR: {
            ret = eval_var(stack, &expr->d);
            break;
        }
        case T_BINOP: {
            ret = eval_binop(stack, &expr->d);
            break;
        }
        case T_UNOP: {
            ret = eval_unop(stack, &expr->d);
            break;
        }
        case T_MALLOC: {
            ret = eval_malloc(stack, &expr->d);
            break;
        }
        case T_RI: {
            ret = eval_ri(stack, &expr->d);
            break;
        }
        case T_RC: {
            ret = eval_rc(stack, &expr->d);
            break;
        }
        case T_FCALLE: {
            ret = eval_fcalle(stack, &expr->d);
            break;
        }
    }
    checkp(ret);
    return ret;
}

Primitive *eval_const(Stack *stack, union ExprContent *expr) {
    // TODO: Implement const evaluation
    if(expr)
        {
            Primitive *ret = new_primitive(expr->CONST.value);
            return ret;
        }
    return NULL;
}

Primitive *eval_var(Stack *stack, union ExprContent *expr) {
    // TODO: Implement var evaluation
    if(expr)
    {
        Binding *b = search(stack, D_PRIMITIVE, expr->VAR.name);
        if(!b) return NULL;
        Primitive *ret=new_primitive(*(b->data->data->primitive)); 
        return ret;
    }
    return NULL;
}

Primitive *eval_binop(Stack *stack, union ExprContent *expr) {
    // TODO: Implement binop evaluation
    if(expr)
    {
        switch(expr->BINOP.op)
        {
            case T_PLUS: return new_primitive( *eval(stack, expr->BINOP.left) + *eval(stack, expr->BINOP.right));
            case T_MINUS: return new_primitive( *eval(stack, expr->BINOP.left) - *eval(stack, expr->BINOP.right));
            case T_MUL: return new_primitive( *eval(stack, expr->BINOP.left) * *eval(stack, expr->BINOP.right));
            case T_DIV: return new_primitive( *eval(stack, expr->BINOP.left) / *eval(stack, expr->BINOP.right));
            case T_MOD: return new_primitive( *eval(stack, expr->BINOP.left) % *eval(stack, expr->BINOP.right));
            case T_LT: return new_primitive( *eval(stack, expr->BINOP.left) < *eval(stack, expr->BINOP.right));
            case T_GT: return new_primitive( *eval(stack, expr->BINOP.left) > *eval(stack, expr->BINOP.right));
            case T_LE: return new_primitive( *eval(stack, expr->BINOP.left) <= *eval(stack, expr->BINOP.right));
            case T_GE: return new_primitive( *eval(stack, expr->BINOP.left) >= *eval(stack, expr->BINOP.right));
            case T_EQ: return new_primitive( *eval(stack, expr->BINOP.left) == *eval(stack, expr->BINOP.right));
            case T_NE: return new_primitive( *eval(stack, expr->BINOP.left) != *eval(stack, expr->BINOP.right));
            case T_AND: return new_primitive( *eval(stack, expr->BINOP.left) && *eval(stack, expr->BINOP.right));
            case T_OR: return new_primitive( *eval(stack, expr->BINOP.left) || *eval(stack, expr->BINOP.right));
        }
    }
    return NULL;
}

Primitive *eval_unop(Stack *stack, union ExprContent *expr) {
    // Implement unop evaluation
    Primitive *ret = NULL;
    switch (expr->UNOP.op) {
        case T_UMINUS: {
            ret = new_primitive(-(*eval(stack, expr->UNOP.arg)));
            break;
        }
        case T_NOT: {
            ret = new_primitive(!(*eval(stack, expr->UNOP.arg)));
            break;
        }
    }
    return ret;
}

Primitive *eval_deref(Stack *stack, union ExprContent *expr) {
    return DEREF(*eval(stack, expr->DEREF.arg));
}

Primitive *eval_malloc(Stack *stack, union ExprContent *expr) {
    Primitive *val = eval(stack, expr->MALLOC.arg);
    return MALLOC(*val);
}

Primitive *eval_ri(Stack *stack, union ExprContent *expr) {
    // Implement ri evaluation
    Primitive *ret = NULL;
    Primitive read_num;
    scanf("%lld", &read_num);
    ret = new_primitive(read_num);
    return ret;
}

Primitive *eval_rc(Stack *stack, union ExprContent *expr) {
    // Implement rc evaluation
    Primitive *ret = NULL;
    char read_char;
    scanf("%c", &read_char);
    ret = new_primitive(read_char);
    return ret;
}

Primitive *eval_fcalle(Stack *stack, union ExprContent *expr) {
    Primitive *ret = NULL;
    size_t new_counter = 0;
    Binding *func_b = search(stack, D_CLOSURE, expr->FCALLE.fname);
    if (!func_b) {
        fault("Undefined function!");
    }
    Closure *func = func_b->data->data->closure;
    push_args(stack, func, expr->FCALLE.params, &new_counter);
    ret = exec(stack, func->body, &new_counter);
    popn(stack, new_counter);
    return ret;
}

void exec_prog(struct cmd *prog) {
    Stack *stack = init();
    size_t counter = 0;
    exec(stack, prog, &counter);
}