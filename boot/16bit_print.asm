print_string:
    pusha

; Prints all character in the string pointed by bx.
; If the current letter is 0, it means that we have 
; reached the end of the string and we want to finish.
iterate_string:
    mov al, [bx]
    cmp al, 0
    je end_print_string

    call print_single_char

    inc bx
    jmp iterate_string

end_print_string:
    popa
    ret 


print_newline:
    pusha 

    mov al, 0x0a ; Newline character
    call print_single_char
    mov al, 0x0d ; Carriage return
    call print_single_char

    popa
    ret


print_single_char:
    mov ah, 0x0e
    ; Assumes that al contains our letter.
    int 0x10
    ret


print_hex:
    pusha

    mov cx, 0

; Strategy:
; Get last character of "dx", and check whether it is numeric or alphabetical.
; Add the corresponding ASCII value (0x30 for numeric, 0x37 for alphabetical).
; Then, place the converted character in the result string, and repeat 3 more times.
; Finally, print the string.
hex_loop:
    cmp cx, 4
    je end_print_hex

    mov ax, dx
    and ax, 0x000F ; Get last character
    add al, 0x30
    cmp al, 0x39 ; If greater, it means that its a character.
    jle write_to_string
    add al, 0x7 ; 'A' in ASCII is 0x41, not 0x3A
    
write_to_string:
    mov bx, RESULT + 0x5
    sub bx, cx
    mov [bx], al
    ror dx, 4 ; Rotate 4 of the number's 16 bits (hence - one digit).
    inc cx
    jmp hex_loop

end_print_hex:
    mov bx, RESULT
    call print_string

    popa
    ret

RESULT:
    db '0x0000', 0