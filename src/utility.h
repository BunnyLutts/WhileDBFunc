#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "lang.h"
#include "mem.h"
#include "stdlib.h"

typedef signed long long Primitive;

#define Null() (NULL)
#define IsNull(o) (o == Null())
#define UnWrap(o) (IsNull(o) ? exit(0) : *o)

#define AsBindingPtr(p) ((Binding *) (p))
#define AsPrimitivePtr(p) ((Primitive *) (p))
#define UnWrapAsBinding(p) (UnWrap(AsBindingPtr(p)))
#define UnWrapAsPrimitive(p) (UnWrap(AsPrimitivePtr(p)))

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
typedef struct binding_prototype {
  char *sig;
  Data *data;
} Binding;

Binding *new_binding(char *sig, Data *data);

Primitive *new_primitive(Primitive val);

Data *new_primitive_data(Primitive val);

Data *new_closure_data(struct list *params, struct cmd *body);

Binding *new_empty_binding(char *sig);

Binding *new_primitive_binding(char *sig, Primitive val);

Binding *new_closure_binding(char *sig, struct list *params, struct cmd *body);

Binding *search(Stack *stack, DataType type, char *sig);

struct expr *fold(struct expr *e);

#endif