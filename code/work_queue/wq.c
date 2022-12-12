
#include <emmintrin.h>

internal void
WQ_SetupQueue(work_queue *WorkQueue, uint32_t WorkersCount)
{
  WQ_CreateSemaphore(&WorkQueue->AvailableWorkSemaphore, 0, WorkersCount);
  
  WorkQueue->WorkersCount = WorkersCount;
}

#if 0
internal void
SignalWaitingWorkers(work_queue *WorkQueue)
{
  for (;;)
  {
    int64_t SemValue = WorkQueue->AvailableWorkSemaphoreValue;
    if (SemValue + 1 < (int64_t)WorkQueue->WorkersCount)
    {
      if (WQ_AtomicCompareAndExchange64(&WorkQueue->AvailableWorkSemaphoreValue, SemValue, SemValue + 1))
      {
        WQ_ReleaseSemaphore(&WorkQueue->AvailableWorkSemaphore);
      }
      else
      {
        continue;
      }
    }
    break;
  }
}
#endif

internal next_work
WQ_GetNextWorkToDo(work_queue *WorkQueue)
{
  next_work Result = {0};
  
  uint64_t OldHead = WorkQueue->Head;
  
  if (WorkQueue->Head != WorkQueue->CommittedTail)
  {
    uint64_t EntryIndex = OldHead % ArrayCount(WorkQueue->Entries);
    Result.WorkEntry = WorkQueue->Entries[EntryIndex];
    if (WQ_AtomicCompareAndExchange64(&WorkQueue->Head, OldHead, OldHead + 1))
    {
      Result.IsValid = 1;
    }
  }
  
  return Result;
}

internal int32_t
WQ_ProcessOneEntry(worker_id WorkerID, work_queue *WorkQueue)
{
  int32_t Result = 0;
  
  next_work NextWorkToDo = WQ_GetNextWorkToDo(WorkQueue);
  if (NextWorkToDo.IsValid)
  {
    Result = 1;
    work_entry WorkEntry = NextWorkToDo.WorkEntry;
    WorkEntry.Work(WorkerID, WorkEntry.Data);
  }
  Assert(WorkQueue->Head <= WorkQueue->CommittedTail);
  return Result;
}

internal int32_t
WQ_PushEntrySP(work_queue *WorkQueue, worker_work *Work, void *Data)
{
  int32_t Result = 0;
  Assert(Work && Data);
  if ((WorkQueue->CommittedTail - WorkQueue->Head) != ArrayCount(WorkQueue->Entries))
  {
    Result = 1;
    uint64_t EntryIndex = WorkQueue->CommittedTail % ArrayCount(WorkQueue->Entries);
    
    WorkQueue->Entries[EntryIndex].Work = Work;
    WorkQueue->Entries[EntryIndex].Data = Data;
    
    CompletePreviousWritesBeforeFutureWrites();
    
    // NOTE(fakhri): make the changes visible
    ++ WorkQueue->CommittedTail;
    // NOTE(fakhri): wake up any worker thread that is waiting for a work entry
    WQ_ReleaseSemaphore(&WorkQueue->AvailableWorkSemaphore);
  }
  
  return Result;
}

internal int32_t
WQ_PushEntry(work_queue *WorkQueue, worker_work *Work, void *Data)
{
  int32_t Result = 0;
  
  uint64_t OurTail = WQ_AtomicIncrement64(&WorkQueue->ClaimedTail);
  
  if (OurTail - WorkQueue->Head >= ArrayCount(WorkQueue->Entries))
  {
    // NOTE(fakhri): queue was full
    // NOTE(fakhri): decrement the tail to undo the atomic increment we just did
    // before we found out that the queue was full
    uint64_t Expected = OurTail + 1;
    while((OurTail - WorkQueue->Head >= ArrayCount(WorkQueue->Entries)) &&
          (!WQ_AtomicCompareAndExchange64(&WorkQueue->ClaimedTail, Expected, OurTail)))
    {
      _mm_pause();
    }
  }
  
  if (OurTail - WorkQueue->Head < ArrayCount(WorkQueue->Entries))
  {
    uint64_t EntryIndex = OurTail % ArrayCount(WorkQueue->Entries);
    
    WorkQueue->Entries[EntryIndex].Work = Work;
    WorkQueue->Entries[EntryIndex].Data = Data;
    
    uint64_t Expected = OurTail;
    while(!WQ_AtomicCompareAndExchange64(&WorkQueue->CommittedTail, Expected, OurTail + 1))
    {
      _mm_pause();
    }
    
    Result = 1;
    // NOTE(fakhri): wake up any worker thread that is waiting for a work entry
    WQ_ReleaseSemaphore(&WorkQueue->AvailableWorkSemaphore);
  }
  
  return Result;
}

internal
SYS_THREAD_SIG(WorkerThreadMain)
{
  struct worker_info *WorkerInfo = (struct worker_info *)Input;
  work_queue *WorkQueue = (work_queue *)WorkerInfo->WorkQueue;
  for(;;)
  {
    next_work WorkToDo = WQ_GetNextWorkToDo(WorkQueue);
    if (WorkToDo.IsValid)
    {
      WorkToDo.WorkEntry.Work(WorkerInfo->WorkerID, WorkToDo.WorkEntry.Data);
    }
    else
    {
      // NOTE(fakhri): work queue was empty, call the os
      // to see what should we do about it
      WQ_AcquireSemaphore(&WorkQueue->AvailableWorkSemaphore);
    }
  }
}
