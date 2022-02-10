# My Linux Kernel (RISC-V)

This is a simple linux kernel on RISC-V. This kernel completed the exception modula, process manager and virtual memory manager.

The major modula and its functions are as follow.

| file                       | functions                     |
|----------------------------|-------------------------------|
| /arch/riscv/kernel/head.S  | the entry of the whole system |
| /arch/riscv/kernel/trap.c  | handle exceptions             |
| /arch/riscv/kernel/clock.c | control clock exceptions      |
| /arch/riscv/kernel/entry.S | change the process            |
| /arch/riscv/kernel/proc.c  | manage the process            |
| /arch/riscv/kernel/vm.c    | manage the virtual memory     |
