#ifndef INTER_H_INCLUDED
#define INTER_H_INCLUDED

#include "utility.h"
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
// 3. Thanks to the resurrsive structure of AST, we can use a single pointer of the AST as PC
// 4. Use `0` and `1` to represent bool value

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

// eval expression, a hard work, I know
// it should be EMPHASIZED THAT for any left value(such as binop and unop) you should use new_primitive instead of old value.
Primitive *eval(Stack *stack, struct expr* expr);

// a wrap of exec
void exec_prog(struct cmd *prog);

// showing error message and exit
void fault(const char * msg);

// check p is not null
void checkp(void *p);

// push arguments for function call
void push_args(Stack *stack, Closure *closure, struct list *params, size_t *counter);

Primitive *exec_decl(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_asgn(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_seq(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_if(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_while(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_wi(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_wc(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_fdecl(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_fcallc(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_ret(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_retval(Stack *stack, union CmdContent *body, size_t *counter);
Primitive *exec_nop(Stack *stack, union CmdContent *body, size_t *counter);

Primitive *eval_const(Stack *stack, union ExprContent *expr);
Primitive *eval_var(Stack *stack, union ExprContent *expr);
Primitive *eval_binop(Stack *stack, union ExprContent *expr);
Primitive *eval_unop(Stack *stack, union ExprContent *expr);
Primitive *eval_deref(Stack *stack, union ExprContent *expr);
Primitive *eval_malloc(Stack *stack, union ExprContent *expr); // The argument of malloc should be byte number.
Primitive *eval_ri(Stack *stack, union ExprContent *expr);
Primitive *eval_rc(Stack *stack, union ExprContent *expr);
Primitive *eval_fcalle(Stack *stack, union ExprContent *expr);

#endif