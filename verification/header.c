#include "stdio.h"
#include "stdlib.h"

typedef signed long long _num_;

_num_ read_int() {
  _num_ tmp;
  scanf("%lld", &tmp);
  return tmp;
}
_num_ read_char() {
  return (_num_) getchar();
}
void write_int(_num_ input) {
  printf("%lld\n", input);
}
void write_char(_num_ str) {
  printf("%c\n", (int) str);
}
_num_ _my_malloc_(_num_ input) {
  return (_num_) malloc(input);
}
void _my_free_(_num_ input) {
  free((void *) input);
}

#define malloc _my_malloc_
#define free _my_free_
