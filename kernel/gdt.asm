;;
; Global Descriptor Table managing
;
; @author Ernesto Martínez García <me@ecomaikgolf.com>
;

; tell nasm we need 64 bit instructions
[bits 64]

;
; void load_gdt(gdt_ptr*)
;
load_gdt:
	lgdt [rdi] ; rdi contains first parameter passed to load_gdt
	mov ax, 0x10 ; gdt kernel data segment
	mov ds, ax ; set data segments
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	pop rdi ; get return addr (from stack) to rdi register
	mov rax, 0x08 ; gdt kernel code segment
	push rax
	push rdi
	retfq ; far 64bit return (pops address and code segment)

; make it accessible for other code
GLOBAL load_gdt
