#include <stdio.h>
#include "lang.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#define INIT mem_init()

#define CLEAN purge(_mem_)

extern struct cmd * root;
int yyparse();

int main(int argc, char **argv) {
    INIT;
    yyin = stdin;
    yyparse();
    fclose(stdin);
    printf("__PARSE_RESULT_BEGIN__ ");
    print_cmd(root);
    printf(" __PARSE_RESULT_END__ ");
    printf("__INPTR_RESULT_BEGIN__ ");
    #ifdef INTERPRETER_ENABLED
    exec_prog(root);
    #endif
    #ifndef INTERPRETER_ENABLED
    printf("EMPTY");
    #endif
    printf(" __INPTR_RESULT_END__");
    
    CLEAN;
}
