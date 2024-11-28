#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "lang.h"
#include "stdlib.h"

#define NEW(type) (type *) malloc(sizeof(type))

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

typedef size_t Primitive;

#define Null() (NULL)
#define IsNull(o) (o == Null())
#define UnWrap(o) (IsNull(o) ? exit(0) : *o)

// Closure type
typedef struct closure_prototype {
  struct list *params;
  struct cmd *body;
} Closure;

// We have three types of binding
typedef enum data_type_prototype {
  D_PRIMITIVE, D_CLOSURE, D_EMPTY
} DataType;

// Data type, has two categories
typedef struct data_prototype {
  DataType type;
  union {
    Primitive *primitive;
    Closure *closure;
  } *data;
} Data;

// Binding type
// Note: "var sig" is assigned a null prime binding
typedef struct Bbinding_prototype {
  char *sig;
  Data *data;
} Binding;

Binding *new_binding(char *sig, Data *data) {
  Binding *res = NEW(Binding);
  res->sig = sig;
  res->data = data;
  return res;
}

Data *new_primitive_data(Primitive val) {
  Data *res = NEW(Data);
  res->type = D_PRIMITIVE;
  res->data->primitive = NEW(Primitive);
  *res->data->primitive = val;
  return res;
}

Data *new_closure_data(struct list *params, struct cmd *body) {
  Data *res = NEW(Data);
  res->type = D_CLOSURE;
  res->data->closure = NEW(Closure);
  res->data->closure->params = params;
  res->data->closure->body = body;
  return res;
}

Binding *new_empty_binding(char *sig) {
  Data *data = NEW(Data);
  data->type = D_EMPTY;
  data->data = NULL;
  return new_binding(sig, data);
}

Binding *new_primitive_binding(char *sig, Primitive val) {
  return new_binding(sig, new_primitive_data(val));
}

Binding *new_closure_binding(char *sig, struct list *params, struct cmd *body) {
  return new_binding(sig, new_closure_data(params, body));
}

#endif