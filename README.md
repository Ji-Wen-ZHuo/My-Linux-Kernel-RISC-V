# Linux 内核实现 （RISC-V）

该项目实现了RISC-V版的Linux内核的部分功能，实现的功能有：时钟中断、线程调度、虚拟内存。

相关功能和对应的文件如下表

| 文件                       | 功能                     |
|----------------------------|-------------------------------|
| /arch/riscv/kernel/head.S  | 系统入口 |
| /arch/riscv/kernel/trap.c  | 处理中断             |
| /arch/riscv/kernel/clock.c | 处理时钟中断      |
| /arch/riscv/kernel/entry.S | 切换线程            |
| /arch/riscv/kernel/proc.c  | 管理线程            |
| /arch/riscv/kernel/vm.c    | 管理虚拟内存     |
