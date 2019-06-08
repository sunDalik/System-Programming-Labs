;---convenient macros---
%define _lw 0
%macro native 3 ;%1 - word name, %2 - word label, %3 - immediate flag
    section .data
    wh_ %+ %2 : dq _lw
    db 0, %1, 0, %3
    
    %define _lw wh_%+ %2 
    xt_ %+ %2 :  dq i_ %+ %2 
    section .text
    i_ %+ %2:
%endmacro

%macro native 2 ;immediate flag is 0 by default
native %1, %2, 0
%endmacro


;---stack combinators?---

;last letter [c in (a b c)] in stack diagrams marks top of the stack 
;(a b -- a) 
native "drop", drop
    add rsp, 8
    jmp next

;(a b -- b a)
native "swap", swap
    pop rax
    pop rdx
    push rax
    push rdx
    jmp next

;(a -- a a)
native "dup", dup
    push qword [rsp]
    jmp next

	
;---arithmetic operators---

native "+", plus
    pop rax
    add [rsp], rax
    jmp next

native "*", mul
    pop rax
    pop rdx
    imul rdx
    push rax
    jmp next

;(a b -- a/b) integer division
;notice that stack top is the second operand in arithmetic operations
native "/", div
    pop rcx
    pop rax
    xor rdx, rdx
    idiv rcx
    push rax
    jmp next

;(a b -- a%b) remainder of division
native "%", mod
    pop rcx
    pop rax
	xor rdx, rdx
    idiv rcx
    push rdx
    jmp next

;(a b -- a-b) 
native "-", minus
    pop rax
    sub [rsp], rax
    jmp next

native "=", equals
    pop rax
    pop rdx
    cmp rax, rdx
    sete al
    movzx rax, al
    push rax
    jmp next

native "<", less
    pop rdx
    pop rax
    cmp rax, rdx
    setl al
    movzx rax, al
    push rax
    jmp next
	

;---logical operators---
	
native "not", not
    pop rax
    test rax, rax
    setz al
    movzx rax, al
    push rax
    jmp next

;bitwise and
native "and", and
    pop rax
    and [rsp], rax
    jmp next

;bitwise or
native "or", or
    pop rax
    or [rsp], rax
    jmp next
	
;logical and, expects both arguments to be 1 or 0
native "land", land
    pop rax
    pop rdx
    test rax, rax
    je .no
    push rdx
    jmp next
.no:
    push rax
    jmp next

;logical or, expects both arguments to be 1 or 0
native "lor", lor
    pop rax
    pop rdx
    test rax, rax
    jne .yes
    push rdx
    jmp next
.yes:
    push rax
    jmp next