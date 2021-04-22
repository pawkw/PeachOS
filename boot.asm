ORG 0
BITS 16

;; This is loaded at 0x07c0:00
_start:
    ;; Set BIOS parameter block
    jmp short endParam
    nop
    times 33 db 0

endParam:
    jmp 0x7c0:start ; Sets CS to 0x7c0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Interrupts
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

div_by_zero:
    mov si, zeroMsg
    call print
    ; mov ah, 0eh
    ; mov al, 'Z'
    ; mov bx, 0
    ; int 0x10
    iret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Bootloader
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
start:
    cli
    ;; Set segments
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0
    mov ss, ax
    mov sp, 0x7c00
    sti

    ;; Set interrupts
    mov word[ss:0x00], div_by_zero
    mov word[ss:0x02], 0x7c0

    ;; Read sector 2 from disk then contents
    mov ah, 2 ; Read disk interrupt
    mov al, 1 ; One sector
    mov ch, 0 ; Cylinder low eight bits
    mov cl, 2 ; Sector 2
    mov dh, 0 ; Head number 0
    ;; dl, the drive number, is already set from boot
    mov bx, buffer
    int 0x13
    jc .readError ; Carry flag is set if there is an error
    mov si, buffer
    call print
    jmp $

.readError:
    mov si, diskReadErr
    call print
    ;; Loop forever!!!
    jmp $

print:
    mov bx, 0 ; Code page 0
.loop:
    lodsb
    cmp al, 0
    je .done
    cmp al, 13
    je .loop
    cmp al, 10
    jne .cont
    call printchar
    mov al, 13
.cont:
    call printchar
    jmp .loop
.done:
    ret

printchar:
    mov ah, 0eh
    int 0x10
    ret
    
zeroMsg: db 'Divide by zero error.',10,13,0
diskReadErr: db 'Failed to load sector.',10,13,0

times 510-($-$$) db 0 ; Fill remaining space with 0
dw 0xAA55

buffer: