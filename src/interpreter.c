#include "interpreter.h"
#include "lang.h"
#include <stdio.h>

// U CAN ONLY USE MACROS TO ALLOC MEMORY!!!
// these three macros solves memory management on the backend
// NEW(Type) -> Type *
// MALLOC(size: Primitive) -> space: void *
// DEREF(p: Primitive) -> *p: Primitive *
// This deref is actually a type conversion...
// As u need to tackle with null case