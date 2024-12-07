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
    if (argc > 1) {
        if ((yyin = fopen(argv[1], "r"))==NULL) {
            fprintf(stderr, "Can't open file %s\n", argv[1]);
            CLEAN;
            exit(1);
        }
    } else {
        yyin = stdin;
    }
    yyparse();
    if (argc > 1) {
        fclose(yyin);
    }
    // fclose(stdin);
    #ifdef DEBUG_MODE
    printf("__PARSE_RESULT_BEGIN__\n");
    print_cmd(root);
    printf("\n__PARSE_RESULT_END__\n"); 
    // These flags are used for result parsing
    // Carefully modify them!
    printf("__INPTR_RESULT_BEGIN__\n");
    #ifdef INTERPRETER_ENABLED
    exec_prog(root);
    #endif
    #ifndef INTERPRETER_ENABLED
    printf("EMPTY\n");
    #endif
    printf("__INPTR_RESULT_END__\n");
    #else
    #ifdef INTERPRETER_ENABLED
    exec_prog(root);
    #else
    printf("No interpreter enabled.\n");
    #endif
    #endif
    
    CLEAN;
}
