#lang racket/base
(require parser-tools/lex)
(require parser-tools/lex-sre)
(require parser-tools/yacc)

(define (lexer-wraper lxr input)
  (let ([cur (lxr input)])
    (if [eq? cur 'eof]
        (list 'eof)
        (cons cur (lexer-wraper lxr input)))))

(define-tokens valued-tokens (nat id))

(define-empty-tokens reserved-tokens 
  (var if then else while do return func))

(define-empty-tokens op-tokens
  (add minus mul div mod le ge eq neq lt gt asgn and or not))

(define-empty-tokens punct-tokens
  (semicol left-paren right-paren left-brace right-brace comma eof))

(define lxr 
  (lexer 
    [(eof) (token-eof)]
    ["var" (token-var)]
    ["if" (token-if)]
    ["then" (token-then)]
    ["else" (token-else)]
    ["while" (token-while)]
    ["do" (token-do)]
    ["return" (token-return)]
    ["func" (token-func)]
    [(: (or "_" alphabetic) (* (or "_" alphabetic numeric))) (token-id lexeme)]
    [(+ numeric) (token-nat (string->number lexeme))]
    [";" (token-semicol)]
    ["(" (token-left-paren)]
    [")" (token-right-paren)]
    ["{" (token-left-brace)]
    ["}" (token-right-brace)]
    ["+" (token-add)]
    ["-" (token-minus)]
    ["*" (token-mul)]
    ["/" (token-div)]
    ["%" (token-mod)]
    ["<=" (token-le)]
    [">=" (token-ge)]
    ["==" (token-eq)]
    ["!=" (token-neq)]
    ["<" (token-lt)]
    [">" (token-gt)]
    ["=" (token-asgn)]
    ["&&" (token-and)]
    ["||" (token-or)]
    ["!" (token-not)]
    ["," (token-comma)]
    [(+ (char-set " \n")) (lxr input-port)]))

(define i (open-input-file "tests/complex_julia.src"))

(define psr
  (parser
    
    [start prog]
    [end eof]
    [error void]
    [tokens valued-tokens reserved-tokens op-tokens punct-tokens]
    
    [precs 
      [nonassoc asgn]
      [left or]
      [left and]
      [left lt le gt ge eq neq]
      [left add minus]
      [left mul div mod]
      [left not]
      [left left-paren right-paren]
      [right semicol comma]]
    
    [grammar
      [prog 
        [(cmd) (list 'prog $1)]]
      
      [body
        [(left-brace cmd right-brace) (list 'body $2)]]
      
      [call
        [(id left-paren list right-paren) (list 'call $1 $3)]]
      
      [cmd 
        [(var id) (list 'decl $2)]
        [(expr asgn expr) (list 'asgn $1 $3)]
        [(cmd semicol cmd) (cons $1 $3)]
        [(if expr then body else body) (list 'if $2 $4 $6)]
        [(while expr do body) (list 'while $2 $4)]
        [(func id left-paren list right-paren body) (list 'func $2 $4 $6)]
        [(call) $1]
        [(return expr) (list 'return $2)]
        [(return) '(return)]]
      
      [expr2
        [(nat) $1]
        [(left-paren expr right-paren) $2]
        [(id) $1]
        [(not expr2) (list 'not $2)]
        [(minus expr2) (list 'neg $2)]
        [(mul expr2) (list 'deref $2)]
        [(call) $1]]
      
      [expr
        [(expr2) $1]
        [(expr mul expr) (list '* $1 $3)]
        [(expr add expr) (list '+ $1 $3)]
        [(expr minus expr) (list '- $1 $3)]
        [(expr div expr) (list '/ $1 $3)]
        [(expr mod expr) (list 'mod $1 $3)]
        [(expr lt expr) (list '< $1 $3)]
        [(expr gt expr) (list '> $1 $3)]
        [(expr le expr) (list '<= $1 $3)]
        [(expr ge expr) (list '>= $1 $3)]
        [(expr eq expr) (list '= $1 $3)]
        [(expr neq expr) (list '!= $1 $3)]
        [(expr and expr) (list '&& $1 $3)]
        [(expr or expr) (list '|| $1 $3)]]
      
      [list
        [() '()]
        [(expr) (list $1)]
        [(expr comma list) (cons $1 $3)]]]))

(define (lnp i) (psr (lambda () (lxr i))))

(lnp i)