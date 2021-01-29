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
