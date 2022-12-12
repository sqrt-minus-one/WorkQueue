
#define WORK_QUEUE_IMPLEMENTATION
#include "work_queue/wq.h"

#include <stdio.h>

global work_queue GlobalWorkQueue;

volatile int64_t ProcessdedWork = 0;

internal
WORK_TASK_SIG(SayHelloXTimes)
{
  int X = (int)((size_t)Input);
  printf("thread[%d] processing thrad with input=%d\n", WorkerID, X);
  WQ_AtomicIncrement64(&ProcessdedWork);
}

int main()
{
  // NOTE(fakhri): init the work queue
#define WORKERS_COUNT 4
  WQ_SetupQueue(&GlobalWorkQueue, WORKERS_COUNT);
  
  // NOTE(fakhri): launch 4 worker threads
  for (uint32_t ThreadIndex = 0;
       ThreadIndex < WORKERS_COUNT;
       ++ThreadIndex)
  {
    struct worker_info *WorkerInfo = (struct worker_info*)malloc(sizeof(struct worker_info));
    
    WorkerInfo->WorkQueue = &GlobalWorkQueue;
    WorkerInfo->WorkerID = ThreadIndex;
    
    CloseHandle(CreateThread(0, 0, WorkerThreadMain, WorkerInfo, 0, 0));
  }
  
  // NOTE(fakhri): push tasks
  printf("pushing 10 tasks to the queue\n");
  for (int i = 0; i < 10; ++i)
  {
    void *Input = (void*)(i + 1ll); 
    WQ_PushEntry(&GlobalWorkQueue, SayHelloXTimes, Input);
  }
  
  // NOTE(fakhri): wait for all the tasks to finish
  // NOTE(fakhri): the waiting thread will participate
  // in executing the tasks in the queue
  while(ProcessdedWork < 10) WQ_ProcessOneEntry(4, &GlobalWorkQueue);
  printf("finished the 10 tasks");
  return 0;
}
