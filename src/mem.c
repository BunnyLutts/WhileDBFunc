#include "mem.h"
#include "stack.h"

Stack *_mem_ = NULL;

void mem_init() {
  _mem_ = (Stack *) malloc(sizeof(Stack));
}