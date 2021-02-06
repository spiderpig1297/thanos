KERNEL_DS equ 0x10

[extern isr_handler]
isr_common_stub:
    ; 1. Save CPU state
    pusha

    ; Save lower 16-bit of the data segment
    mov ax, ds
    push eax

    mov ax, KERNEL_DS
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; 2. Call C handler
    call isr_handler

    ; 3. Restore state
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8  ; Cleans up pushed error code and ISR number
    sti         ; Set Interrupt Flag instruction
    iret        ; Pop CS, EIP, EFLAGS, SS and ESP which were pushed automatically 
                ; by the CPU when the interrupt was fired.

[extern irq_handler]
irq_common_stub:
    ; 1. Save CPU state
    pusha

    ; Save lower 16-bit of the data segment
    mov ax, ds
    push eax

    mov ax, KERNEL_DS
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; 2. Call C handler
    call irq_handler

    ; 3. Restore state
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8  ; Cleans up pushed error code and ISR number
    sti         ; Set Interrupt Flag instruction
    iret        ; Pop CS, EIP, EFLAGS, SS and ESP which were pushed automatically 
                ; by the CPU when the interrupt was fired.


; 0: Divide By Zero
; #DE, Fault
global isr_divide_by_zero
isr_divide_by_zero:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

; 1: Debug Exception
; #DB, Fault / Trap
global isr_debug
isr_debug:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

; 2: Non Maskable Interrupt
; N/a, Interrupt
global isr_nmi
isr_nmi:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

; 3: Breakpoint (INT3 instruction)
; #BP, Trap
global isr_breakpoint
isr_breakpoint:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

; 4: Overflow (INTO instruction)
; #OF, Trap
global isr_overflow
isr_overflow:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

; 5: Out of Bounds
; #BR, Fault
global isr_boundrange
isr_boundrange:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

; 6: Invalid Opcode (UD2 opcode)
; #UD, Fault
global isr_invalid_opcode
isr_invalid_opcode:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

; 7: Device Not Available
; #NM, Fault
global isr_device_not_available
isr_device_not_available:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

; 8: Double Fault
; #DF, Abort, Built-in error code (0)
global isr_double_fault
isr_double_fault:
    cli
    push byte 8
    jmp isr_common_stub

; 9: Coprocessor Segment Overrun
; N/a, Fault
global isr_segment_overrun
isr_segment_overrun:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Invalid TSS
; #TS, Fault, Built-in error code
global isr_invalid_tss
isr_invalid_tss:
    cli
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present
; #NP, Fault, Built-in error code
global isr_segment_not_present
isr_segment_not_present:
    cli
    push byte 11
    jmp isr_common_stub

; 12: Stack Fault Exception
; #SS, Fault, Built-in error code
global isr_stack_segment_fault
isr_stack_segment_fault:
    cli
    push byte 12
    jmp isr_common_stub

; 13: General Protection Fault
; #GP, Fault, Built-in error code
global isr_general_protection
isr_general_protection:
    cli
    push byte 13
    jmp isr_common_stub

; 14: Page Fault
; #PF, Fault, Built-in error code
global isr_page_fault
isr_page_fault:
    cli
    push byte 14
    jmp isr_common_stub

; 15: Intel-reserved. 
global isr_reserved_15
isr_reserved_15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Floating Point Error
; #MF, Fault
global isr_fpe
isr_fpe:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check
; #AC, Fault, Built-in error code (0)
global isr_alignment_check
isr_alignment_check:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine Check
; #MC, Abort
global isr_machine_check
isr_machine_check:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: SIMD FPE Exception
; #XM, Fault
global isr_simd_fpe
isr_simd_fpe:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; TODO: implement
global isr_syscall
isr_syscall:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

global irq0
irq0:
    cli
    push byte 0
    push byte 32
    jmp irq_common_stub

global irq1
irq1:
    cli
    push byte 0
    push byte 33
    jmp irq_common_stub

global irq2
irq2:
    cli
    push byte 0
    push byte 34
    jmp irq_common_stub

global irq3
irq3:
    cli
    push byte 0
    push byte 35
    jmp irq_common_stub

global irq4
irq4:
    cli
    push byte 0
    push byte 36
    jmp irq_common_stub

global irq5
irq5:
    cli
    push byte 0
    push byte 37
    jmp irq_common_stub

global irq6
irq6:
    cli
    push byte 0
    push byte 38
    jmp irq_common_stub

global irq7
irq7:
    cli
    push byte 0
    push byte 39
    jmp irq_common_stub

global irq8
irq8:
    cli
    push byte 0
    push byte 40
    jmp irq_common_stub

global irq9
irq9:
    cli
    push byte 0
    push byte 41
    jmp irq_common_stub

global irq10
irq10:
    cli
    push byte 0
    push byte 42
    jmp irq_common_stub

global irq11
irq11:
    cli
    push byte 0
    push byte 43
    jmp irq_common_stub

global irq12
irq12:
    cli
    push byte 0
    push byte 44
    jmp irq_common_stub

global irq13
irq13:
    cli
    push byte 0
    push byte 45
    jmp irq_common_stub

global irq14
irq14:
    cli
    push byte 0
    push byte 46
    jmp irq_common_stub

global irq15
irq15:
    cli
    push byte 0
    push byte 47
    jmp irq_common_stub