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

struct Node *new_node(struct Node *last, struct Node *next, Binding *binding);

void push(Stack *stack, Binding *binding);

Binding *pop(Stack *stack);

Binding *search(Stack *stack, DataType type, char *sig);

#endif