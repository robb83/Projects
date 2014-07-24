#!/bin/sh

if [ ! -f c_empty ]; then
	gcc -std=c99 -O2 -Wall -o c_empty c_empty.c
fi

if [ ! -f nasm_empty ]; then
	nasm -f elf nasm_empty.s
	ld -melf_i386 -o nasm_empty -e _start nasm_empty.o
fi

if [ ! -f mono_empty.exe ]; then
	mono-csc mono_empty.cs
fi

if [ ! -f go_empty ]; then
	go build go_empty.go
fi

if [ ! -f python_empty.pyc ]; then
	python -m compileall python_empty.py
fi

/usr/bin/time -v ./nasm_empty 2>> nasm_empty_result.txt
/usr/bin/time -v ./c_empty 2>> c_empty_result.txt
/usr/bin/time -v ./go_empty 2>> go_empty_result.txt
/usr/bin/time -v ./mono_empty.exe 2>> mono_empty_result.txt
/usr/bin/time -v python python_empty.pyc 2>> python_empty_result.txt
/usr/bin/time -v pypy python_empty.py 2>> python_pypy_empty_result.txt
/usr/bin/time -v php php_empty.php 2>> php_empty_result.txt
/usr/bin/time -v hhvm php_empty.php 2>> php_hhvm_empty_result.txt
/usr/bin/time -v nodejs nodejs_empty.js 2>> nodejs_empty_result.txt
