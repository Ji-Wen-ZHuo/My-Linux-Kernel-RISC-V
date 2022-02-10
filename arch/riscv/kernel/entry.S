#include <defs.h>

    .section .text.entry
    .align 2
    .globl _traps 
	.globl __dummy
	.globl __switch_to 
    .equ reg_size, 0x8
_traps:
	addi sp, sp, -32*reg_size
	sd x1, 1*reg_size(sp)
	sd x2, 2*reg_size(sp)
	sd x3, 3*reg_size(sp)
	sd x4, 4*reg_size(sp)
	sd x5, 5*reg_size(sp)
	sd x6, 6*reg_size(sp)
	sd x7, 7*reg_size(sp)
	sd x8, 8*reg_size(sp)
	sd x9, 9*reg_size(sp)
	sd x10, 10*reg_size(sp)
	sd x11, 11*reg_size(sp)
	sd x12, 12*reg_size(sp)
	sd x13, 13*reg_size(sp)
	sd x14, 14*reg_size(sp)
	sd x15, 15*reg_size(sp)
	sd x16, 16*reg_size(sp)
	sd x17, 17*reg_size(sp)
	sd x18, 18*reg_size(sp)
	sd x19, 19*reg_size(sp)
	sd x20, 20*reg_size(sp)
	sd x21, 21*reg_size(sp)
	sd x22, 22*reg_size(sp)
	sd x23, 23*reg_size(sp)
	sd x24, 24*reg_size(sp)
	sd x25, 25*reg_size(sp)
	sd x26, 26*reg_size(sp)
	sd x27, 27*reg_size(sp)
	sd x28, 28*reg_size(sp)
	sd x29, 29*reg_size(sp)
	sd x30, 30*reg_size(sp)
	sd x31, 31*reg_size(sp)

	csrr t0, sepc
	sd t0, 32*reg_size(sp)

	csrr a0, scause
	csrr a1, sepc
    call trap_handler

    ld t0, 32*reg_size(sp)
    csrw sepc, t0
    
	ld x1, 1*reg_size(sp)
	ld x3, 3*reg_size(sp)
	ld x4, 4*reg_size(sp)
	ld x5, 5*reg_size(sp)
	ld x6, 6*reg_size(sp)
	ld x7, 7*reg_size(sp)
	ld x8, 8*reg_size(sp)
	ld x9, 9*reg_size(sp)
	ld x10, 10*reg_size(sp)
	ld x11, 11*reg_size(sp)
	ld x12, 12*reg_size(sp)
	ld x13, 13*reg_size(sp)
	ld x14, 14*reg_size(sp)
	ld x15, 15*reg_size(sp)
	ld x16, 16*reg_size(sp)
	ld x17, 17*reg_size(sp)
	ld x18, 18*reg_size(sp)
	ld x19, 19*reg_size(sp)
	ld x20, 20*reg_size(sp)
	ld x21, 21*reg_size(sp)
	ld x22, 22*reg_size(sp)
	ld x23, 23*reg_size(sp)
	ld x24, 24*reg_size(sp)
	ld x25, 25*reg_size(sp)
	ld x26, 26*reg_size(sp)
	ld x27, 27*reg_size(sp)
	ld x28, 28*reg_size(sp)
	ld x29, 29*reg_size(sp)
	ld x30, 30*reg_size(sp)
	ld x31, 31*reg_size(sp)
	ld x2, 2*reg_size(sp)
	addi sp, sp, 32*reg_size
        
	sret

__dummy:
	li t1, VM_START + OPENSBI_SIZE # base
	li t0, %lo(dummy)
	add t0, t0, t1
	csrw sepc, t0
	sret

__switch_to:
	sd ra, 5*reg_size(a0)
	sd sp, 6*reg_size(a0)
	sd s0, 7*reg_size(a0)
	sd s1, 8*reg_size(a0)
	sd s2, 9*reg_size(a0)
	sd s3, 10*reg_size(a0)
	sd s4, 11*reg_size(a0)
	sd s5, 12*reg_size(a0)
	sd s6, 13*reg_size(a0)
	sd s7, 14*reg_size(a0)
	sd s8, 15*reg_size(a0)
	sd s9, 16*reg_size(a0)
	sd s10, 17*reg_size(a0)
	sd s11, 18*reg_size(a0)
	
	ld ra, 5*reg_size(a1)
	ld sp, 6*reg_size(a1)
	ld s0, 7*reg_size(a1)
	ld s1, 8*reg_size(a1)
	ld s2, 9*reg_size(a1)
	ld s3, 10*reg_size(a1)
	ld s4, 11*reg_size(a1)
	ld s5, 12*reg_size(a1)
	ld s6, 13*reg_size(a1)
	ld s7, 14*reg_size(a1)
	ld s8, 15*reg_size(a1)
	ld s9, 16*reg_size(a1)
	ld s10, 17*reg_size(a1)
	ld s11, 18*reg_size(a1)
	ret
