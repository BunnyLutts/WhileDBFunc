#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include "stack.h"

// Memeory Management

#define MALLOC(E) (push(_mem_, malloc(E)))
#define NEW(type) ((type *) (MALLOC(sizeof(type))))
#define DEREF(p) ((Primitive *) p)

extern Stack *_mem_;

void mem_init();

#endif