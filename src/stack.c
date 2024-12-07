#include "stack.h"

struct Node *new_node(struct Node *last, struct Node *next, void *ptr) {
  struct Node *res = (struct Node *) malloc(sizeof(struct Node));
  res->last = last;
  res->next = next;
  res->ptr = ptr;
  return res;
}

void *push(Stack *stack, void *ptr) {
  if(stack->top) {
    stack->top->next = new_node(stack->top, NULL, ptr);
    stack->top = stack->top->next;
  } else { 
    stack->top = new_node(NULL, NULL, ptr);
  }
  return ptr;
}

void *pop(Stack *stack) {
  if(stack->top) {
    void *res = stack->top->ptr;
    struct Node *old_top = stack->top;
    stack->top = old_top->last;
    return res;
  } else {
    void *res = stack->top->ptr;
    stack->top = NULL;
    return res;
  }
}

void *popn(Stack *stack, size_t n) {
    void *ret = NULL;
    if (stack->top) {
        for (; n; n--) {
            ret = pop(stack);
        }
    } else {
        // Assert sth.
    }
    return ret;
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