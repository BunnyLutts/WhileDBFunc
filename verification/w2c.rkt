#lang racket/base
(require racket/string)
(require parser-tools/lex)
(require parser-tools/yacc)
(require racket/match)

(define counter 0)

(define key-gen (lambda () (set! counter (+ counter 1)) (string-append "_" (number->string counter))))

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
    [(repetition 1 +inf.0 (char-set " \n")) (lxr input-port)]))

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
        [(expr eq expr) (list 'binop "=" $1 $3)]
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

; A -> B -> B
(define (replace pack p)
  (if [and (list? p) (not (null? p))]
      (if [eq? (fst p) (fst pack)]
          (if [equal? (snd p) (snd pack)] pack p)
          (map (lambda (x) (replace pack x)) p))
      p))

; B -> list A -> B
(define (replace-list p pack*) (foldl replace p pack*))

(define var-list '())

(define func-list '())

; A*B -> list A -> B -> (list A) * b
(define (applier f al b)
  (if [null? al]
      (cons al b)
      (let* ([had (f (cons (car al) b))]
             [tal (applier f (cdr al) (cdr had))])
            (cons (cons (car had) (car tal)) (cdr tal)))))

; A*B -> A*B
(define (scan-var-core pack)
  (let ([p (car pack)]
        [t (cdr pack)])
      (if [or (not (list? p)) (null? p)]
          pack
          (match p
            [`(decl ,var)
              (begin  (define bind `(id ,var ,(key-gen)))
                      (set! var-list (cons bind var-list))
                      (set! t (cons bind t))
                      (set! t (replace bind t))
                      (cons '(nop) t))]
            [`(func ,sig (list-params ,params) ,body)
              (begin  (define tmp-var-list '())
                      (map (lambda (x) (set! tmp-var-list (cons `(id ,(snd x) ,(key-gen)) tmp-var-list))) params)
                          (set! var-list (append tmp-var-list var-list))
                          (set! t (append tmp-var-list t))
                          (set! t (replace-list t tmp-var-list))
                          (define lpr (replace-list params t))
                          (define nxt (scan-var-core (cons body t)))
                          (set! p `(func ,(snd p) (list-params ,lpr) ,(car nxt)))
                          (cons p (cdr nxt)))]
            [else 
              (begin  (set! p (replace-list p t))
                      (applier scan-var-core p t))]))))

(define (scan-var x) (scan-var-core (cons x '())))

(define (scan-func p)
  (match p 
    [`(func ,res) 
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
    [`(callc ,sig ,list-args) 
      (emit-all sig list-args cmd_tail)]
    [`(calle ,sig ,list-args) 
      (emit-all sig list-args)]
    [`(asgn ,var ,expr) 
      (emit-all var " = " expr cmd_tail)]
    [`(decl ,id ,tag) 
      (emit-all "_num_ " id tag cmd_tail)]
    [`(if ,cond-expr ,then-body ,else-body)
      (emit-all "if (" cond-expr ") " then-body "else\n" else-body)]
    [`(while ,cond-expr ,do-body)
      (emit-all "while (" cond-expr ") " do-body)]
    [`(unop ,op ,target) 
      (emit-all "(" op target ")")]
    [`(binop ,op ,arg1 ,arg2) 
      (emit-all "(" arg1 op arg2 ")")]
    [`(deref ,ptr) 
      (emit-all "*((_num_*)" ptr ")")]
    [`(return) 
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
    [`(id ,id ,tag) (emit-all id tag)]
    [`(nat ,num) (emit num)]
    [else (display p)]))

(define path (fst (vector->list (current-command-line-arguments))))
(define presult (lnp path))
(define vresult (car (scan-var presult)))
(set! var-list (map (lambda (x) (cons 'decl (cdr x))) var-list))
(define fresult (scan-func vresult))
(define final `(,@var-list ,@func-list ,fresult))
; (println final)
(define ret (for-each emit final))