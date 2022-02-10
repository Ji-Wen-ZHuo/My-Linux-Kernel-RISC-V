#include <defs.h>

.extern start_kernel

    .section .text.init
    .globl _start
_start:
    la sp, boot_stack_top

	call setup_vm
	call relocate
    
    call mm_init
    call setup_vm_final
	call task_init
	
	li t1, VM_START + OPENSBI_SIZE # base
	
	li t0, %lo(_traps)
	add t0, t0, t1
	csrw stvec, t0

	li t0, 0x20
	csrs sie, t0
	
	call clock_set_next_event

	li t0, 2
	csrs sstatus, t0
    call start_kernel

relocate:
	li t0, PA2VA_OFFSET
	add ra, ra, t0
	add sp, sp, t0

	la t0, early_pgtbl
	srli t0, t0, 12
	li t1, 0x8000000000000000
	add t0, t0, t1
	csrw satp, t0

	sfence.vma zero, zero
	ret

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 * 4 # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top:
