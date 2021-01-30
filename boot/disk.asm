; Load DH sectors to ES:BX from drive DL.
disk_load:
    pusha 

    push dx         ; Store dx on stack to recall how many sectors
                    ; were requested to be read.
    mov ah, 0x02    ; BIOS read sector function
    mov al, dh      ; Read DH sectors.
    mov ch, 0x00    ; Select cylinder 0.
    mov dh, 0x00    ; Select head 0.
    mov cl, 0x02    ; Start reading from the 2th sector on the track (i.e 
                    ; after the boot sector).

    int 0x13 

    ; If the BIOS has failed to read the disk, the Carry flag is set
    ; and al contains the number of sectors read.
    jc disk_error

    pop dx
    cmp dh, al
    jne disk_error

    popa
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    mov dl, ah
    call print_hex
    jmp $


DISK_ERROR_MSG:     db "Disk read error: ", 0