#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    // ------------------ PROFILE ------------------
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("███████╗██╗  ██╗██╗██████╗     ██╗      █████╗ ██████╗ \n");
    printf("██╔════╝██║  ██║██║██╔══██╗    ██║     ██╔══██╗██╔══██╗\n");
    printf("██╔════╝██║██║  ██║██╔══██╗    ██║     ██╔══██╗██╔══██╗\n");
    printf("███████╗█████   ██║██║  ██║    ██║     ███████║██████╔╝\n");
    printf("╚════██║██╔ ██  ██║██║  ██║    ██║     ██╔══██║██╔══██╗\n");
    printf("███████║██║  ██║██║██████╔╝    ███████╗██║  ██║██████╔╝\n");
    printf("╚══════╝╚═╝  ╚═╝╚═╝╚═════╝     ╚══════╝╚═╝  ╚═╝╚═════╝ \n");
    printf("\n");
    printf("CPU %d: starting xv6 kernel\n", cpuid());
    printf("  physical memory: %dMB\n", (riscv_memsize()>> 20));
    printf("  kernel virtual memory: %dMB\n", (KERNBASE>> 20));
    printf(" today is %s\n", ctime(riscv_time()));
    printf("  RISC-V ISA: %s\n", riscv_isa_string());

    //-------------------------------------- Z INIT --------------------------------------
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
