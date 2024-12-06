#include "utility.h"

Binding *new_binding(char *sig, Data *data) {
  Binding *res = NEW(Binding);
  res->sig = sig;
  res->data = data;
  return res;
}

Primitive *new_primitive(Primitive val) {
    Primitive *res = NEW(Primitive);
    *res = val;
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

Binding *search(Stack *stack, DataType type, char *sig) {
  struct Node *ptr = stack->top;
  while(ptr) {
    if(((Binding *)ptr->ptr)->data->type == type && strcmp(((Binding *)ptr->ptr)->sig, sig)==0) {
      return ptr->ptr;
    }
    ptr = ptr->last;
  }
  return NULL;
}