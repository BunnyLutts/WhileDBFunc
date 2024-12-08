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
        [(cmdseq) (list 'prog $1)]]
      
      [body
        [(left-brace cmdseq right-brace) (list 'body $2)]]
      
      [call
        [(id left-paren list_expr right-paren) (list 'call $1 (list 'list $3))]]
      
      [cmdseq
        [(cmd) (list $1)]
        [(cmd semicol cmdseq) (cons $1 $3)]]
      
      [cmd 
        [(var id) (list 'decl $2)]
        [(expr asgn expr) (list 'asgn $1 $3)]
        [(if expr then body else body) (list 'if $2 $4 $6)]
        [(while expr do body) (list 'while $2 $4)]
        [(func id left-paren list_id right-paren body) (list 'func $2 (list 'list-params $4) $6)]
        [(call) (cons 'callc (cdr $1))]
        [(return expr) (list 'return $2)]
        [(return) '(return)]]
      
      [expr2
        [(nat) (list 'nat $1)]
        [(left-paren expr right-paren) $2]
        [(id) (list 'id $1)]
        [(not expr2) (list 'unop "!" $2)]
        [(minus expr2) (list 'unop "-" $2)]
        [(mul expr2) (list 'deref $2)]
        [(call) (cons 'calle (cdr $1))]]
      
      [expr
        [(expr2) $1]
        [(expr mul expr) (list 'binop "*" $1 $3)]
        [(expr add expr) (list 'binop "+" $1 $3)]
        [(expr minus expr) (list 'binop "-" $1 $3)]
        [(expr div expr) (list 'binop "/" $1 $3)]
        [(expr mod expr) (list 'binop "%" $1 $3)]
        [(expr lt expr) (list 'binop "<" $1 $3)]
        [(expr gt expr) (list 'binop ">" $1 $3)]
        [(expr le expr) (list 'binop "<=" $1 $3)]
        [(expr ge expr) (list 'binop ">=" $1 $3)]
        [(expr eq expr) (list 'binop "=" $1 $3)]
        [(expr neq expr) (list 'binop "!=" $1 $3)]
        [(expr and expr) (list 'binop "&&" $1 $3)]
        [(expr or expr) (list 'binop "||" $1 $3)]]
      
      [list_id
        [() '()]
        [(id) (list $1)]
        [(id comma list_id) (cons $1 $3)]]
      
      [list_expr
        [() '()]
        [(expr) (list $1)]
        [(expr comma list_expr) (cons $1 $3)]]]))

(define (lnp f) (let ([i (open-input-file f)]) (psr (lambda () (lxr i)))))


(define (fst x) (car x))
(define (snd x) (car (cdr x)))
(define (thd x) (car (cdr (cdr x))))
(define (frd x) (car (cdr (cdr (cdr x)))))

(define apply-seq
  (lambda (f seq)
    (if [null? seq]
        seq
        (begin (f (car seq)) (apply-seq f (cdr seq))))))

(define (emit p)
  (define emit-all (lambda x (apply-seq emit x)))
  (define cmd_tail ";\n")
  (cond
    [(number? p) (display p)]
    [(string? p) (display p)]
    [(eq? (fst p) 'prog) 
      (emit `(func "main" (list ()) (body ,(snd p))))]
    [(eq? (fst p) 'body) 
      (begin 
        (emit "{\n") 
        (apply-seq emit (snd p)) 
        (emit "};\n"))]
    [(eq? (fst p) 'func) 
      (emit-all "_num_ " (snd p) (thd p) " " (frd p))]
    [(eq? (fst p) 'callc) 
      (emit-all (snd p) (thd p) cmd_tail)]
    [(eq? (fst p) 'calle) 
      (emit-all (snd p) (thd p))]
    [(eq? (fst p) 'asgn) 
      (emit-all (snd p) " = " (thd p) cmd_tail)]
    [(eq? (fst p) 'decl) 
      (emit-all "_num_ " (snd p) cmd_tail)]
    [(eq? (fst p) 'unop) 
      (emit-all "(" (snd p) (thd p) ")")]
    [(eq? (fst p) 'binop) 
      (emit-all "(" (thd p) (snd p) (frd p) ")")]
    [(eq? (fst p) 'deref) 
      (emit-all "*((_num_*)" (snd p) ")")]
    [(eq? (fst p) 'return)
      (if [eq? 1 (length p)]
          (emit-all "return 0" cmd_tail)
          (emit-all "return " (snd p) cmd_tail))]
    [(eq? (fst p) 'list) 
      (begin
        (define l (snd p))
        (emit "(")
        (define (emit-list x)
          (cond
            [(null? x) '()]
            [(eq? 1 (length x)) (emit (fst x))]
            [else 
              (begin (emit-all (fst x) ", ") (emit-list (cdr x)))]))
        (emit-list l)
        (emit ")"))]
    [(eq? (fst p) 'list-params) 
      (begin
        (define l (snd p))
        (emit "(")
        (define (emit-list-params x)
          (cond
            [(null? x) '()]
            [(eq? 1 (length x)) (emit-all "_num_ " (fst x))]
            [else 
              (begin (emit-all "_num_ " (fst x) ", ") (emit-list-params (cdr x)))]))
        (emit-list-params l)
        (emit ")"))]
    [(eq? (fst p) 'id) (emit (snd p))]
    [(eq? (fst p) 'nat) (emit (snd p))]))

(define ret (emit (lnp "tests/complex_julia.src")))
; (lnp "tests/complex_julia.src")
; 
; TODO: Get all binding and add them global
; TODO: Get all function, rename and add them global