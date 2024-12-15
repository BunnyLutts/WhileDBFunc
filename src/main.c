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
    #ifdef DEBUG_MODE
    printf("__PARSE_RESULT_BEGIN__\n");
    print_cmd(root);
    printf("\n__PARSE_RESULT_END__\n");
    printf("__INPTR_RESULT_BEGIN__\n");
    exec_prog(root);
    printf("__INPTR_RESULT_END__\n");
    #endif
    #ifndef DEBUG_MODE
    exec_prog(root);
    #endif
    
    CLEAN;
}
