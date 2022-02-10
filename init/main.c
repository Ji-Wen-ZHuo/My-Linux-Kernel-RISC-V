#include "printk.h"
#include "sbi.h"

extern void test();

int start_kernel() {
    // puti(2021);
    // puts(" Hello RISC-V\n");

	// printk("%d %s",2021," Hello RISC-V\n");
    test(); // DO NOT DELETE !!!

	return 0;
}
