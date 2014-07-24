; nasm -f elf nasm_prime32.s || ld -melf_i386 -o nasm_prime32 -e _start nasm_prime32.o
bits 32
section     .text
global      _start

_isprime:
	cmp ecx, 10
	jae _isprime_bigger_then_10
	cmp ecx, 0
	jz _isprime_false
	cmp ecx, 1
	jz _isprime_false
	cmp ecx, 2
	jz _isprime_true
	cmp ecx, 3
	jz _isprime_true
	cmp ecx, 5
	jz _isprime_true
	cmp ecx, 7
	jz _isprime_true
	cmp ecx, 9
	jz _isprime_false
		
_isprime_bigger_then_10:

	; ecx % 2 == 0
	xor edx, edx
	mov eax, ecx
	mov ebx, 2
	div ebx
	cmp edx, 0
	jz _isprime_false
	
	; ecx % 5 == 0
	xor edx, edx
	mov eax, ecx
	mov ebx, 5
	div ebx
	cmp edx, 0
	jz _isprime_false
	
	; sqrt
	mov dword [temp], ecx
	fild dword [temp]
	fsqrt
	fistp dword [temp]
	mov ebx, [temp]
		
	; edi = 3
	mov edi, 3
_isprime_loop:

	; ecx % edi == 0
	xor edx, edx
	mov eax, ecx
	div edi
	cmp edx, 0
	jz _isprime_false
	
	; edi += 2
	add edi, 2
	
	; edi <= ebx
	cmp edi, ebx
	jle _isprime_loop

_isprime_true:
	mov eax, 1
	ret
	
_isprime_false:
	mov eax, 0
	ret
	
;;; PRINT PRIMES ;;;

_print_primes:
	
	mov eax, [primes]
	mov ebx, 10
	mov ecx, buffer_length

	; end of line symbol
	dec ecx
	mov byte [buffer + ecx], 10	
	
_print_primes_loop:
	xor dx, dx
	div ebx
	add dl, 48
	dec ecx
	mov [buffer + ecx], dl
	cmp eax, 0
	jnz _print_primes_loop
	
	mov edx, buffer_length
	sub edx, ecx
	add ecx, buffer
	mov ebx, 1
	mov eax, 4
	int 0x80
	
	ret
	
;;; START ;;;
	
_start:
	finit
	mov dword [primes], 0
	mov ecx, 0

_start_loop:
	call _isprime
	cmp eax, 0
	jz _start_loop_continue
	
	inc dword [primes]

_start_loop_continue:
	inc ecx
	cmp ecx, 5000011
	jnz _start_loop
	
	call _print_primes

exit:
	mov ebx, [primes]
    mov eax, 1
    int 0x80

;;; DATA SEGMENT ;;;

segment .data
	primes: dd 0
	temp: dd 0
	buffer: times 64 db 0
	buffer_length: equ $-buffer
