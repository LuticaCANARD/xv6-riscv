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
    printf("                       ███████╗██╗  ██╗██╗██████╗  \n");
    printf("                       ██╔════╝██║  ██║██║██╔══██╗ \n");
    printf("                       ██╔════╝██║██║  ██║██╔══██╗ \n");
    printf("                       ███████╗█████   ██║██║  ██║ \n");
    printf("                       ╚═══██║██╔ ██  ██║██║  ██║ \n");
    printf("                       ███████║██║  ██║██║██████╔╝ \n");
    printf("                       ╚══════╝╚═╝  ╚═╝╚═╝╚═════╝  \n");
    printf("\n");
    printf("CPU %d: starting xv6 kernel\n", cpuid());
    /**
     * the kernel expects there to be RAM
     * for use by the kernel and user pages
     * from physical address 0x80000000 to PHYSTOP.
     */
    printf(" basic physical memory: %ldMB\n", ((PHYSTOP - KERNBASE) >> 20));
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
