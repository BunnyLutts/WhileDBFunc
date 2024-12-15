# WhileDBFunc

[TOC]

A repo for group assignment of PL. An Implementation of While Lang with Deference, Built-in functions and ordinary Functions.

## Progress

- [x] Lexer
- [x] Parser
- [x] Interpreter Framework
- [x] Interpreter
- [x] Robust & Correctness Test

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

## How to compile


``` bash
git clone git@github.com:BunnyLutts/WhileDBFunc.git

cd WhileDBFunc/

make
```

### Dependencies

* gcc
* make
* flex
* bison
* clang-format
* nushell
* racket

## How to use the interpreter 
You may write you WhileDB program in filename.src

after finish compiling , run

``` bash
./bin/main ${path_to_your_WhileDB_file}
```
then the running results will be printed in the terminal.

We have prepared some WhileDB source code, which are in the ' raw_tests ' folder, 
you can use
``` bash
./bin/main ./raw_tests/${test_name}.src
```
to run them


## How to test 

### Once for all

If you need to run tests and verifications, please install [`nushell`](https://www.nushell.sh/zh-CN/book/installation.html). 
If you use `npm`, you can simply run `make depend` to install nushell. 

After install, you can run:

```nu
make test_all
```

to get a report of all tests in the `tests/` folder.

To do single tests and verifications, you need to install `racket` and `clang`. 
Then enter the `nushell` environment

```bash
nu
````

run this command

```nu
source scripts/utility.nu
```

We have two functions in `utility.nu`. `test_one` accept a test file path and output a report; `w2c` convert the test into a `C` file.

you can use them to test correctness.

#### Warning : you shall not directly run the testfiles in 'tests' using the interpreter. 
#### Instead, you can run those in 'raw_tests'.

### Single Point

If you want to test specific single point, you may refer to the "How to use interpreter" Section.

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

### ATTENTION When using IO
when using `read_char()` and `read_int()`, please press ENTER after input. 

Actually we use `scanf(" %c", &ch)` to receive input. So naturally we'll ignore all the blank characters from your input.

### ATTENTION When allocating memories
The interpreter would free all the memory when exiting. However, you should be careful not to allocate too much memory(including too many variables and too many function calls and too many calculations).

### ATTENTION When assigning values
You can do like `1+1 = 2`. However, this does not make any sense.