build/lexer.h: build/lang.l
	cd build && flex lang.l

build/lexer.c: build/lang.l
	cd build && flex lang.l

build/parser.c: build/lang.y
	bison -o build/parser.c -d -v build/lang.y

build/parser.h: build/lang.y
	bison -o build/parser.c -d -v build/lang.y

build/lang.o: build/lang.c build/lang.h
	gcc -c build/lang.c -o build/lang.o

build/parser.o: build/parser.c build/parser.h build/lexer.h build/lang.h
	gcc -c build/parser.c -o build/parser.o

build/lexer.o: build/lexer.c build/lexer.h build/parser.h build/lang.h
	gcc -c build/lexer.c -o build/lexer.o

build/main.o: build/main.c build/lexer.h build/parser.h build/lang.h
	gcc -c build/main.c -o build/main.o

bin/main: build/lang.o build/parser.o build/lexer.o build/main.o
	gcc build/lang.o build/parser.o build/lexer.o build/main.o -o bin/main

all: bin/main

clean:
	rm -rf build/*

%.c: %.y

%.c: %.l

build/% : src/%
	cp src/* build

.DEFAULT_GOAL := all

