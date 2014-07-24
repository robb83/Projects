#!/bin/sh

if [ ! -f c_prime ]; then
	gcc -std=c99 -O2 -Wall -o c_prime c_prime.c -lm
fi

if [ ! -f nasm_prime ]; then
	nasm -f elf nasm_prime.s
	ld -melf_i386 -o nasm_prime -e _start nasm_prime.o
fi

if [ ! -f mono_prime.exe ]; then
	mono-csc mono_prime.cs
fi

if [ ! -f go_prime ]; then
	go build go_prime.go
fi

if [ ! -f python_prime.pyc ]; then
	python -m compileall python_prime.py
fi

/usr/bin/time -v ./nasm_prime 2>> nasm_prime_result.txt
/usr/bin/time -v ./c_prime 2>> c_prime_result.txt
/usr/bin/time -v ./go_prime 2>> go_prime_result.txt
/usr/bin/time -v ./mono_prime.exe 2>> mono_prime_result.txt
/usr/bin/time -v python python_prime.pyc 2>> python_prime_result.txt
/usr/bin/time -v pypy python_prime.py 2>> python_pypy_prime_result.txt
/usr/bin/time -v php php_prime.php 2>> php_prime_result.txt
/usr/bin/time -v hhvm php_prime.php 2>> php_hhvm_prime_result.txt
/usr/bin/time -v nodejs nodejs_prime.js 2>> nodejs_prime_result.txt
