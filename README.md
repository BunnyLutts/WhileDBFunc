# WhileDBFunc

A repo for group assignment of PL. An Implementation of While Lang with Deference, Built-in functions and ordinary Functions.

## Progress

- [x] Lexer
- [x] Parser
- [x] Interpreter Framework
- [x] Interpreter
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

You may enconter problems when executing the above commands. Then

``` bash
git push --set-upstream origin dev_${Your_name}
```

When you want to sync with the main branch:

``` bash
git checkout main
git pull
git checkout dev_${Your_name}
git merge main
```

You shall NEVER directly change the main branch or push the main branch.

## How to test

Then, if you need to =to run auto tests, please install [`nushell`](https://www.nushell.sh/zh-CN/book/installation.html). 
If you use `npm`, you can simply run `make depend` to install nushell. 
After install, you can run:

```bash
make test_all
```

This command will build a debug version and out put a json like `test_result` file.

If you want to test on one test, you need to use `nu` as shell. Firstly import the function:

```bash
nu
source scripts/test_one.nu
```

Then prepare a debug version:

```nu
make MODE=DEBUG
```

Then use

```nu
test_one $file_name
```

To get a json like output.

## Cross Verification

To ensure correctness, we need to use some method to simulate our results.

Now the method is using `racket` to lex and parse `.src`, then convert it to `C` (or directly run it in `racket`)

I must say, it's much easier to write a lexer & parser in functional lanuages!

*Progress*

- [x] lexer
- [ ] parser
- [ ] c-converter / interpreter

`Racket` is a sibling of `Scheme`, so you should be familier with it.

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