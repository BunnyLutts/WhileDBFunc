#lang racket/base
(require parser-tools/lex)
(require parser-tools/lex-sre)

(define (lexer-wraper lxr input)
  (let ([cur (lxr input)])
    (if [eq? cur eof]
        (list eof)
        (cons cur (lexer-wraper lxr input)))))

(define lxr 
  (lexer 
    [(eof) eof]
    ["var" 'var]
    ["if" 'if]
    ["then" 'then]
    ["else" 'else]
    ["while" 'while]
    ["do" 'do]
    ["malloc" 'malloc]
    ["read_int" 'read_int]
    ["read_char" 'read_char]
    ["write_int" 'write_int]
    ["write_char" 'write_char]
    ["return" 'return]
    ["func" 'func]
    [(: (or "_" alphabetic) (* (or "_" alphabetic numeric))) lexeme]
    [(+ numeric) (string->number lexeme)]
    [";" 'semicol]
    ["(" 'left-paren]
    [")" 'right-paren]
    ["{" 'left-brace]
    ["}" 'right-brace]
    ["+" 'add]
    ["-" 'minus]
    ["*" 'mul]
    ["/" 'div]
    ["%" 'mod]
    ["<=" 'le]
    [">=" 'ge]
    ["==" 'eq]
    ["!=" 'neq]
    ["<" 'lt]
    [">" 'gt]
    ["=" 'asgn]
    ["&&" 'and]
    ["||" 'or]
    ["!" 'not]
    ["," 'comma]
    [(char-set " \n") (lxr input-port)]))

(define i (open-input-string "var a; a = 1; a = a + a + a / a; func squ(a) {return a*a}; a = squ(a)"))

(lexer-wraper lxr i)