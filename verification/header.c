#include "stdio.h"
#include "stdlib.h"

typedef unsigned long long _num_;

#define BUFFER 1024

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
void write_char(char *str) {
  printf("%s\n", str);
}
_num_ _my_malloc_(_num_ input) {
  return (_num_) malloc(input);
}

#define malloc _my_malloc_
