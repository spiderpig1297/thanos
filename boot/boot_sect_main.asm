[org 0x7C00]
KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl    ; BIOS stores out boot drive in DL

mov bp, 0x9000  ; Initialize the stack
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call load_kernel    ; Load our kernel.

call switch_to_pm

jmp $

%include "boot/boot_sect_print.asm"
%include "boot/boot_sect_print_hex.asm"
%include "boot/boot_sect_disk.asm"
%include "boot/boot_sect_gdt_def.asm"
%include "boot/boot_sect_pm_switch.asm"
%include "boot/boot_sect_pm_print.asm"

[bits 16]

load_kernel:
    pusha

    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET
    mov dh, 2
    mov dl, [BOOT_DRIVE]
    call disk_load
    
    popa

    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    
    call KERNEL_OFFSET

    jmp $

; Global variables
BOOT_DRIVE      db 0
MSG_REAL_MODE   db "Entered 16-bit Real Mode...", 0
MSG_PROT_MODE   db "Successfully landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0

; Bootsector padding.
times 510-($-$$) db 0
dw 0xAA55