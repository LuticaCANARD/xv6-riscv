#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
  ushort inuse[NPROC];   // whether this slot of the process table is in use (1 or 0)
  uint64 tickets[NPROC]; // the number of tickets this process has
  uint64 pid[NPROC];     // the PID of each process 
  uint64 ticks[NPROC];   // the number of ticks each process has accumulated 
};

#endif // _PSTAT_H_