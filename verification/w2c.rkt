#lang racket/base
(require racket/string)
(require parser-tools/lex)
(require parser-tools/yacc)
(require racket/match)

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
    [(concatenation (union "_" alphabetic) (repetition 0 +inf.0 (union "_" alphabetic numeric))) (token-id lexeme)]
    [(repetition 1 +inf.0 numeric) (token-nat (string->number lexeme))]
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
    [(repetition 1 +inf.0 (char-set " \n\t\r")) (lxr input-port)]))

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
        [() '(nop)]
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
        [(id) (list 'id $1 "")]
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
        [(expr eq expr) (list 'binop "==" $1 $3)]
        [(expr neq expr) (list 'binop "!=" $1 $3)]
        [(expr and expr) (list 'binop "&&" $1 $3)]
        [(expr or expr) (list 'binop "||" $1 $3)]]
      
      [list_id
        [() '()]
        [(expr) (list $1)]
        [(expr comma list_id) (cons $1 $3)]]
      
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

(define var-list '())

(define func-list '())

(define (scan-var p)
  (match p 
    [`(decl ,@res) 
      (begin (set! var-list (cons p var-list)) '(nop))]
    [else p]))


(define (scan-func p)
  (match p 
    [`(func ,@res) 
      (begin (set! func-list (cons p func-list)) '(nop))]
    [else 
      (if [list? p] (map scan-func p) p)]))


(define (emit p)
  (define emit-all (lambda x (apply-seq emit x)))
  (define cmd_tail ";\n")
  (match p
    ['() (display "")]
    ['(nop) (display "")]
    [`(prog ,seq) 
      (emit `(func "main" (list ()) (body ,seq)))]
    [`(body ,seq) 
      (begin 
        (emit "{\n") 
        (apply-seq emit seq) 
        (emit "}\n"))]
    [`(func ,sig ,list-params ,body) 
      (emit-all "_num_ " sig list-params " " body)]
    [`(func-sig ,sig ,list-params) 
      (emit-all "_num_ " sig list-params cmd_tail)]
    [`(callc ,sig ,list-args) 
      (emit-all sig list-args cmd_tail)]
    [`(calle ,sig ,list-args) 
      (emit-all sig list-args)]
    [`(asgn ,var ,expr) 
      (emit-all var " = " expr cmd_tail)]
    [`(decl ,@res) 
      (apply-seq emit `("_num_ " ,@res ,cmd_tail))]
    [`(if ,cond-expr ,then-body ,else-body)
      (emit-all "if (" cond-expr ") " then-body "else\n" else-body)]
    [`(while ,cond-expr ,do-body)
      (emit-all "while (" cond-expr ") " do-body)]
    [`(unop ,op ,target) 
      (emit-all "(" op target ")")]
    [`(binop ,op ,arg1 ,arg2) 
      (emit-all "(" arg1 op arg2 ")")]
    [`(deref (binop "+" (id ,@res) (nat ,offset)))
      #:when (= 0 (modulo offset 8))
      (emit-all "(" "(_num_ *)" `(id ,@res) ")" "[" (/ offset 8) "]")]
    [`(deref (binop "+" (id ,@res) (binop "*" ,offset (nat 8))))
      (emit-all "(" "(_num_ *)" `(id ,@res) ")" "[" offset "]")]
    [`(deref ,ptr) 
      (emit-all "*((_num_*)" ptr ")")]
    [`(return ) 
      (emit-all "return 0" cmd_tail)]
    [`(return ,ret-expr) 
      (emit-all "return " ret-expr cmd_tail)]
    [`(list ,l) 
      (begin
        (emit "(")
        (define (emit-list x)
          (cond
            [(null? x) '()]
            [(eq? 1 (length x)) (emit (fst x))]
            [else 
              (begin (emit-all (fst x) ", ") (emit-list (cdr x)))]))
        (emit-list l)
        (emit ")"))]
    [`(list-params ,l) 
      (begin
        (emit "(")
        (define (emit-list-params x)
          (cond
            [(null? x) '()]
            [(eq? 1 (length x)) (emit-all "_num_ " (fst x))]
            [else 
              (begin (emit-all "_num_ " (fst x) ", ") (emit-list-params (cdr x)))]))
        (emit-list-params l)
        (emit ")"))]
    [`(id ,@res)
      (apply-seq emit res)]
    [`(nat ,num) (emit num)]
    [else (display p)]))

(define path (fst (vector->list (current-command-line-arguments))))
(define presult (lnp path))
(define vresult (list 'prog (map scan-var (snd presult))))
(set! var-list (map (lambda (x) (cons 'decl (cdr x))) var-list))
(define fresult (scan-func vresult))
(define func-sig* (map (lambda (x) (match x [`(func ,sig ,list-params ,body) `(func-sig ,sig ,list-params)] [else x])) func-list))
(define final `(,@(reverse var-list) ,@(reverse func-sig*) ,@(reverse func-list) ,fresult))
; (println final)
(define ret (for-each emit final))