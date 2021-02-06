[extern isr_handler]

KERNEL_DS equ 0x10

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

global isr_divide_by_zero
global isr_debug
global isr_nmi
global isr_breakpoint
global isr_overflow
global isr_boundrange
global isr_invalid_opcode
global isr_device_not_available
global isr_double_fault
global isr_segment_overrun
global isr_invalid_tss
global isr_segment_not_present
global isr_stack_segment_fault
global isr_general_protection
global isr_page_fault
global isr_reserved_15
global isr_fpe
global isr_alignment_check
global isr_machine_check
global isr_simd_fpe
global isr_virtualization_error
global isr_syscall

; 0: Divide By Zero
; #DE, Fault
isr_divide_by_zero:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

; 1: Debug Exception
; #DB, Fault / Trap
isr_debug:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

; 2: Non Maskable Interrupt
; N/a, Interrupt
isr_nmi:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

; 3: Breakpoint (INT3 instruction)
; #BP, Trap
isr_breakpoint:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

; 4: Overflow (INTO instruction)
; #OF, Trap
isr_overflow:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

; 5: Out of Bounds
; #BR, Fault
isr_boundrange:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

; 6: Invalid Opcode (UD2 opcode)
; #UD, Fault
isr_invalid_opcode:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

; 7: Device Not Available
; #NM, Fault
isr_device_not_available:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

; 8: Double Fault
; #DF, Abort, Built-in error code (0)
isr_double_fault:
    cli
    push byte 8
    jmp isr_common_stub

; 9: Coprocessor Segment Overrun
; N/a, Fault
isr_segment_overrun:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Invalid TSS
; #TS, Fault, Built-in error code
isr_invalid_tss:
    cli
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present
; #NP, Fault, Built-in error code
isr_segment_not_present:
    cli
    push byte 11
    jmp isr_common_stub

; 12: Stack Fault Exception
; #SS, Fault, Built-in error code
isr_stack_segment_fault:
    cli
    push byte 12
    jmp isr_common_stub

; 13: General Protection Fault
; #GP, Fault, Built-in error code
isr_general_protection:
    cli
    push byte 13
    jmp isr_common_stub

; 14: Page Fault
; #PF, Fault, Built-in error code
isr_page_fault:
    cli
    push byte 14
    jmp isr_common_stub

; 15: Intel-reserved. 
isr_reserved_15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Floating Point Error
; #MF, Fault
isr_fpe:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check
; #AC, Fault, Built-in error code (0)
isr_alignment_check:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine Check
; #MC, Abort
isr_machine_check:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: SIMD FPE Exception
; #XM, Fault
isr_simd_fpe:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Virtualization Exception
; #VE, Fault
isr_virtualization_error:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; TODO: implement
isr_syscall:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub