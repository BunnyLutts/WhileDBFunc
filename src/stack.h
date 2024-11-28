#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "utility.h"
#include "stdlib.h"
#include "string.h"

struct Node {
  struct Node *last;
  struct Node *next;
  Binding *binding;
};

typedef struct stack_prototype {
  struct Node* top;
} Stack;

struct Node *new_node(struct Node *last, struct Node *next, Binding *binding) {
  struct Node *res = (struct Node *) malloc(sizeof(struct Node));
  res->last = last;
  res->next = next;
  res->binding = binding;
  return res;
}

void push(Stack *stack, Binding *binding) {
  if(stack) {
    stack->top->next = new_node(stack->top, NULL, binding);
    stack->top = stack->top->next;
  } else {
    stack->top = new_node(NULL, NULL, binding);
  }
}

Binding *pop(Stack *stack) {
  if(stack) {
    Binding *res = stack->top->binding;
    struct Node *old_top = stack->top;
    stack->top = old_top->last;
    free(old_top);
    return res;
  } else {
    return NULL;
  }
}

Binding *search(Stack *stack, DataType type, char *sig) {
  struct Node *ptr = stack->top;
  while(ptr) {
    if(ptr->binding->data->type == type && strcmp(ptr->binding->sig, sig)) {
      return ptr->binding;
    }
    ptr = ptr->last;
  }
  return NULL;
}

#endif