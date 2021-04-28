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

;; Load global descriptor
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax ; Switch to 32 bit mode
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load kernel
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 32]
load32:
    mov eax, 1 ; Sector start number
    mov ecx, 100 ; Number of sectors
    mov edi, 0x0100000 ; Where to load in memory
    call ata_lba_read
    jmp CODE_SEG:0x0100000 ; Jump the kernel


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Read from HDD
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ata_lba_read:
    mov ebx, eax ; Backup LBA for later.

    ; Send the highest 8 bits of the LBA to the HDD controller
    shr eax, 24
    or eax, 0xe0 ; Select master drive
    mov dx, 0x1f6 ; HDD port
    out dx, al ; Send to port

    ; Send the number of sectors to read to the HDD controller
    mov eax, ecx
    mov dx, 0x1f2
    out dx, al

    ; Send more bits of the LBA
    mov eax, ebx ; Restore backup
    mov dx, 0x1f3
    out dx, al

    ; Send more more bits of the LBA
    mov dx, 0x1f4
    mov eax, ebx ; Restore again for safety
    shr eax, 8
    out dx, al

    ; Send upper 16 bits of the LBA
    mov dx, 0x1f5
    mov eax, ebx ; Restore again for safety
    shr eax, 16
    out dx, al

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

.next_sector:
    push ecx

; Check if we need to read.
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

    ; Read 256 words at a time (one sector)
    mov ecx, 256
    mov dx, 0x1f0
    rep insw

    ; Loop decrements the number of sectors.
    pop ecx
    loop .next_sector
    ; Return when done.
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; End of file
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

times 510-($-$$) db 0 ; Fill remaining space with 0
dw 0xAA55
