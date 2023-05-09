#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  //在xv6中，不能直接将系统调用的参数作为函数的参数传递。
  //相反，我们需要使用argaddr()和argint()函数将参数复制到内核空间中，并在函数中使用这些复制后的参数值。
  //这样，我们就可以在内核空间中安全地访问这些参数值，而不会影响用户空间中的数据。
  // lab pgtbl: your code here.
  int numpage;
  uint64 pageaddr,useraddr;
  struct proc *p = myproc();
  if(argaddr(0, &pageaddr) < 0) //arg0 is the address of the page
    return -1;
  if(argint(1, &numpage) < 0)  //arg1 is the number of pages
    return -1;
  if(argaddr(2, &useraddr) < 0) //arg2 is a user address to a buffer to store the results into a bitmask
    return -1;
  if(numpage>32)
    return -1;
  uint64 res=0;
  uint64 abit=1;
  for(uint64 i=0;i<numpage;i++)
  {
    uint64 va=pageaddr+i*PGSIZE;
    pte_t* pte = walk(p->pagetable, va, 0); //get the pte
    if((*pte & PTE_A)!=0){ //check the access bit
      *pte=*pte&(~PTE_A); //clear the access bit
      res = res|(abit<<i); //set the bit in the bitmask
    }
  }
  if(copyout(p->pagetable, useraddr,(char*)&res,sizeof(res))<0) //copy the bitmask to the user address
    return -1;
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
