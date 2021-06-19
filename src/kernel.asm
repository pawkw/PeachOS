[BITS 32]
global _start

extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Set up protected 32 bit mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax 
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

;; Enable address line 20 (A20)
;; See https://wiki.osdev.org/A20_Line#Recommended_Method for a better way.
    in al, 0x92
    or al, 2
    out 0x92, al

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Call the C kernel
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    call kernel_main

    jmp $

;; Fill end of file to 512 byte boundry
times 512-($ - $$) db 0
