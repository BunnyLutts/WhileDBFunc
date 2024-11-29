#ifndef INTER_H_INCLUDED
#define INTER_H_INCLUDED

#include "utility.h"
#include "stack.h"
#include <stdio.h>

// This is the head file of interpreter
// In this file, we define the components of the interpreter

// Firstly, the framework works like this:
// 1. Maintain a global stack that stores all var-binding, functions, and jumps
// 2. When encounter a cmd, operate on the stack
// 3. When encounter a function decl, pack it as a pointer into the stack
// 4. When encounter a expression, eval it over current stack

// Example:
// Prog: var a; a = 1; func squ(x) {return x*x}; write_int(squ(a))
// We exec_prog(c) i.e., exec(an_empty_stack, c, counter_init_by_0)
// 0. Encounter cmd_seq, create a new counter and pass it down
// 1. Encounter variable decl, push an empty binding to the stack 
//    then increase the counter by one. also pass the counter down
// 2. Encounter variable assign, scan the stack top down to find the first "a" binding, 
//    and bind it to eval(1)
// 3. Encounter function decl, pack it body and param list into closure, 
//    push an binding of fname into the stack. increase counter
// 4. Encounter function call (builtin), we need to eval(squ(a))
// 5. Scan the stack top down to find first closure binding of "seq"
// 6. Start function call (squ): 
//    1) push all params bindings into the stack; 
//    2) exec the body
//    3) when encounter return, return a value
//    4) pop all params bindings
//    5) return
// 7. Start function call (write_int): call printf
// 8. cmd_seq c has been executed, pop the number of counter bindings. 
// 9. return. here is null.

// Some points:
// 1. To simplify code, I recommand interpret "program" as a "main" function
// 2. Function calls in expression and command should be treated seperatedly.
// 3. Pointers are stack pointers.
// 4. Thanks to the resurrsive structure of AST, we can use a single pointer of the AST as PC
// 5. When deref a pointer, deref it to the address of Binding
// 6. Use `0` and `1` to represent bool value

// init empty stack
Stack *init();

// exec body
// 1) stack:    program stack of last environment
// 2) body:     program body to exec, (type: T_SEQ)
// 3) counter:  count how many bindings we added
// When encounter a local var/func decl, push it into stack
// and pop them when exiting,
// i.e., When you exec on a cmd_seq, you need to create a new counter,
// and pass it to all cmd. After exec, pop the number of bindings in the stack.
// you only need to increase counter when encounter *pure* var and func decl.
Primitive *exec(Stack *stack, struct cmd *body, size_t *counter);

// call function
// push params bindings into stack before exec
// pop bindings after exec
Primitive *call(Stack *stack, Closure *closure, struct list *params);

// eval expression
Primitive *eval(Stack *stack, struct expr* expr);

// a wrap of exec
void exec_prog(struct cmd *prog);

#endif