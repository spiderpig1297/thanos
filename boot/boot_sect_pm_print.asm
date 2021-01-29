[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; Prints a null-terminated string pointed to by EBX.
print_string_pm:
    pusha

    mov edx, VIDEO_MEMORY

iterate_string_pm:
    mov al, [ebx]   
    mov ah, WHITE_ON_BLACK  ; Store the character attributes

    cmp al, 0       ; If we have reached the end, finish.
    je end_print_string_pm

    mov [edx], ax   ; Store char and its attributes at current character cell.
    inc ebx
    add edx, 2      ; Since each cell in the VGA memory is 2 bytes (stores the
                    ; ASCII value of the character and its attributes) we want
                    ; to increment edx by 2.

    jmp iterate_string_pm

end_print_string_pm:
    popa
    ret
