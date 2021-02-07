gdt_start:

; CPU requires that the first entry of the GDT will be an empty entry.
; Used to identify mistakes where a developer forget to initialize the 
; segment register. Accessing the first GDT entry will cause a CPU exception.
gdt_null:
    dq 0x0

; Code Segment
gdt_code:
    ; Base = 0x0, limit = 0xFFFFF.
    ; Flags:
    ;   - 1st flags (1001b): Present(1), Privilege(00), Type(1)
    ;   - Type flags (1010b): Code(1), Conforming(0), Readable(1), Accessed(0)
    ;   - 2nd flags (1100b): Granularity(1), 32-bit Default(1), 64-bit Seg(0), AVL(0).
    dw 0xFFFF       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 10011010b    ; 1st flags + type flags
    db 11001111b    ; 2nd flags + Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

; Data Segment
gdt_data:
    ; Flags:
    ;   ** Identical to the code segmentt except for the type flags. **
    ;   - Type flags (0010b): Code(0), Expand Down(0), Writable(1), Accessed(0)
    dw 0xFFFF       ; Limit (bits 0-15)        
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 10010010b    ; 1st flags + type flags        
    db 11001111b    ; 2nd flags + Limit (bits 16-19)    
    db 0x0          ; Base (bits 24-31)

; Label is used for the assembler to be able to calculate the gdt size.
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; GDT size
    dd gdt_start                ; 8 byte address points to the start of the GDT

; Define handy constants for the
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start