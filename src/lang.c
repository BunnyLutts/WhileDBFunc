#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.h"

struct expr * new_expr_ptr() {
  struct expr * res = (struct expr *) malloc(sizeof(struct expr));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct cmd * new_cmd_ptr() {
  struct cmd * res = (struct cmd *) malloc(sizeof(struct cmd));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct list * new_list_ptr() {
  struct list * res = (struct list *) malloc(sizeof(struct list));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct expr * TConst(unsigned int value) {
  struct expr * res = new_expr_ptr();
  res -> t = T_CONST;
  res -> d.CONST.value = value;
  return res;
}

struct expr * TVar(char * name) {
  struct expr * res = new_expr_ptr();
  res -> t = T_VAR;
  res -> d.VAR.name = name;
  return res;
}

struct expr * TBinOp(enum BinOpType op, struct expr * left, struct expr * right) {
  struct expr * res = new_expr_ptr();
  res -> t = T_BINOP;
  res -> d.BINOP.op = op;
  res -> d.BINOP.left = left;
  res -> d.BINOP.right = right;
  return res;
}

struct expr * TUnOp(enum UnOpType op, struct expr * arg) {
  struct expr * res = new_expr_ptr();
  res -> t = T_UNOP;
  res -> d.UNOP.op = op;
  res -> d.UNOP.arg = arg;
  return res;
}

struct expr * TDeref(struct expr * arg) {
  struct expr * res = new_expr_ptr();
  res -> t = T_DEREF;
  res -> d.DEREF.arg = arg;
  return res;
}

struct expr * TMalloc(struct expr * arg) {
  struct expr * res = new_expr_ptr();
  res -> t = T_MALLOC;
  res -> d.MALLOC.arg = arg;
  return res;
}

struct expr * TReadInt() {
  struct expr * res = new_expr_ptr();
  res -> t = T_RI;
  return res;
}

struct expr * TReadChar() {
  struct expr * res = new_expr_ptr();
  res -> t = T_RC;
  return res;
}

struct expr * TFCallE(char * fname, struct list * params) {
  struct expr * res = new_expr_ptr();
  res -> t = T_FCALLE;
  res -> d.FCALLE.fname = fname;
  res -> d.FCALLE.params = params;
  return res;
}

struct cmd * TDecl(char * name) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_DECL;
  res -> d.DECL.name = name;
  return res;
}

struct cmd * TAsgn(struct expr * left, struct expr * right) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_ASGN;
  res -> d.ASGN.left = left;
  res -> d.ASGN.right = right;
  return res;
}

struct cmd * TSeq(struct cmd * left, struct cmd * right) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_SEQ;
  res -> d.SEQ.left = left;
  res -> d.SEQ.right = right;
  return res;
}

struct cmd * TIf(struct expr * cond, struct cmd * left, struct cmd * right) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_IF;
  res -> d.IF.cond = cond;
  res -> d.IF.left = left;
  res -> d.IF.right = right;
  return res;
}

struct cmd * TWhile(struct expr * cond, struct cmd * body) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_WHILE;
  res -> d.WHILE.cond = cond;
  res -> d.WHILE.body = body;
  return res;
}

struct cmd * TWriteInt(struct expr * arg) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_WI;
  res -> d.WI.arg = arg;
  return res;
}

struct cmd * TWriteChar(struct expr * arg) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_WC;
  res -> d.WC.arg = arg;
  return res;
}

struct cmd * TFDecl(char * fname, struct list * params, struct cmd * body) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_FDECL;
  res -> d.FDECL.fname = fname;
  res -> d.FDECL.params = params;
  res -> d.FDECL.body = body;
  return res;
}

struct cmd * TFCallC(char * fname, struct list * params) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_FCALLC;
  res -> d.FCALLC.fname = fname;
  res -> d.FCALLC.params = params;
  return res;
}

struct cmd * TRet() {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_RET;
  return res;
}

struct cmd * TRetVal(struct expr * val) {
  struct cmd * res = new_cmd_ptr();
  res -> t = T_RETVAL;
  res -> d.RETVAL.val = val;
  return res;
}

struct list * TParams(struct expr * head, struct list * tails) {
  struct list * res = new_list_ptr();
  res -> t = T_PARAMS;
  res -> d.PARAMS.head = head;
  res -> d.PARAMS.tails = tails;
  return res;
}

struct list * TNil() {
  struct list * res = new_list_ptr();
  res -> t = T_NIL;
  return res;
}


