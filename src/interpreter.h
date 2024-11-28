#ifndef INTER_H_INCLUDED
#define INTER_H_INCLUDED

#include "utility.h"
#include "stack.h"
#include <stdio.h>

// init empty stack
Stack *init();

// exec body
// 1) stack: program stack of last environment
// 2) body: program body to exec, (type: T_SEQ)
// When encounter a local var/func decl, push it into stack
// and pop then when exiting
Primitive *exec(Stack *stack, struct cmd *body, size_t *counter);

// call function
// push params bindings into stack before exec
// pop bindings after exec
Primitive *call(Stack *stack, Closure *closure, struct list *params);

// eval expression
Primitive *eval(Stack *stack, struct expr* expr);

//
void exec_prog(struct cmd *prog);

#endif