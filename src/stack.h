#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "stdlib.h"
#include "string.h"

struct Node {
  struct Node *last;
  struct Node *next;
  void *ptr;
};

typedef struct stack_prototype {
  struct Node* top;
} Stack;

struct Node *new_node(struct Node *last, struct Node *next, void *binding);

void *push(Stack *stack, void *ptr);

void *pop(Stack *stack);

void purge(Stack *stack);

#endif