void print_binop(enum BinOpType op) {
  switch (op) {
  case T_PLUS:
    printf("PLUS");
    break;
  case T_MINUS:
    printf("MINUS");
    break;
  case T_MUL:
    printf("MUL");
    break;
  case T_DIV:
    printf("DIV");
    break;
  case T_MOD:
    printf("MOD");
    break;
  case T_LT:
    printf("LT");
    break;
  case T_GT:
    printf("GT");
    break;
  case T_LE:
    printf("LE");
    break;
  case T_GE:
    printf("GE");
    break;
  case T_EQ:
    printf("EQ");
    break;
  case T_NE:
    printf("NE");
    break;
  case T_AND:
    printf("AND");
    break;
  case T_OR:
    printf("OR");
    break;
  }
}

void print_unop(enum UnOpType op) {
  switch (op) {
  case T_UMINUS:
    printf("UMINUS");
    break;
  case T_NOT:
    printf("NOT");
    break;
  }
}

void print_expr(struct expr * e) {
  switch (e -> t) {
  case T_CONST:
    printf("CONST(%d)", e -> d.CONST.value);
    break;
  case T_VAR:
    printf("VAR(%s)", e -> d.VAR.name);
    break;
  case T_BINOP:
    print_binop(e -> d.BINOP.op);
    printf("(");
    print_expr(e -> d.BINOP.left);
    printf(",");
    print_expr(e -> d.BINOP.right);
    printf(")");
    break;
  case T_UNOP:
    print_unop(e -> d.UNOP.op);
    printf("(");
    print_expr(e -> d.UNOP.arg);
    printf(")");
    break;
  case T_DEREF:
    printf("DEREF(");
    print_expr(e -> d.DEREF.arg);
    printf(")");
    break;
  case T_MALLOC:
    printf("MALLOC(");
    print_expr(e -> d.MALLOC.arg);
    printf(")");
    break;
  case T_RI:
    printf("READ_INT()");
    break;
  case T_RC:
    printf("READ_CHAR()");
    break;
  case T_FCALLE:
    printf("FCALLE(%s, [", e -> d.FCALLE.fname);
    print_list(e -> d.FCALLE.params);
    printf("])");
    break;
  }
}

void print_cmd(struct cmd * c) {
  switch (c -> t) {
  case T_DECL:
    printf("DECL(%s)", c -> d.DECL.name);
    break;
  case T_ASGN:
    printf("ASGN(");
    print_expr(c -> d.ASGN.left);
    printf(",");
    print_expr(c -> d.ASGN.right);
    printf(")");
    break;
  case T_SEQ:
    printf("SEQ(");
    print_cmd(c -> d.SEQ.left);
    printf(",");
    print_cmd(c -> d.SEQ.right);
    printf(")");
    break;
  case T_IF:
    printf("IF(");
    print_expr(c -> d.IF.cond);
    printf(",");
    print_cmd(c -> d.IF.left);
    printf(",");
    print_cmd(c -> d.IF.right);
    printf(")");
    break;
  case T_WHILE:
    printf("WHILE(");
    print_expr(c -> d.WHILE.cond);
    printf(",");
    print_cmd(c -> d.WHILE.body);
    printf(")");
    break;
  case T_WI:
    printf("WRITE_INT(");
    print_expr(c -> d.WI.arg);
    printf(")");
    break;
  case T_WC:
    printf("WRITE_CHAR(");
    print_expr(c -> d.WC.arg);
    printf(")");
    break;
  case T_FDECL:
    printf("DECL(%s, [", c -> d.FDECL.fname);
    print_list(c -> d.FDECL.params);
    printf("], ");
    print_cmd(c -> d.FDECL.body);
    printf(")");
  case T_FCALLC:
    printf("FCALLC(%s, [", c -> d.FCALLC.fname);
    print_list(c -> d.FCALLC.params);
    printf("])");
    break;
  case T_RET:
    printf("RETURN()");
    break;
  case T_RETVAL:
    printf("RETURN(");
    print_expr(c -> d.RETVAL.val);
    printf(")");
    break;
  }
}

void print_list(struct list * l) {
  switch (l -> t) {
  case T_NIL:
    break;
  case T_PARAMS:
    print_expr(l -> d.PARAMS.head);
    printf(", ");
    print_list(l -> d.PARAMS.tails);
  }
}

unsigned int build_nat(char * c, int len) {
  int s = 0, i = 0;
  for (i = 0; i < len; ++i) {
    if (s > 429496729) {
      printf("We cannot handle natural numbers greater than 4294967295.\n");
      exit(0);
    }
    if (s == 429496729 && c[i] > '5') {
      printf("We cannot handle natural numbers greater than 4294967295.\n");
      exit(0);
    }
    s = s * 10 + (c[i] - '0');
  }
  return s;
}

char * new_str(char * str, int len) {
  char * res = (char *) malloc(sizeof(char) * (len + 1));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  strcpy(res, str);
  return res;
}

