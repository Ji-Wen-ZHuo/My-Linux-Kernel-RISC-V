//arch/riscv/kernel/proc.c

#include "proc.h"

extern void __dummy();
extern void __switch_to(struct task_struct* prev, struct task_struct* next);

struct task_struct* idle;           // idle process
struct task_struct* current;        // 指向当前运行线程的 `task_struct`
struct task_struct* task[NR_TASKS]; // 线程数组，所有的线程都保存在此

void task_init() {
    // 1. 调用 kalloc() 为 idle 分配一个物理页
    // 2. 设置 state 为 TASK_RUNNING;
    // 3. 由于 idle 不参与调度 可以将其 counter / priority 设置为 0
    // 4. 设置 idle 的 pid 为 0
    // 5. 将 current 和 task[0] 指向 idle

    /* YOUR CODE HERE */
    
    idle = kalloc();
    idle->state = TASK_RUNNING;
    idle->counter = 0;
    idle->priority = 0;
    idle->pid = 0;
    current = idle;
    task[0] = 0;

    // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进行初始化
    // 2. 其中每个线程的 state 为 TASK_RUNNING, counter 为 0, priority 使用 rand() 来设置, pid 为该线程在线程数组中的下标。
    // 3. 为 task[1] ~ task[NR_TASKS - 1] 设置 `thread_struct` 中的 `ra` 和 `sp`,
    // 4. 其中 `ra` 设置为 __dummy （见 4.3.2）的地址， `sp` 设置为 该线程申请的物理页的高地址

    /* YOUR CODE HERE */

    for(int i = 1;i < NR_TASKS;i++) {
    	task[i] = kalloc();
    	task[i]->counter = 0;
    	task[i]->priority = rand();
    	task[i]->pid = i;
    	task[i]->thread.ra = __dummy;
    	// uint64 sp = task[i] + 4096;
    	uint64 sp = task[i];
    	sp += 4096;
    	task[i]->thread.sp = sp;
    	// printk("%lx\n", task[i]->thread.sp);
    	// printk("SET [PID = %d PRIORITY = %d COUNTER = %d]\n",task[i]->pid,task[i]->priority,task[i]->counter);
    }

    printk("...proc_init done!\n");
}

void dummy() {
    uint64 MOD = 1000000007;
    uint64 auto_inc_local_var = 0;
    int last_counter = -1;
    while(1) {
        if (last_counter == -1 || current->counter != last_counter) {
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
            // printk("[PID = %d] is running. auto_inc_local_var = %d\n", current->pid, auto_inc_local_var);
			printk("[PID = %d] is running! thread space begin at 0x%lx\n", current->pid, current);
        }
    }
}

void switch_to(struct task_struct* next) {
	if(next == current) return;
	else {
		printk("switch to [PID = %d PRIORITY = %d COUNTER = %d]\n", next->pid, next->priority, next->counter);
		struct task_struct* previous = current;
		current = next;
		if(next->thread.ra == __dummy) next->counter--;
		 __switch_to(previous, next);
	}
}

void do_timer() {
	if(current == idle) schedule();
	else {
		if(current->counter > 0) current->counter--;
		else schedule();
	}
}

void schedule() {
	schedule_DSJF();
	// schedule_DPRIORITY();
}

void schedule_DSJF() {
	int minCounter = 99999999 , minTask = 0;
	for(int i = 1;i < NR_TASKS;i++) {
		if(task[i]->counter < minCounter && task[i]->counter != 0) {
			minCounter = task[i]->counter;
			minTask = i;
		}
	}
	if(minTask == 0) {
		for(int i = 1;i < NR_TASKS;i++) {
			task[i]->counter = rand();
    		printk("SET [PID = %d PRIORITY = %d COUNTER = %d]\n",task[i]->pid,task[i]->priority,task[i]->counter);
		}
		schedule();
	}
	else {
		switch_to(task[minTask]);
	}
}

void schedule_DPRIORITY() {
	int maxPriority = 0 , maxTask = 0;
	for(int i = 1;i < NR_TASKS;i++) {
		if(task[i]->priority > maxPriority && task[i]->counter != 0) {
			maxPriority = task[i]->priority;
			maxTask = i;
		}
	}
	if(maxTask == 0) {
		for(int i = 1;i < NR_TASKS;i++) {
			task[i]->counter = rand();
    		printk("SET [PID = %d PRIORITY = %d COUNTER = %d]\n",task[i]->pid,task[i]->priority,task[i]->counter);
		}
		schedule();
	}
	else {
		switch_to(task[maxTask]);
	}
}
