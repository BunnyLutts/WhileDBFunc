#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "lang.h"

// This is the head file of interpreter
// In this file, we define the components of the interpreter

// Firstly, the framework works like this:
// 1. Maintain a global stack that stores all var-binding, functions, and jumps
// 2. When encounter a cmd, operate on the stack
// 3. When encounter a function decl, pack it as a pointer into the stack
// 4. When encounter a expression, eval it over current stack

// Example:
// Prog: var a; a = 1; func squ(x) {return x*x}; write_int(seq(a))
// 1. Encounter variable decl, push an empty binding to the stack 
//    (or initialize it with a default)
// 2. Encounter variable assign, scan the stack top down to find the first "a" binding, 
//    and bind it to eval(1)
// 3. Encounter function decl, pack it body and param list into closure, 
//    push an binding of fname into the stack.
// 4. Encounter function call (builtin), we need to eval(seq(a))
// 5. Scan the stack top down to find first closure binding of "seq"
// 6. Start function call: 
//    1) push current program pointer into the stack; 
//    2) push all params bindings into the stack; 
//    3) exec the body
//    4) when encounter return, return a value like Option<T>
//    5) pop all params bindings
//    6) jump back
// 7. Exec "write_int"

// Some points:
// 1. To simplify code, I recommand interpret "program" as a "main" function
// 2. Function calls in expression and command should be treated seperatedly.
// 3. Pointers are stack pointers.
// 4. Thanks to the resurrsive structure of AST, we can use a single pointer of the AST as PC

// Something like Option<int64> in Rust.
// Used for binding and empty Binding.
// Since we cannot get a empty binding for function, 
// this option is used only for int and ptr.
typedef size_t* Option;

#define Null() = (NULL)
#define IsNull(o) = (o == Null())
#define UnWrap(o) = (IsNull(o) ? exit(0) : *o)

// Closure type
struct Closure {
  struct list *params;
  struct cmd *body;
};

// We have two types of binding
enum DataType {
  D_OPTION, D_CLOSURE
};

// Data type, has two categories
struct Data {
  enum DataType type;
  union {
    Option prime;
    struct Closure closure;
  } data;
};

// Binding type
// Note: "var sig" is assigned a null prime binding
struct Binding {
  char *sig;
  struct Data *data;
};

#endif