ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

;; This is loaded at 0x07c0:00
_start:
    ;; Set BIOS parameter block
    jmp short endParam
    nop
    times 33 db 0

endParam:
    jmp 0:start ; Sets CS to 0x7c0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Bootloader
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

start:
    cli
    ;; Set segments
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

.load_protected:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; GDT - Global Descriptor Table
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

gdt_start:
gdt_null:
    dd 0
    dd 0

; Offset 0x08
gdt_code:     ; CS should point to this
    dw 0xffff ; Segment limit: first 0-15 bits
    dw 0      ; Base: first 0-15 bits
    db 0      ; Base: 16-23 bits
    db 0x9a   ; Access byte
    db 11001111b ; High and low 4-bit flags
    db 0      ; Base 24-32 bits

; Offset 0x10
gdt_data:     ; DS, ES, SS, FS, GS
    dw 0xffff ; Segment limit: first 0-15 bits
    dw 0      ; Base: first 0-15 bits
    db 0      ; Base: 16-23 bits
    db 0x92   ; Access byte
    db 11001111b ; High and low 4-bit flags
    db 0      ; Base 24-32 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax 
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp
    jmp $

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; End of file
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

times 510-($-$$) db 0 ; Fill remaining space with 0
dw 0xAA55

buffer: