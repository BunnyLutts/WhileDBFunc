#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "lang.h"
#include "stdlib.h"

#define NEW(type) (type *) malloc(sizeof(type))


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