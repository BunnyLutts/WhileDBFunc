#include "stack.h"

struct Node *new_node(struct Node *last, struct Node *next, void *ptr) {
  struct Node *res = (struct Node *) malloc(sizeof(struct Node));
  res->last = last;
  res->next = next;
  res->ptr = ptr;
  return res;
}

void *push(Stack *stack, void *ptr) {
  if(stack) {
    stack->top->next = new_node(stack->top, NULL, ptr);
    stack->top = stack->top->next;
  } else {
    stack->top = new_node(NULL, NULL, ptr);
  }
  return ptr;
}

void *pop(Stack *stack) {
  if(stack) {
    void *res = stack->top->ptr;
    struct Node *old_top = stack->top;
    stack->top = old_top->last;
    free(old_top);
    return res;
  } else {
    return NULL;
  }
}

void purge(Stack *stack) {
  struct Node *ptr = stack->top;
  while (ptr) {
    free(ptr->ptr);
    struct Node *tmp = ptr;
    ptr = ptr->last;
    free(tmp);
  }
  free(stack);
}