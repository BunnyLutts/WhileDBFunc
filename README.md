# WhileDBFunc

A repo for group assignment of PL. An Implementation of While Lang with Deference, Built-in functions and ordinary Functions.

## Progress

- [x] Lexer
- [x] Parser
- [x] Interpreter Framework
- [ ] Interpreter
- [ ] Robust & Correctness Test

## How to contribute (For team members)

``` bash
git clone git@github.com:BunnyLutts/WhileDBFunc.git
cd WhileDBFunc
git checkout -b dev_${Your_name}
```

Do some work, and:

``` bash
git add ${Something}
git commit -m "${Message}"
git push
```
## How to test

If you need to use `auto_test.nu` to run auto tests, please install [`nushell`](https://www.nushell.sh/zh-CN/book/installation.html). 
If you use `npm`, you can simply run `make depend` to install nushell. 
After install, you can run:

```bash
make test
```

You will get a json like `test_result` file and a table terminal output.

## Language Specification

This language spec is only used for explain syntax, not absolutely applied in implement. The syntax rules are written in regex way.

```
ident     = [_a-zA-Z][_0-9a-zA-Z]
num       = 0|[1-9][0-9]*

prog      = cmd_seq
block     = "{" cmd_seq "}"

cmd_seq   = empty
          | (cmd ";")* cmd

cmd       = cmd_decl
          | cmd_asgn
          | cmd_if
          | cmd_while
          | cmd_fdecl
          | cmd_fcall
          | cmd_ret

cmd_decl  = "var" ident
cmd_asgn  = lval "=" expr
cmd_if    = "if" expr "then" block "else" block
cmd_while = "while" expr "do" block
cmd_fdecl = "func" ident "(" id_list ")" block
cmd_fcall = fcall
cmd_ret   = "return" (expr)+

fcall     = ident "(" expr_list ")"

expr_list = empty
          | (expr ",")* expr
id_list   = empty
          | (ident ",")* ident

expr      = num
          | lval
          | expr binop expr
          | unop expr
          | fcall
          | "(" expr ")"

lval      = ident
          | deref expr
          | "(" lval ")"
          
binop     = "+" | "-" | "*" | "/" | "%" | "<" | ">" 
          | "<=" | "=>" | "=" | "!=" | "&&" | "||"
unop      = "-" | "!"
deref     = "*"
```