MODE=RELEASE

ifeq ($(MODE), DEBUG)
CFLAGS = -g3 -DDEBUG_MODE
else
CFLAGS = -O2
endif

build/lexer.h: build/lang.l
	cd build && flex lang.l

build/lexer.c: build/lang.l
	cd build && flex lang.l

build/parser.c: build/lang.y
	bison -o build/parser.c -d -v build/lang.y

build/parser.h: build/lang.y
	bison -o build/parser.c -d -v build/lang.y

build/lang.o: build/lang.c build/lang.h
	gcc -c build/lang.c -o build/lang.o $(CFLAGS)

build/parser.o: build/parser.c build/parser.h build/lexer.h build/lang.h
	gcc -c build/parser.c -o build/parser.o $(CFLAGS)

build/lexer.o: build/lexer.c build/lexer.h build/parser.h build/lang.h
	gcc -c build/lexer.c -o build/lexer.o $(CFLAGS)

build/stack.o: build/stack.c build/stack.h
	gcc -c build/stack.c -o build/stack.o $(CFLAGS)

build/mem.o: build/mem.c build/mem.h
	gcc -c build/mem.c -o build/mem.o $(CFLAGS)

build/utility.o: build/utility.c build/utility.h build/mem.h
	gcc -c build/utility.c -o build/utility.o $(CFLAGS)

build/interpreter.o: build/interpreter.h build/utility.h
	gcc -c build/interpreter.c -o build/interpreter.o $(CFLAGS)

build/main.o: build/main.c build/lexer.h build/parser.h build/lang.h
	gcc -c build/main.c -o build/main.o $(CFLAGS)

bin/main: build/lang.o build/parser.o build/lexer.o build/main.o build/utility.o build/stack.o build/interpreter.o build/mem.o bin/
	gcc build/lang.o build/parser.o build/lexer.o build/main.o build/utility.o build/stack.o build/interpreter.o build/mem.o -o bin/main

all: bin/main

clean:
	rm -rf build/*
	rm -rf test_result.json
	rm -rf bin/*

test:
	nu auto_test.nu

depend:
	npm install -g nushell
	
%.c: %.y

%.c: %.l

build/ :
	mkdir build

build/% : src/% build/
	cp src/* build

bin/ :
	mkdir bin
	
.DEFAULT_GOAL := all

