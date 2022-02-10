// arch/riscv/kernel/vm.c

#include <defs.h>
#include <types.h>
#include <mm.h>

extern char *_stext;
extern char *_etext;
extern char *_srodata;
extern char *_erodata;

/* early_pgtbl: 用于 setup_vm 进行 1GB 的 映射。 */
unsigned long  early_pgtbl[512] __attribute__((__aligned__(0x1000)));

/* swapper_pg_dir: kernel pagetable 根目录， 在 setup_vm_final 进行映射。 */
unsigned long  swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));

void setup_vm(void) {
    /* 
    1. 由于是进行 1GB 的映射 这里不需要使用多级页表 
    2. 将 va 的 64bit 作为如下划分： | high bit | 9 bit | 30 bit |
        high bit 可以忽略
        中间9 bit 作为 early_pgtbl 的 index
        低 30 bit 作为 页内偏移 这里注意到 30 = 9 + 9 + 12， 即我们只使用根页表， 根页表的每个 entry 都对应 1GB 的区域。 
    3. Page Table Entry 的权限 V | R | W | X 位设置为 1
    */
    int FLAG = 0xF;
    int PPN  = PHY_START >> 12;
    int VPN  = (VM_START  >> 30) & 0x1FF;
    early_pgtbl[VPN] = PPN << 10 | FLAG;
    early_pgtbl[2] = PPN << 10 | FLAG;
}

unsigned long VA2PA(unsigned long va) {
	return va - PA2VA_OFFSET;
}

void setup_vm_final(void) {
    memset(swapper_pg_dir, 0x0, PGSIZE);

    // No OpenSBI mapping required

    // mapping kernel text X|-|R|V
    int textSize = PGROUNDUP(&_etext) - PGROUNDUP(&_stext);
    create_mapping(swapper_pg_dir, &_stext, VA2PA(&_stext), textSize, 0xB);
    // create_mapping(swapper_pg_dir, VA2PA(&_stext), VA2PA(&_stext), textSize, 0xB);

    // mapping kernel rodata -|-|R|V
    int rodataSize = PGROUNDUP(&_erodata) - PGROUNDUP(&_srodata);
    create_mapping(swapper_pg_dir, &_srodata, VA2PA(&_srodata), rodataSize, 0x3);
    // create_mapping(swapper_pg_dir, VA2PA(&_srodata), VA2PA(&_srodata), rodataSize, 0x3);

    // mapping other memory -|W|R|V
    create_mapping(swapper_pg_dir, (unsigned long)&_srodata + rodataSize, VA2PA((unsigned long)&_srodata + rodataSize), PHY_SIZE - textSize - rodataSize, 0x7);
    // create_mapping(swapper_pg_dir, VA2PA((unsigned long)&_srodata + rodataSize), VA2PA((unsigned long)&_srodata + rodataSize), PHY_SIZE - textSize - rodataSize, 0x7);
	

    // set satp with swapper_pg_dir
	unsigned long satp = 0x8000000000000000 | (VA2PA(swapper_pg_dir) >> 12);
	__asm__ volatile(
		"mv t0, %[satp]\n"
		"csrw satp, t0\n"
		:: [satp] "r" (satp)
		: "memory"
	);

    // flush TLB
    asm volatile("sfence.vma zero, zero");
    return;
}

void create_page(uint64 *pgtbl, uint64 va, uint64 pa, int perm) {
	// printk("%lx %lx %lx\n", pgtbl, va, pa);
	unsigned long VPN[3];
	unsigned long *TBL[3];
	
	VPN[2] = (va >> 30) & 0x1FF;
	VPN[1] = (va >> 21) & 0x1FF;
	VPN[0] = (va >> 12) & 0x1FF;

	TBL[2] = swapper_pg_dir;
	
	if((TBL[2][VPN[2]] & 1) == 0) {
		TBL[1] = kalloc();
		TBL[2][VPN[2]] = ((VA2PA(TBL[1]) >> 12) << 10) | 1;
		//printk("%lx\n", TBL[1]);
		//printk("%lx\n", TBL[2][VPN[2]]);
	}
	else {
		TBL[1] = (TBL[2][VPN[2]] << 2) & 0xFFFFFFFFFFF000;
	}
	
	if((TBL[1][VPN[1]] & 1) == 0) {
		TBL[0] = kalloc();
		TBL[1][VPN[1]] = ((VA2PA(TBL[0]) >> 12) << 10) | 1;
	}
	else {
		TBL[0] = (TBL[1][VPN[1]] << 2) & 0xFFFFFFFFFFF000;
	}

	TBL[0][VPN[0]] = ((pa >> 12) << 10) | perm;
	
}

/* 创建多级页表映射关系 */
void create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm) {
    /*
    pgtbl 为根页表的基地址
    va, pa 为需要映射的虚拟地址、物理地址
    sz 为映射的大小
    perm 为映射的读写权限

    创建多级页表的时候可以使用 kalloc() 来获取一页作为页表目录
    可以使用 V bit 来判断页表项是否存在
    */
    int pageNum = sz / PGSIZE;

    for(int i = 0;i < pageNum;i++) {
    	create_page(pgtbl, va + PGSIZE * i, pa + PGSIZE * i, perm);
    }
}
